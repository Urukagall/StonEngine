#include "pch.h"
#include "Camera.h"

using namespace DirectX;

Camera::Camera() {}

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

XMMATRIX Camera::getRotationMatrix() const {
	XMVECTOR rightVec = XMLoadFloat3(&m_mRight);
	XMVECTOR upVec = XMLoadFloat3(&m_mUp);
	XMVECTOR lookVec = XMLoadFloat3(&m_mLook);

	return XMMATRIX(rightVec, upVec, lookVec, XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
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

XMFLOAT4X4 Camera::getView4x4f()const {
	return m_mView;
}

XMMATRIX Camera::getView()const {
	return m_mCameraView;
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

	XMMATRIX Persp = XMMatrixPerspectiveFovLH(m_fFovY, m_fAspect, m_fNearZ, m_fFarZ);
	XMStoreFloat4x4(&m_mProj, Persp);
}

void Camera::setView() {
	XMVECTOR target = getPosition() + getLook();
	XMFLOAT4X4 camView = getView4x4f();
	m_mCameraView = XMMatrixLookAtLH(getPosition(), target, getUp());
	updateViewMatrix();
}
#pragma endregion

#pragma region Other methods
void Camera::Walk(float d) {
	/*
	XMVECTOR Walk = XMVectorReplicate(d);
	XMVECTOR Look = XMLoadFloat3(&m_mLook);
	XMVECTOR Pos = XMLoadFloat3(&m_mPosition);
	XMStoreFloat3(&m_mPosition, XMVectorMultiplyAdd(Walk, Look, Pos
	*/
}

void Camera::Strafe(float d) {
	XMVECTOR Strafe = XMVectorReplicate(d);
	XMVECTOR Right = XMLoadFloat3(&m_mRight);
	XMVECTOR Pos = XMLoadFloat3(&m_mPosition);
	XMStoreFloat3(&m_mPosition, XMVectorMultiplyAdd(Strafe, Right, Pos));
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

void Camera::RotateY(float angle) {
	XMMATRIX Rot = XMMatrixRotationY(angle);

	XMStoreFloat3(&m_mRight, XMVector3Transform(XMLoadFloat3(&m_mRight), Rot));
	XMStoreFloat3(&m_mUp, XMVector3TransformNormal(XMLoadFloat3(&m_mUp), Rot));
	XMStoreFloat3(&m_mLook, XMVector3TransformNormal(XMLoadFloat3(&m_mLook), Rot));
}

void Camera::updateViewMatrix() {
	//OutputDebugStringA("nope");
	//if (m_bViewDirty) {
		//OutputDebugStringA("m_bViewDirty");
		XMVECTOR Right = XMLoadFloat3(&m_mRight);
		XMVECTOR Up = XMLoadFloat3(&m_mUp);
		XMVECTOR Look = XMLoadFloat3(&m_mLook);
		XMVECTOR Pos = XMLoadFloat3(&m_mPosition);

		Look = XMVector3Normalize(Look);
		Up = XMVector3Normalize(XMVector3Cross(Look, Right));

		Right = XMVector3Cross(Up, Look);

		float x = XMVectorGetX(XMVector3Dot(Pos, Right));
		float y = XMVectorGetX(XMVector3Dot(Pos, Up));
		float z = XMVectorGetX(XMVector3Dot(Pos, Look));

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

		/*m_bViewDirty = false;
	}*/
}

/*
void Camera::Move(float dx, float dy, float dz) {
	// D�place la cam�ra dans la direction sp�cifi�e par les valeurs dx, dy et dz
	// dx : d�placement horizontal (gauche/droite)
	// dy : d�placement vertical (haut/bas)
	// dz : d�placement avant/arri�re

	// Obtenez les vecteurs de base de la cam�ra
	DirectX::XMVECTOR right = XMLoadFloat3(&m_mRight);
	DirectX::XMVECTOR up = XMLoadFloat3(&m_mUp);
	DirectX::XMVECTOR look = XMLoadFloat3(&m_mLook);
	DirectX::XMVECTOR position = XMLoadFloat3(&m_mPosition);

	// Calcule le d�placement total
	DirectX::XMVECTOR movement = dx * right + dy * up + dz * look;

	// Met � jour la position de la cam�ra
	position += movement;
	XMStoreFloat3(&m_mPosition, position);

	// Marque la vue comme �tant sale pour que la matrice de vue soit mise � jour lors de l'appel � updateViewMatrix()
	m_bViewDirty = true;
}
*/

#pragma endregion

Camera::~Camera(){}