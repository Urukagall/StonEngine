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
	void Scale(float x, float y, float z);
	void SetPosition(float x, float y, float z);
	void Walk(float velocity, float deltaTime);
	XMFLOAT4X4 GetMatrix();


};

