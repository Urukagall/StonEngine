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

	quatRot = XMQuaternionRotationAxis(XMLoadFloat3(&m_vDir), roll);
	

	quatRot = XMQuaternionMultiply(quatRot, XMQuaternionRotationAxis(XMLoadFloat3(&m_vRight), pitch));

	quatRot = XMQuaternionMultiply(quatRot, XMQuaternionRotationAxis(XMLoadFloat3(&m_vUp), yaw));

	XMVECTOR vRot = XMLoadFloat4(&m_qRot);
	vRot = XMQuaternionMultiply(vRot, quatRot);
	XMStoreFloat4(&m_qRot, vRot);

	XMMATRIX matRot = XMMatrixRotationQuaternion(vRot);  // Correction de cette ligne
	XMStoreFloat4x4(&m_mRot, matRot);


	m_vDir.x = m_mRot._11;
	m_vDir.y = m_mRot._12;
	m_vDir.z = m_mRot._13;

	m_vRight.x = m_mRot._21;
	m_vRight.y = m_mRot._22;
	m_vRight.z = m_mRot._23;

	m_vUp.x = m_mRot._31;
	m_vUp.y = m_mRot._32;
	m_vUp.z = m_mRot._33;
	UpdateMatrix();
}

void Transform::Walk(float velocity, float deltaTime) {
	m_vPos.x += m_vDir.x * velocity * deltaTime;
	m_vPos.y += m_vDir.y * velocity * deltaTime;
	m_vPos.z += m_vDir.z * velocity * deltaTime;

	XMMATRIX matrix = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMStoreFloat4x4(&m_mPos, matrix);
	UpdateMatrix();
}


void Transform::Translation(float x, float y, float z)
{
	m_vPos.x *= x;
	m_vPos.y *= y;
	m_vPos.z *= z;

	XMMATRIX matrix = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMStoreFloat4x4(&m_mPos, matrix);
	UpdateMatrix();
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

void Transform::SetPosition(float x, float y, float z) {

	m_vPos.x += x;
	m_vPos.y += y;
	m_vPos.z += z;

	XMMATRIX matrix = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMStoreFloat4x4(&m_mPos, matrix);
	UpdateMatrix();
}

XMFLOAT4X4 Transform::GetMatrix()
{
	return m_mTransform;
}
