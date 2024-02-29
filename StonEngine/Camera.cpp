#include "pch.h"
#include "Camera.h"

using namespace DirectX;

#pragma region getMethods
XMVECTOR Camera::getPosition()const {
	return XMLoadFloat3(&m_mPosition);
}

XMFLOAT3 Camera::getPosition3f()const {
	return m_mPosition;
}

float Camera::getFovX()const {
	float halfWidth = 0.5f * getNearWindowWidth();
	return 2.0f * atan(halfWidth / m_fNearZ);
}

float Camera::getNearWindowWidth()const {
	return m_fAspect * m_fNearWindowHeight;
}

float Camera::getNearWindowHeight()const {
	return m_fNearWindowHeight;
}

float Camera::getFarWindowWidth()const {
	return m_fAspect * m_fFarWindowHeight;
}

float Camera::getFarWindowHeight()const {
	return m_fFarWindowHeight;
}
#pragma endregion

#pragma region setMethods
void Camera::setPosition(float x, float y, float z) {
	m_mPosition = XMFLOAT3(x, y, z);
}

void Camera::setPosition(const XMFLOAT3& v) {
	m_mPosition = v;
}

void Camera::setLens(float fovY, float aspect, float zn, float zf) {
	m_fFovY = fovY;
	m_fAspect = aspect;
	m_fNearZ = zn;
	m_fFarZ = zf;

	m_fNearWindowHeight = 2.0f * m_fNearZ * tanf(0.5f * m_fFovY);
	m_fFarWindowHeight = 2.0f * m_fFarZ * tanf(0.5f * m_fFovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(m_fFovY, m_fAspect, m_fNearZ, m_fFarZ);
	XMStoreFloat4x4(&m_mProj, P);
}
#pragma endregion

#pragma region Other methods
void Camera::Walk(float d) {
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&m_mLook);
	XMVECTOR p = XMLoadFloat3(&m_mPosition);
	XMStoreFloat3(&m_mPosition, XMVectorMultiplyAdd(s, l, p));
}

void Camera::Strafe(float d) {
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&m_mRight);
	XMVECTOR p = XMLoadFloat3(&m_mPosition);
	XMStoreFloat3(&m_mPosition, XMVectorMultiplyAdd(s, r, p));
}

void Camera::Pitch(float angle) {
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_mRight), angle);

	XMStoreFloat3(&m_mUp, XMVector3TransformNormal(XMLoadFloat3(&m_mUp), R));
	XMStoreFloat3(&m_mLook, XMVector3TransformNormal(XMLoadFloat3(&m_mLook), R));
}

void Camera::Yaw(float angle) {
	XMMATRIX U = XMMatrixRotationAxis(XMLoadFloat3(&m_mUp), angle);

	XMStoreFloat3(&m_mRight, XMVector3TransformNormal(XMLoadFloat3(&m_mRight), U));
	XMStoreFloat3(&m_mLook, XMVector3TransformNormal(XMLoadFloat3(&m_mLook), U));
}

void Camera::Roll(float angle) {
	XMMATRIX L = XMMatrixRotationAxis(XMLoadFloat3(&m_mLook), angle);

	XMStoreFloat3(&m_mRight, XMVector3TransformNormal(XMLoadFloat3(&m_mRight), L));
	XMStoreFloat3(&m_mUp, XMVector3TransformNormal(XMLoadFloat3(&m_mUp), L));
}

void Camera::RotateY(float angle) {
	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&m_mRight, XMVector3Transform(XMLoadFloat3(&m_mRight), R));
	XMStoreFloat3(&m_mUp, XMVector3TransformNormal(XMLoadFloat3(&m_mUp), R));
	XMStoreFloat3(&m_mLook, XMVector3TransformNormal(XMLoadFloat3(&m_mLook), R));
}

void Camera::updateViewMatrix() {
	if (m_bViewDirty) {
		XMVECTOR R = XMLoadFloat3(&m_mRight);
		XMVECTOR U = XMLoadFloat3(&m_mUp);
		XMVECTOR L = XMLoadFloat3(&m_mLook);
		XMVECTOR P = XMLoadFloat3(&m_mPosition);

		L = XMVector3Normalize(L);
		U = XMVector3Normalize(XMVector3Cross(L, R));

		R = XMVector3Cross(U, L);

		float x = XMVectorGetX(XMVector3Dot(P, R));
		float y = XMVectorGetX(XMVector3Dot(P, U));
		float z = XMVectorGetX(XMVector3Dot(P, L));

		XMStoreFloat3(&m_mRight, R);
		XMStoreFloat3(&m_mUp, U);
		XMStoreFloat3(&m_mLook, L);
		m_mView(0, 0) = m_mRight.x;
		m_mView(1, 0) = m_mRight.y;
		m_mView(2, 0) = m_mRight.z;
		m_mView(3, 0) = x;

		m_mView(0, 3) = 0.0f;
		m_mView(1, 3) = 0.0f;
		m_mView(2, 3) = 0.0f;
		m_mView(3, 3) = 1.0f;

		m_bViewDirty = false;
	}
}
#pragma endregion