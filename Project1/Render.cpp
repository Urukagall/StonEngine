#include "pch.h"
#include "Render.h"
#include "defines.h"
#include "Camera.h"

Camera camera;
Defines defines;

Render::Render(HINSTANCE hInstance)
	: Init(hInstance)
{
}

Render::~Render()
{
}

bool Render::Initialize()
{
	if (!Init::Initialize())
		return false;

	// Reset the command list to prep for initialization commands.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	BuildDescriptorHeaps();
	//BuildConstantBuffers(); //a retirer
	BuildRootSignature();
	BuildShadersAndInputLayout();
	CreateEntity();
	CreateParticles();
	BuildPSO();

	// Execute the initialization commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	FlushCommandQueue();

	return true;
}

void Render::OnResize()
{
	Init::OnResize();

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * Math::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void Render::HandleInput(Timer& gt)
{
	float dT = gt.GetDT();
	float speed = 0.001f;
	// Vérifiez les touches enfoncées et mettez à jour les valeurs de déplacement en conséquence
	if (input.getKey(pitchUp)) {
		camera.Pitch(speed * dT);
		
	}
	else if (input.getKey(pitchDown)) {
		camera.Pitch((-speed)* dT);
	}

	if (input.getKey(yawLeft)) {
		camera.Yaw((-speed) * dT);
	}
	else if (input.getKey(yawRight)) {
		camera.Yaw(speed * dT);
	}

	if (input.getKey(rollLeft)) {
		camera.Roll((-speed) * dT);
	}
	else if (input.getKey(rollRight)) {
		camera.Roll(speed * dT);
	}

	if (input.getKey(ARROW_UP)) {
		camera.Walk(speed * dT);
	}
	else if (input.getKey(ARROW_DOWN)) {
		camera.Walk((-speed) * dT);
	}
}

void Render::Update(Timer& gt)
{
	for (int i = 0; i < m_Particles.size(); i++) {
		m_Particles[i]->Update(gt.GetDT());
	}
	// Gérer les entrées utilisateur
	HandleInput(gt);

	camera.setPosition(x, y, z);
	camera.setView();

	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX worldViewProj = world * camera.getView() * proj;


	// ============ il faut mettre ça dans le MeshRenderer::Update() =================

	// Update the constant buffer with the latest worldViewProj matrix.
	/*ObjectConstants objConstants;
	XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));

	mObjectCB->CopyData(0, objConstants);*/

	//============ il faut juste appeller tout les update() des components ============

	XMFLOAT4X4 pr;
	XMStoreFloat4x4(&pr, proj);
	XMFLOAT4X4 cam;
	XMStoreFloat4x4(&cam, camera.getView());

	for (int i = 0; i < m_Entities.size(); ++i) {
		for (const auto& pair : m_Entities[i]->m_oMeshRenderers) {
			pair.second->Update(pr, cam);	
		}
		
	}

	for (int i = 0; i < m_Particles.size(); ++i) {
		for (int j = 0; j < m_Particles[i]->particles.size(); j++) {
			for (const auto& pair : m_Particles[i]->particles[j].m_oEntity->m_oMeshRenderers) {
				pair.second->Update(pr, cam);
			}
		}
	}
	m_Entities[0]->m_oMeshRenderers["cube"]->m_oEntity->m_mTransform.Rotate(0.0f, 0.1f, 0.0f);
	m_Entities[1]->m_oMeshRenderers["pyr"]->m_oEntity->m_mTransform.Rotate(0.0f, 0.0f, 0.1f);
}

void Render::Draw(const Timer& gt)
{
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);


	CD3DX12_RESOURCE_BARRIER targetState = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &targetState);

	// Clear the back buffer and depth buffer.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.

	D3D12_CPU_DESCRIPTOR_HANDLE backBuffer = CurrentBackBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencil = DepthStencilView();

	mCommandList->OMSetRenderTargets(1, &backBuffer, true, &depthStencil);

	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);


	//for (int i = 0; i < m_vEntities.size(); ++i) {
	//	mCommandList->IASetVertexBuffers(0, 1, &m_vEntities[i]->VertexBufferView());
	//	mCommandList->IASetIndexBuffer(&m_vEntities[i]->IndexBufferView());
	//	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());

	//	mCommandList->DrawIndexedInstanced(
	//		m_vEntities[i]->DrawArgs["box"].IndexCount,
	//		1, 0, 0, 0);
	//}


	for (int i = 0; i < m_Entities.size(); ++i) {
		for (const auto& pair : m_Entities[i]->m_oMeshRenderers) {
			// pair.first est la clé, pair.second est la valeur
			std::cout << "Clé : " << pair.first << ", Valeur : " << pair.second << std::endl;
			Mesh* comp = pair.second->mBoxGeo;

			mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
			mCommandList->SetPipelineState(mPSO.Get());

			D3D12_VERTEX_BUFFER_VIEW vertexBuffer = comp->VertexBufferView();
			D3D12_INDEX_BUFFER_VIEW indexBuffer = comp->IndexBufferView();
			mCommandList->IASetVertexBuffers(0, 1, &vertexBuffer);
			mCommandList->IASetIndexBuffer(&indexBuffer);
			mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


			//mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());
			mCommandList->SetGraphicsRootConstantBufferView(0, pair.second->mObjectCB->Resource()->GetGPUVirtualAddress());

			mCommandList->DrawIndexedInstanced(
				comp->DrawArgs["box"].IndexCount,
				1, 0, 0, 0);
		}
	}

	for (int i = 0; i < m_Particles.size(); ++i) {
		for (int j = 0; j < m_Particles[i]->particles.size(); ++j) {
			for (const auto& pair : m_Particles[i]->particles[j].m_oEntity->m_oMeshRenderers) {
				// pair.first est la clé, pair.second est la valeur
				std::cout << "Clé : " << pair.first << ", Valeur : " << pair.second << std::endl;
				Mesh* comp = pair.second->mBoxGeo;

				mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
				mCommandList->SetPipelineState(mPSO.Get());

				D3D12_VERTEX_BUFFER_VIEW vertexBuffer = comp->VertexBufferView();
				D3D12_INDEX_BUFFER_VIEW indexBuffer = comp->IndexBufferView();
				mCommandList->IASetVertexBuffers(0, 1, &vertexBuffer);
				mCommandList->IASetIndexBuffer(&indexBuffer);
				mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


				//mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());
				mCommandList->SetGraphicsRootConstantBufferView(0, pair.second->mObjectCB->Resource()->GetGPUVirtualAddress());

				mCommandList->DrawIndexedInstanced(
					comp->DrawArgs["box"].IndexCount,
					1, 0, 0, 0);
			}
		}

	}


	//for (int i = 0; i < m_Entities.size(); ++i) {
	//	for (int j = 0; j < m_Entities[i]->m_mComponents.size(); j++) {
	//		Mesh* comp = m_Entities[i]->m_mComponents["cube"]->mBoxGeo;

	//		D3D12_VERTEX_BUFFER_VIEW vertexBuffer = comp->VertexBufferView();
	//		D3D12_INDEX_BUFFER_VIEW indexBuffer = comp->IndexBufferView();
	//		mCommandList->IASetVertexBuffers(0, 1, &vertexBuffer);
	//		mCommandList->IASetIndexBuffer(&indexBuffer);
	//		mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//		mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());

	//		mCommandList->DrawIndexedInstanced(
	//			comp->DrawArgs["box"].IndexCount,
	//			1, 0, 0, 0);
	//	}

	//}

	CD3DX12_RESOURCE_BARRIER resssourceState = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &resssourceState);

	// Done recording commands.
	ThrowIfFailed(mCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// swap the back and front buffers
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	FlushCommandQueue();
}

