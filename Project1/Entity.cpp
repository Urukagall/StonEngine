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

void Entity::CreatePlane(string sColor, MeshCreator* mc)
{
	string sMesh = "plane_" + sColor;

	MeshRenderer* com = new MeshRenderer(this);
	com->mBoxGeo = mc->m_mMesh[sMesh];
	//com->Plane(oColor);
	m_oMeshRenderers.insert(std::make_pair("plane", com));
}

void Entity::CreateCube(string sColor, MeshCreator* mc) {
	string sMesh = "box_" + sColor;

	MeshRenderer* com = new MeshRenderer(this);
	com->mBoxGeo = mc->m_mMesh[sMesh];
	//com->Box(oColor);
	m_oMeshRenderers.insert(std::make_pair("cube", com));
}

void Entity::CreatePyramid(string sColor, MeshCreator* mc) {
	string sMesh = "pyramid_" + sColor;

	MeshRenderer* com = new MeshRenderer(this);
	com->mBoxGeo = mc->m_mMesh[sMesh];
	//com->Pyramid(oColor);
	m_oMeshRenderers.insert(std::make_pair("pyr", com));
}

void Entity::SetScale(float x, float y, float z) {
	m_mTransform.SetScale(x, y, z);
}

void Entity::SetPosition(float x, float y, float z) {
	m_mTransform.SetPosition(x, y, z);
}

void Entity::SetRotate(float yaw, float pitch, float roll) {
	m_mTransform.Rotate(yaw, pitch, roll);
}

void Entity::SetDirection(float velocity, float deltaTime) {
	m_mTransform.Walk(velocity, deltaTime);
}

bool Entity::DeleteComponent(std::string name) {
	auto it = m_oMeshRenderers.find(name);

	if (it != m_oMeshRenderers.end()) {
		// Element found, so delete it
		delete(m_oMeshRenderers[name]);
		m_oMeshRenderers.erase(name);
		return true;
	}
	else {
		return false;
	}

}