#pragma once
#include <windows.h> // for XMVerifyCPUSupport
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

class Transform
{
public:
	//Scaling
	XMFLOAT3 m_vSca;
	XMFLOAT4X4 m_mSca;

	//Position
	XMFLOAT3 m_vPos;
	XMFLOAT4X4 m_mPos;
	XMVECTOR m_vVelocity = { 0, 0, 0 };
	float m_fMaxVelocity = 0.2f;
	// Dampeners strenght
	float m_fDeceleration = 0.0002f; //m_fMaxVelocity * 0.0002f;

	//Rotate
	XMFLOAT3 m_vUp;
	XMFLOAT3 m_vDir;
	XMFLOAT3 m_vRight;
	XMFLOAT4X4 m_mRot;
	XMFLOAT4 m_qRot;

	//Global
	XMFLOAT4X4 m_mTransform;

	Transform();
	void Identity();
	void UpdateMatrix();
	void Rotate(float yaw, float pitch, float roll);
	void Translation(float x, float y, float z);
	void SetPos(float x, float y, float z);
	void SetPos(const XMFLOAT3& v);
	void SetRot(const XMFLOAT4X4& v);
	void SetDeceleration(float speed);
	XMVECTOR GetPos();
	XMVECTOR GetDir();
	XMVECTOR GetRight();
	XMVECTOR GetUp();
	void Scale(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void Walk(float speed, float deltaTime);
	void AddVelocity(float x, float y, float z);
	void SetVelocity(float x, float y, float z);
	void SetVelocity(XMFLOAT3 vector);
	void ApplyVelocity(float deltaTime);
	XMFLOAT4X4 GetMatrix();
	XMFLOAT4X4 GetRotate();
};

