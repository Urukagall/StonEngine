#include "pch.h"
#include "Transform.h"

Transform::Transform() {
	Identity();
}

void Transform::Identity() {
	XMFLOAT4X4 mIdentity(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	m_vSca = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_mSca = mIdentity;

	m_vPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_mPos = mIdentity;

	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_vDir = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_vRight = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_mRot = mIdentity;
	m_qRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	m_vVelocity = { 0.0f, 0.0f, 0.0f };
	m_mTransform = mIdentity;
}


void Transform::UpdateMatrix() {
	XMMATRIX mSca = XMLoadFloat4x4(&m_mSca);
	XMMATRIX mPos = XMLoadFloat4x4(&m_mPos);
	XMMATRIX mRot = XMLoadFloat4x4(&m_mRot);
	XMMATRIX mTrans = mSca;
	mTrans *= mRot;
	mTrans *= mPos;
	XMStoreFloat4x4(&m_mTransform, mTrans);
}

void Transform::Rotate(float yaw, float pitch, float roll) {
	XMVECTOR quatRot;

	XMVECTOR vDir = XMLoadFloat3(&m_vDir);
	XMVECTOR quat = XMQuaternionRotationAxis(vDir, roll);
	quatRot = quat;

	XMVECTOR vRight = XMLoadFloat3(&m_vRight);
	quat = XMQuaternionRotationAxis(vRight, pitch);
	quatRot *= quat;

	XMVECTOR vUp = XMLoadFloat3(&m_vUp);
	quat = XMQuaternionRotationAxis(vUp, yaw);
	quatRot *= quat;

	XMVECTOR vRot = XMLoadFloat4(&m_qRot);
	vRot *= quatRot;
	XMStoreFloat4(&m_qRot, vRot);

	XMMATRIX matRot = XMMatrixRotationQuaternion(quatRot);
	XMStoreFloat4x4(&m_mRot, matRot);

	// r = row, m128_f32 = collumn
	m_vRight.x = matRot.r[0].m128_f32[0];
	m_vRight.y = matRot.r[0].m128_f32[1];
	m_vRight.z = matRot.r[0].m128_f32[2];

	m_vUp.x = matRot.r[1].m128_f32[0];
	m_vUp.y = matRot.r[1].m128_f32[1];
	m_vUp.z = matRot.r[1].m128_f32[2];

	m_vRight.x = m_mRot._21;
	m_vRight.y = m_mRot._22;
	m_vRight.z = m_mRot._23;

	m_vUp.x = m_mRot._31;
	m_vUp.y = m_mRot._32;
	m_vUp.z = m_mRot._33;
	UpdateMatrix();
}

void Transform::Walk(float speed, float deltaTime) {
	m_vPos.x += m_vDir.x * speed * deltaTime;
	m_vPos.y += m_vDir.y * speed * deltaTime;
	m_vPos.z += m_vDir.z * speed * deltaTime;

	XMMATRIX matrix = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMStoreFloat4x4(&m_mPos, matrix);
	UpdateMatrix();


	//XMVECTOR Dir = GetDir();
	//XMVECTOR Pos = GetPos();
	//XMFLOAT3 targetPos;

	//// add d to position
	//XMStoreFloat3(&targetPos, Pos + Dir * speed * deltaTime);
	//SetPos(targetPos);
	//OutputDebugStringA("\n");
	//OutputDebugStringA(std::to_string(targetPos.z).c_str());
	//UpdateMatrix();
}


void Transform::Translation(float x, float y, float z)
{
	m_vPos.x += x;
	m_vPos.y += y;
	m_vPos.z += z;

	XMMATRIX matrix = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMStoreFloat4x4(&m_mPos, matrix);
	UpdateMatrix();
}

void Transform::SetPos(float x, float y, float z)
{
	m_vPos.x = x;
	m_vPos.y = y;
	m_vPos.z = z;

	XMMATRIX matrix = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMStoreFloat4x4(&m_mPos, matrix);
	UpdateMatrix();
}

void Transform::SetPos(const XMFLOAT3& v) {
	m_vPos = v;

	XMMATRIX matrix = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMStoreFloat4x4(&m_mPos, matrix);
	UpdateMatrix();
}

XMVECTOR Transform::GetPos() {
	return XMLoadFloat3(&m_vPos);
}

XMVECTOR Transform::GetDir() {
	return XMLoadFloat3(&m_vDir);
}

XMVECTOR Transform::GetRight() {
	return XMLoadFloat3(&m_vRight);
}

XMVECTOR Transform::GetUp() {
	return XMLoadFloat3(&m_vUp);
}

void Transform::Scale(float x, float y, float z)
{
	m_vSca.x *= x;
	m_vSca.y *= y;
	m_vSca.z *= z;

	XMMATRIX matrix = XMMatrixScaling(m_vSca.x, m_vSca.y, m_vSca.z);
	XMStoreFloat4x4(&m_mSca, matrix);
	UpdateMatrix();
}

void Transform::SetScale(float x, float y, float z)
{
	m_vSca.x = x;
	m_vSca.y = y;
	m_vSca.z = z;

	XMMATRIX matrix = XMMatrixScaling(m_vSca.x, m_vSca.y, m_vSca.z);
	XMStoreFloat4x4(&m_mSca, matrix);
	UpdateMatrix();
}

void Transform::SetVelocity(float x, float y, float z) {
	m_vVelocity = { x,y,z };
}

void Transform::AddVelocity(float x, float y, float z) {
	m_vVelocity = { m_vVelocity.x + x, m_vVelocity.y + y, m_vVelocity.z + z };
}

void Transform::ApplyVelocity(float deltaTime) {
	float speed = 1;
	m_vPos.x += m_vVelocity.x * speed * deltaTime;
	m_vPos.y += m_vVelocity.x * speed * deltaTime;
	m_vPos.z += m_vVelocity.x * speed * deltaTime;

	XMMATRIX matrix = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMStoreFloat4x4(&m_mPos, matrix);
	UpdateMatrix();
}

XMFLOAT4X4 Transform::GetMatrix()
{
	return m_mTransform;
}
