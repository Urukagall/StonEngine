#include "pch.h"
#include "Entity.h"

Entity::Entity(ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap, Render* pRender) {
	m_mTransform = Transform();
	
	// Create new collider assigned to this entity
	m_collider = new Collider();
	m_collider->BindEntity(this);

	this->md3dDevice = md3dDevice;
	this->mCommandList = mCommandList;
	this->mCbvHeap = mCbvHeap;
	this->m_pRender = pRender;
}

Entity::~Entity() {

}
void Entity::CreateScript(Script* pScript)
{
	m_script = pScript;
	m_script->OnLoad();
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

void Entity::CreateMissiles(MeshCreator* mc) {
	MeshRenderer* com = new MeshRenderer(this);
	com->mBoxGeo = mc->m_mMesh["missiles"];
	//com->Box(oColor);
	m_oMeshRenderers.insert(std::make_pair("missile", com));
}

void Entity::CreatePyramid(string sColor, MeshCreator* mc) {
	string sMesh = "pyramid_" + sColor;

	MeshRenderer* com = new MeshRenderer(this);
	com->mBoxGeo = mc->m_mMesh[sMesh];
	//com->Pyramid(oColor);
	m_oMeshRenderers.insert(std::make_pair("pyr", com));
}

void Entity::CreateEnemy(MeshCreator* mc) {
	MeshRenderer* com = new MeshRenderer(this);
	com->mBoxGeo = mc->m_mMesh["enemy"];
	//com->Pyramid(oColor);
	m_oMeshRenderers.insert(std::make_pair("enemy", com));
}

void Entity::SetScale(float x, float y, float z, bool scaleColliderSize) {
	m_mTransform.SetScale(x, y, z);
	if (scaleColliderSize) {
		float maxXY = Math::Max(x, y);
		float maxXYZ = Math::Max(maxXY, z);
		m_collider->SetScale(maxXYZ);
	}
}

void Entity::SetPosition(float x, float y, float z) {
	m_mTransform.SetPos(x, y, z);
}

void Entity::SetRotate(float yaw, float pitch, float roll) {
	m_mTransform.Rotate(yaw, pitch, roll);
}

void Entity::SetDirection(float velocity, float deltaTime) {
	m_mTransform.Walk(velocity, deltaTime);
}

void Entity::setTexture(std::string sName) {
	TextureEntity* texture = new TextureEntity();
	m_mTextures.insert(std::make_pair(sName, texture));
	//texture->Init()
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