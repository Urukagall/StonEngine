#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Entity.h"
MeshRenderer::MeshRenderer(Entity* pEntity): Component(pEntity) {

	m_oEntity = pEntity;
	md3dDevice = m_oEntity->md3dDevice;
	mCommandList = m_oEntity->mCommandList;
	mCbvHeap = m_oEntity->mCbvHeap;
	BuildConstantBuffers();
}

MeshRenderer::~MeshRenderer(){

}

void MeshRenderer::BuildConstantBuffers()
{
	mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice.Get(), 1, true);

	UINT objCBByteSize = Tools::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	
	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
	// Offset to the ith object constant buffer in the buffer.
	int boxCBufIndex = 0;
	cbAddress += boxCBufIndex * objCBByteSize;
	
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = cbAddress;
	cbvDesc.SizeInBytes = Tools::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	
	md3dDevice->CreateConstantBufferView(
		&cbvDesc,
		mCbvHeap->GetCPUDescriptorHandleForHeapStart());
}

void MeshRenderer::Update(XMFLOAT4X4 proj, XMFLOAT4X4 cam)
{



	XMFLOAT4X4 world = m_oEntity->m_mTransform.GetMatrix();

	XMMATRIX worldM = XMLoadFloat4x4(&world);
	XMMATRIX camM = XMLoadFloat4x4(&cam);
	XMMATRIX projM = XMLoadFloat4x4(&proj);

	XMMATRIX worldViewProj = worldM * camM * projM;
	XMStoreFloat4x4(&world, worldViewProj);


	ObjectConstants objConstants;
	XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(XMLoadFloat4x4(&world)));

	mObjectCB->CopyData(0, objConstants);
}

void MeshRenderer::Box(XMFLOAT4 oColor) {
	std::array<Vertex, 8> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), oColor }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), oColor }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), oColor }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), oColor })
	};

	std::array<std::uint16_t, 36> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	mBoxGeo = new Mesh();
	mBoxGeo->Name = "boxGeo";

	mBoxGeo->VertexBufferGPU = Tools::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = Tools::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(Vertex);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;

	Submesh submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mBoxGeo->DrawArgs["box"] = submesh;
}

void MeshRenderer::Pyramid(XMFLOAT4 oColor) {
	std::array<Vertex, 5> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, 0.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(-1.0f, 0.0f, 1.0f), oColor }),
		Vertex({ XMFLOAT3(1.0f, 0.0f, 1.0f), oColor }),
		Vertex({ XMFLOAT3(1.0f, 0.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(0.0f, 1.0f, 0.0f), oColor }),

	};

	std::array<std::uint16_t, 18> indices =
	{
		// front face
		0, 2, 1,
		0, 3, 2,

		// back face (ordre antihoraire)
		0, 1, 4,
		0, 4, 3,

		// left face (ordre antihoraire)
		2, 4, 1,
		2, 3, 4,


	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	mBoxGeo = new Mesh();
	mBoxGeo->Name = "boxGeo";

	mBoxGeo->VertexBufferGPU = Tools::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = Tools::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(Vertex);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;

	Submesh submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mBoxGeo->DrawArgs["box"] = submesh;
}