/*
void Render::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void Render::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Render::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi += dy;

		// Restrict the angle mPhi.
		mPhi = Math::Clamp(mPhi, 0.1f, Math::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = Math::Clamp(mRadius, 3.0f, 15.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}
*/

void Render::BuildDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvHeapDesc,
		IID_PPV_ARGS(&mCbvHeap)));
}

//void Render::BuildConstantBuffers()
//{
//	mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice.Get(), 1, true);
//
//	UINT objCBByteSize = Tools::CalcConstantBufferByteSize(sizeof(ObjectConstants));
//
//	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
//	// Offset to the ith object constant buffer in the buffer.
//	int boxCBufIndex = 0;
//	cbAddress += boxCBufIndex * objCBByteSize;
//
//	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
//	cbvDesc.BufferLocation = cbAddress;
//	cbvDesc.SizeInBytes = Tools::CalcConstantBufferByteSize(sizeof(ObjectConstants));
//
//	md3dDevice->CreateConstantBufferView(
//		&cbvDesc,
//		mCbvHeap->GetCPUDescriptorHandleForHeapStart());
//}

void Render::BuildRootSignature()
{
	// Shader programs typically require resources as input (constant buffers,
	// textures, samplers).  The root signature defines the resources the shader
	// programs expect.  If we think of the shader programs as a function, and
	// the input resources as function parameters, then the root signature can be
	// thought of as defining the function signature.  

	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[1];

	// Create a single descriptor table of CBVs.
	//CD3DX12_DESCRIPTOR_RANGE cbvTable;
	//cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	slotRootParameter[0].InitAsConstantBufferView(0);

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&mRootSignature)));
}

void Render::BuildShadersAndInputLayout()
{
	HRESULT hr = S_OK;

	mvsByteCode = Tools::CompileShader(L"Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
	mpsByteCode = Tools::CompileShader(L"Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}

void Render::CreateEntity() {
	Entity* en = new Entity(md3dDevice, mCommandList, mCbvHeap);
	en->CreateCube(XMFLOAT4(Colors::Blue));
	en->m_mTransform.Scale(1.0f,0.5f,0.5f);
	m_Entities.push_back(en);
	Entity* py = new Entity(md3dDevice, mCommandList, mCbvHeap);
	py->CreatePyramid(XMFLOAT4(Colors::GreenYellow));
	py->m_mTransform.Scale(1.0f, 0.5f, 0.5f);
	m_Entities.push_back(py);
}

void Render::CreateParticles() {
	Particles* par = new Particles(5, md3dDevice, mCommandList, mCbvHeap);
	m_Particles.push_back(par);
}

void Render::BuildPSO()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.pRootSignature = mRootSignature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
		mvsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
		mpsByteCode->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = mBackBufferFormat;
	psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = mDepthStencilFormat;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
}