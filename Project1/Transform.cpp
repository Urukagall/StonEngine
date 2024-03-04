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

	m_vDir.x = matRot.r[2].m128_f32[0];
	m_vDir.y = matRot.r[2].m128_f32[1];
	m_vDir.z = matRot.r[2].m128_f32[2];
}