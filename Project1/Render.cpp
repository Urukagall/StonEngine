#include "pch.h"
#include "Render.h"
#include "defines.h"

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

	mc = new MeshCreator(mCommandList, md3dDevice);
	
	mc->Init();

	//CreateEntityCube(2.0, 2.0, 2.0, "red");
	//CreateEntituPyramid(1.0, 2.0, 2.0, XMFLOAT4(Colors::Aquamarine));

	//CreateParticlesExplosion(2.0, 2.0, 2.0);
	//CreateParticlesExplosion(3.0, 3.0, 3.0);
	//CreateParticlesExplosion(4.0, 4.0, 4.0);

	BuildPSO();


	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	FlushCommandQueue();

	//Set default position
	camera.setPosition(x, y, z);

	return true;
}

void Render::OnResize()
{
	Init::OnResize();


	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * Math::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

Input* Render::GetInput() {
	return &input;
}

void Render::HandleInput(Timer& gt)
{
	float dT = gt.GetDT();
	float speed = 0.001f;

	if (input.getKey(SPRINT)) {
		speed = 0.01f;
	}

	// Vérifiez les touches enfoncées et mettez à jour les valeurs de déplacement en conséquence
	if (input.getKey(pitchUp)) {
		camera.m_transform->Rotate(0, speed * dT, 0);
		//camera.Pitch(speed * dT);
	}
	else if (input.getKey(pitchDown)) {
		camera.m_transform->Rotate(0, -speed * dT, 0);
		//camera.Pitch((-speed)* dT);
	}

	if (input.getKey(yawLeft)) {
		camera.m_transform->Rotate(-speed * dT, 0, 0);
		//camera.Yaw((-speed) * dT);
	}
	else if (input.getKey(yawRight)) {
		camera.m_transform->Rotate(speed * dT, 0, 0);
		//camera.Yaw(speed * dT);
	}

	if (input.getKey(rollLeft)) {
		camera.m_transform->Rotate(0, 0, speed * dT);
		//camera.Roll(speed * dT);
	}
	else if (input.getKey(rollRight)) {
		camera.m_transform->Rotate(0, 0, -speed * dT);
	}

	if (input.getKey(ARROW_UP)) {
		//camera.m_transform->Walk(speed*10, dT);
		camera.m_transform->VelocityWalk(speed * 0.5f);
		//camera.m_transform->SetVelocity({ 0.0f, 0.0f, 0.05f});
	}
	else if (input.getKey(ARROW_DOWN)) {
		//camera.m_transform->Walk(-speed*10, dT);
		camera.m_transform->VelocityWalk(-speed * 0.5f);
		//camera.m_transform->SetVelocity({ 0.0f, 0.0f, -0.05f});
	}

	if (input.getKey(ARROW_RIGHT)) {
		// MISSILE
		//camera.Strafe(speed * dT);
	}
	else if (input.getKey(ARROW_LEFT)) {
		// SHOOT
		//camera.Strafe((-speed) * dT);
	}
}

void Render::Update(Timer& gt)
{
	float dT = gt.GetDT();

	for (int i = 0; i < m_Particles.size(); i++) {
		m_Particles[i]->Update(dT);
	}
	// Gérer les entrées utilisateur
	HandleInput(gt);

	// Update camera
	//camera.setPosition(x, y, z);
	camera.m_transform->ApplyVelocity(dT);
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

	for (int a = 0; a < m_Entities.size(); ++a) {
		// Check collisions with other objects
		for (int b = 0; b < m_Entities.size(); ++b) {
			if(a!=b){
				/*OutputDebugStringA("\nChecking collisions between ");
				OutputDebugStringA(std::to_string(a).c_str());
				OutputDebugStringA(" and ");
				OutputDebugStringA(std::to_string(b).c_str());*/
				//Check collision
				if (m_Entities[a]->m_collider->CheckColl(m_Entities[b])) {
					//OutputDebugStringA("\nCollision");					
				}
				else {
					//OutputDebugStringA("\nNOPE");
				}
			}
		}

		// Update renderer
		for (const auto& pair : m_Entities[a]->m_oMeshRenderers) {
			pair.second->Update(pr, cam);
		}
		if (m_Entities[a]->m_script !=nullptr) {
			//OutputDebugStringA("bbbb\n");
			m_Entities[a]->m_script->Update(dT);
		}

	}

	for (int i = 0; i < m_Particles.size(); ++i) {
		if (m_Particles[i]->particles.size() == 0) {
			m_Particles.erase(m_Particles.begin() + i);
		}
		else {
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
	}
}

void Render::Draw(const Timer& gt)
{
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), mPSO.Get()));

	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);


	CD3DX12_RESOURCE_BARRIER targetState = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &targetState);

	// Clear the back buffer and depth buffer.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::Black, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.

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
					comp->DrawArgs["plane"].IndexCount,
					1, 0, 0, 0);
			}
		}

	}



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

