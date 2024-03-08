#include "pch.h"
#include "Camera.h"

using namespace DirectX;

Camera::Camera() {
	m_transform->SetPos(0.0f, 0.0f, 0.0f);
}

#pragma region getMethods
XMVECTOR Camera::getPosition()const {
	return XMLoadFloat3(&m_mPosition);
}

XMFLOAT3 Camera::getPosition3f()const {
	return m_mPosition;
}

XMVECTOR Camera::getLook()const {
	return XMLoadFloat3(&m_mLook);
}

XMVECTOR Camera::getUp()const {
	return XMLoadFloat3(&m_mUp);
}

XMVECTOR Camera::getRight()const {
	return XMLoadFloat3(&m_mRight);
}

XMMATRIX Camera::getRotationMatrix() const {
	XMVECTOR rightVec = XMLoadFloat3(&m_mRight);
	XMVECTOR upVec = XMLoadFloat3(&m_mUp);
	XMVECTOR lookVec = XMLoadFloat3(&m_mLook);

	return XMMATRIX(rightVec, upVec, lookVec, XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
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

XMFLOAT4X4 Camera::getView4x4f()const {
	return m_mView;
}

XMMATRIX Camera::getView()const {
	return m_mCameraView;
}

#pragma endregion

#pragma region setMethods
void Camera::setPosition(float x, float y, float z) {
	m_transform->SetPos(x, y, z);
	//m_mPosition = XMFLOAT3(x, y, z);
}

void Camera::setPosition(const XMFLOAT3& v) {
	m_transform->SetPos(v);
	//m_mPosition = v;
}

void Camera::setLens(float fovY, float aspect, float zn, float zf) {
	m_fFovY = fovY;
	m_fAspect = aspect;
	m_fNearZ = zn;
	m_fFarZ = zf;

	m_fNearWindowHeight = 2.0f * m_fNearZ * tanf(0.5f * m_fFovY);
	m_fFarWindowHeight = 2.0f * m_fFarZ * tanf(0.5f * m_fFovY);

	XMMATRIX Persp = XMMatrixPerspectiveFovLH(m_fFovY, m_fAspect, m_fNearZ, m_fFarZ);
	XMStoreFloat4x4(&m_mProj, Persp);
}

void Camera::setView() {
	XMVECTOR target = m_transform->GetPos() + m_transform->GetDir();
	//XMVECTOR target = getPosition() + getLook();
	XMFLOAT4X4 camView = getView4x4f();
	m_mCameraView = XMMatrixLookAtLH(getPosition(), target, getUp());
	updateViewMatrix();
}
#pragma endregion

#pragma region Other methods
void Camera::Walk(float d) {
	XMVECTOR Look = getLook();
	XMVECTOR Pos = getPosition();
	XMFLOAT3 targetPos;

	// add d to position
	XMStoreFloat3(&targetPos, Pos + Look * d);
	setPosition(targetPos);
}

void Camera::Strafe(float d) {
	XMVECTOR Right = getRight();
	XMVECTOR Pos = getPosition();
	XMFLOAT3 targetPos;

	// add d to position
	XMStoreFloat3(&targetPos, Pos + Right * d);
	setPosition(targetPos);
}

void Camera::Pitch(float angle) {
	XMMATRIX Rot = XMMatrixRotationAxis(XMLoadFloat3(&m_mRight), angle);

	XMStoreFloat3(&m_mUp, XMVector3TransformNormal(XMLoadFloat3(&m_mUp), Rot));
	XMStoreFloat3(&m_mLook, XMVector3TransformNormal(XMLoadFloat3(&m_mLook), Rot));
}

void Camera::Yaw(float angle) {
	XMMATRIX Rot = XMMatrixRotationAxis(XMLoadFloat3(&m_mUp), angle);

	XMStoreFloat3(&m_mRight, XMVector3TransformNormal(XMLoadFloat3(&m_mRight), Rot));
	XMStoreFloat3(&m_mLook, XMVector3TransformNormal(XMLoadFloat3(&m_mLook), Rot));
}

void Camera::Roll(float angle) {
	XMMATRIX Rot = XMMatrixRotationAxis(XMLoadFloat3(&m_mLook), angle);

	XMStoreFloat3(&m_mRight, XMVector3TransformNormal(XMLoadFloat3(&m_mRight), Rot));
	XMStoreFloat3(&m_mUp, XMVector3TransformNormal(XMLoadFloat3(&m_mUp), Rot));
}

void Camera::updateViewMatrix() {
	/*XMVECTOR Right = XMLoadFloat3(&m_mRight);
	XMVECTOR Up = XMLoadFloat3(&m_mUp);
	XMVECTOR Look = XMLoadFloat3(&m_mLook);
	XMVECTOR Pos = XMLoadFloat3(&m_mPosition);*/
	XMVECTOR Look = m_transform->GetDir();
	XMVECTOR Pos = m_transform->GetPos();
	XMVECTOR Right = m_transform->GetRight();
	XMVECTOR Up = m_transform->GetUp();

	Look = XMVector3Normalize(Look);
	Up = XMVector3Normalize(XMVector3Cross(Look, Right));

	Right = XMVector3Cross(Up, Look);

	/*float x = XMVectorGetX(XMVector3Dot(Pos, Right));
	float y = XMVectorGetX(XMVector3Dot(Pos, Up));
	float z = XMVectorGetX(XMVector3Dot(Pos, Look));*/
	float x = m_transform->m_vPos.x;

	XMStoreFloat3(&m_mRight, Right);
	XMStoreFloat3(&m_mUp, Up);
	XMStoreFloat3(&m_mLook, Look);
	m_mView(0, 0) = m_mRight.x;
	m_mView(1, 0) = m_mRight.y;
	m_mView(2, 0) = m_mRight.z;
	m_mView(3, 0) = x;

	m_mView(0, 3) = 0.0f;
	m_mView(1, 3) = 0.0f;
	m_mView(2, 3) = 0.0f;
	m_mView(3, 3) = 1.0f;
}

#pragma endregion

Camera::~Camera(){}