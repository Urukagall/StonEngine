#include "pch.h"
#include "Entity.h"
#include "MeshRenderer.h"
Entity::Entity(ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList) {
	m_mTransform = Transform();
	this->md3dDevice = md3dDevice;
	this->mCommandList = mCommandList;
}

Entity::~Entity() {

}

void Entity::CreateCube() {
	Component* com = new MeshRenderer(this);
	m_mComponents["cube"] = com;
}