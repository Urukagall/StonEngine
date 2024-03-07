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

	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	BuildDescriptorHeaps();
	BuildRootSignature();
	BuildShadersAndInputLayout();

	//CreateEntityCube(2.0,2.0,2.0, XMFLOAT4(Colors::Aquamarine));
	CreateEntituPyramid(1.0, 2.0, 2.0, XMFLOAT4(Colors::Aquamarine));

	CreateParticlesExplosion(2.0, 2.0, 2.0);
	CreateParticlesExplosion(3.0, 3.0, 3.0);
	CreateParticlesExplosion(4.0, 4.0, 4.0);

	BuildPSO();

	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	FlushCommandQueue();

	return true;
}

void Render::OnResize()
{
	Init::OnResize();


	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * Math::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void Render::HandleInput(Timer& gt)
{
	float dT = gt.GetDT();
	float speed = 0.001f;

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
			XMFLOAT4X4 pr;
			XMStoreFloat4x4(&pr, proj);
			XMFLOAT4X4 cam;
			XMStoreFloat4x4(&cam, camera.getView());
			for (const auto& pair : m_Particles[i]->particles[j]->m_oEntity->m_oMeshRenderers) {
				pair.second->Update(pr, cam);
			}
		}
	}
	m_Entities[0]->SetRotate(0.0, 0.01, 0.01);
	m_Entities[0]->SetScale(2.0,2.0,2.0);

}

void Render::Draw(const Timer& gt)
{

	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);


	CD3DX12_RESOURCE_BARRIER targetState = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	mCommandList->ResourceBarrier(1, &targetState);

	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::Black, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);


	D3D12_CPU_DESCRIPTOR_HANDLE backBuffer = CurrentBackBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencil = DepthStencilView();

	mCommandList->OMSetRenderTargets(1, &backBuffer, true, &depthStencil);

	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);



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
			for (const auto& pair : m_Particles[i]->particles[j]->m_oEntity->m_oMeshRenderers) {
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



	CD3DX12_RESOURCE_BARRIER resssourceState = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	mCommandList->ResourceBarrier(1, &resssourceState);

	ThrowIfFailed(mCommandList->Close());

	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	FlushCommandQueue();
}



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


void Render::BuildRootSignature()
{

	CD3DX12_ROOT_PARAMETER slotRootParameter[1];

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

void Render::CreateEntity(float x, float y, float z) {

	Entity* en = new Entity(md3dDevice, mCommandList, mCbvHeap);
	en->SetPosition(x, y, z);
	m_Entities.push_back(en);
}

void Render::CreateEntityCube(float x, float y, float z, XMFLOAT4 oColor) {
	Entity* en = new Entity(md3dDevice, mCommandList, mCbvHeap);
	en->CreateCube(oColor);
	en->SetPosition(x, y, z);
	m_Entities.push_back(en);
}

void Render::CreateEntituPyramid(float x, float y, float z, XMFLOAT4 oColor) {
	Entity* py = new Entity(md3dDevice, mCommandList, mCbvHeap);
	py->CreatePyramid(XMFLOAT4(Colors::GreenYellow));
	py->SetPosition(x, y, z);
	m_Entities.push_back(py);
}

void Render::CreateParticlesExplosion(float x, float y, float z) {
	XMFLOAT3 pos = XMFLOAT3(x, y, z);
	Particles* par1 = new Particles(XMFLOAT4(Colors::Red), 150, md3dDevice, mCommandList, mCbvHeap, pos);
	Particles* par2 = new Particles(XMFLOAT4(Colors::Orange), 100, md3dDevice, mCommandList, mCbvHeap, pos);
	Particles* par3 = new Particles(XMFLOAT4(Colors::Yellow), 50, md3dDevice, mCommandList, mCbvHeap, pos);
	m_Particles.push_back(par1);
	m_Particles.push_back(par2);
	m_Particles.push_back(par3);
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