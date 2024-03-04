#include "pch.h"
#include "Entity.h"
#include "MeshRenderer.h"
Entity::Entity(ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB) {
	m_mTransform = Transform();
	this->md3dDevice = md3dDevice;
	this->mCommandList = mCommandList;
	this->mObjectCB = std::move(mObjectCB);
}

Entity::~Entity() {

}

void Entity::CreateCube() {
	Component* com = new MeshRenderer(this);
	m_mComponents["cube"] = com;
}