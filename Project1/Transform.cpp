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

void Transform::Walk(float speed, float deltaTime) {
	m_vPos.x += m_vDir.x * speed * deltaTime;
	m_vPos.y += m_vDir.y * speed * deltaTime;
	m_vPos.z += m_vDir.z * speed * deltaTime;

	XMMATRIX matrix = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMStoreFloat4x4(&m_mPos, matrix);
	UpdateMatrix();
}

void Transform::VelocityWalk(float speed) {
	m_fSpeedMultiplier = 1.0f;
	// Load velocity
	XMFLOAT3 fVelocity;
	XMStoreFloat3(&fVelocity, m_vVelocity);
	fVelocity.x += m_vDir.x * speed;
	fVelocity.y += m_vDir.y * speed;
	fVelocity.z += m_vDir.z * speed;
	
	// Update pos
	
	SetVelocity(fVelocity);
	//AddVelocity(fVelocity.x, fVelocity.y, fVelocity.z);
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

void Transform::SetRot(const XMFLOAT4X4& v) {
	m_mRot = v;
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

XMVECTOR Transform::GetPos() {
	return XMLoadFloat3(&m_vPos);
}

XMFLOAT3 Transform::GetPosFloat() {
	return m_vPos;
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

XMFLOAT4X4 Transform::GetMatrix()
{
	return m_mTransform;
}

XMFLOAT4X4 Transform::GetRotate()
{
	return m_mRot;
}


// VELOCITY

void Transform::AddVelocity(float x, float y, float z) {
	XMVECTOR input = {x, y, z};
	m_vVelocity += input;
}

void Transform::SetVelocity(float x, float y, float z) {
	XMFLOAT3 input = { x, y, z };
	m_vVelocity = XMLoadFloat3(&input);
}

void Transform::SetVelocity(XMFLOAT3 vector) {
	m_vVelocity = XMLoadFloat3(&vector);
}

void Transform::SetDeceleration(float speed) {
	m_fDeceleration = speed;
}

void Transform::ApplyVelocity(float deltaTime) {
	// Load velocity
	XMFLOAT3 fVelocity;
	XMStoreFloat3(&fVelocity, m_vVelocity);

	// Cap velocity and decelerate
	XMFLOAT3 cappedVelocity = { fVelocity.x * m_fSpeedMultiplier, fVelocity.y * m_fSpeedMultiplier, fVelocity.z * m_fSpeedMultiplier };
	if (fVelocity.x > m_fMaxVelocity) cappedVelocity.x = m_fMaxVelocity;
	if (fVelocity.y > m_fMaxVelocity) cappedVelocity.y = m_fMaxVelocity;
	if (fVelocity.z > m_fMaxVelocity) cappedVelocity.z = m_fMaxVelocity;

	if (fVelocity.x < -m_fMaxVelocity) cappedVelocity.x = -m_fMaxVelocity;
	if (fVelocity.y < -m_fMaxVelocity) cappedVelocity.y = -m_fMaxVelocity;
	if (fVelocity.z < -m_fMaxVelocity) cappedVelocity.z = -m_fMaxVelocity;

	// Decelerate
	/*if (cappedVelocity.x > 0) cappedVelocity.x -= m_fDeceleration;
	if (cappedVelocity.y > 0) cappedVelocity.y -= m_fDeceleration;
	if (cappedVelocity.z > 0) cappedVelocity.z -= m_fDeceleration;

	if (cappedVelocity.x < 0) cappedVelocity.x += m_fDeceleration;
	if (cappedVelocity.y < 0) cappedVelocity.y += m_fDeceleration;
	if (cappedVelocity.z < 0) cappedVelocity.z += m_fDeceleration;*/

	SetVelocity(cappedVelocity);
	m_fSpeedMultiplier -= m_fDeceleration;

	// Reload velocity after speed cap
	XMStoreFloat3(&fVelocity, m_vVelocity);

	/*OutputDebugStringA("\nVelocity: {");
	OutputDebugStringA(std::to_string(fVelocity.x).c_str());
	OutputDebugStringA(", ");
	OutputDebugStringA(std::to_string(fVelocity.y).c_str());
	OutputDebugStringA(", ");
	OutputDebugStringA(std::to_string(fVelocity.z).c_str());
	OutputDebugStringA("}");*/

	m_vPos.x += deltaTime * fVelocity.x;
	m_vPos.y += deltaTime * fVelocity.y;
	m_vPos.z += deltaTime * fVelocity.z;

	XMMATRIX matrix = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMStoreFloat4x4(&m_mPos, matrix);
	UpdateMatrix();
}
