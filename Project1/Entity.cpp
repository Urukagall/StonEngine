#include "pch.h"
#include "Entity.h"

Entity::Entity(ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap) {
	m_mTransform = Transform();
	this->md3dDevice = md3dDevice;
	this->mCommandList = mCommandList;
	this->mCbvHeap = mCbvHeap;
}

Entity::~Entity() {

}

void Entity::CreateCube(XMFLOAT4 oColor) {
	MeshRenderer* com = new MeshRenderer(this);
	com->Box(oColor);
	m_oMeshRenderers.insert(std::make_pair("cube", com));
}

void Entity::CreatePyramid(XMFLOAT4 oColor) {
	MeshRenderer* com = new MeshRenderer(this);
	com->Pyramid(oColor);
	m_oMeshRenderers.insert(std::make_pair("pyr", com));
}