Entity* Render::CreateEntity(float posx, float posy, float posz) {
	Entity* en = new Entity(md3dDevice, mCommandList, mCbvHeap, this);
	en->SetPosition(posx, posy, posz);
	m_Entities.push_back(en);

	return en;
}

Entity* Render::CreateEntityCube(float x, float y, float z, string sColor) {
	Entity* cu = new Entity(md3dDevice, mCommandList, mCbvHeap, this);
	cu->CreateCube(sColor, mc);
	cu->SetPosition(x, y, z);
	m_Entities.push_back(cu);

	return cu;
}

Entity* Render::CreateEntityMissiles(float x, float y, float z) {
	Entity* mi = new Entity(md3dDevice, mCommandList, mCbvHeap, this);
	mi->CreateMissiles(mc);
	mi->SetPosition(x, y, z);
	m_Entities.push_back(mi);

	return mi;
}

Entity* Render::CreateEntityPyramid(float x, float y, float z, string sColor) {
	Entity* py = new Entity(md3dDevice, mCommandList, mCbvHeap, this);
	py->CreatePyramid(sColor, mc);
	py->SetPosition(x, y, z);
	m_Entities.push_back(py);

	return py;
}

Entity* Render::CreateEntityEnemy(float x, float y, float z) {
	Entity* en = new Entity(md3dDevice, mCommandList, mCbvHeap, this);
	en->CreateEnemy(mc);
	en->SetPosition(x, y, z);
	m_Entities.push_back(en);

	return en;
}

void Render::CreateParticle(float x, float y, float z, string sColor, int minLife, int maxLife, int minScale, int maxScale, int minSpeed, int maxSpeed, int particleNumber) {
	XMFLOAT3 pos = XMFLOAT3(x, y, z);
	Particles* par = new Particles(this, sColor, mc, 150, md3dDevice, mCommandList, mCbvHeap, pos, minLife, maxLife, minScale, maxScale, minSpeed, maxSpeed);
	m_Particles.push_back(par);
}

void Render::CreateParticlesExplosion(float x, float y, float z) {
	XMFLOAT3 pos = XMFLOAT3(x, y, z);
	Particles* par1 = new Particles(this, "red", mc, 150, md3dDevice, mCommandList, mCbvHeap, pos);
	Particles* par2 = new Particles(this, "orange", mc, 100, md3dDevice, mCommandList, mCbvHeap, pos);
	Particles* par3 = new Particles(this, "yellow", mc, 50, md3dDevice, mCommandList, mCbvHeap, pos);
	m_Particles.push_back(par1);
	m_Particles.push_back(par2);
	m_Particles.push_back(par3);
}

void Render::CreateParticlesFire(float x, float y, float z) {
	XMFLOAT3 pos = XMFLOAT3(x, y, z);
	Particles* par1 = new Particles(this, "gray", mc, 150, md3dDevice, mCommandList, mCbvHeap, pos, 1000, 2000, 10, 20, 10, 100);
	Particles* par2 = new Particles(this, "dark", mc, 100, md3dDevice, mCommandList, mCbvHeap, pos, 1000, 2000, 10, 20, 10, 100);
	m_Particles.push_back(par1);
	m_Particles.push_back(par2);
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