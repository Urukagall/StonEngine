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
