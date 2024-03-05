#include "pch.h"
#include "Entity.h"
#include "Particles.h"
Entity::Entity(ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList) {
	m_mTransform = Transform();
	this->md3dDevice = md3dDevice;
	this->mCommandList = mCommandList;
}

Entity::~Entity() {

}

void Entity::CreateCube(XMFLOAT4 oColor) {
	MeshRenderer* com = new MeshRenderer(this);
	com->Box(oColor);
	m_mComponents["cube"] = com;
}

void Entity::CreatePyramid(XMFLOAT4 oColor) {
	MeshRenderer* com = new MeshRenderer(this);
	com->Pyramid( oColor);
	m_mComponents["pyr"] = com;
}

void Entity::CreateParticles() {
	Particles* par = new Particles(this);
	m_mComponents["par"] = par;
}