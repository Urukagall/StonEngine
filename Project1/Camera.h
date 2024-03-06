#pragma once

class Camera {
private:
	DirectX::XMMATRIX				m_mCameraView;
	DirectX::XMFLOAT3				m_mPosition = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3				m_mRight = { 1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3				m_mUp = { 0.0f, 1.0f, 0.0f };
	DirectX::XMFLOAT3				m_mLook = { 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4X4				m_mView = Math::Identity4x4();
	DirectX::XMFLOAT4X4				m_mProj = Math::Identity4x4();
	float							m_fNearZ = 0.0f;
	float							m_fFarZ = 0.0f;
	float							m_fAspect = 0.0f;
	float							m_fFovY = 0.0f;
	float							m_fNearWindowHeight = 0.0f;
	float							m_fFarWindowHeight = 0.0f;
	bool							m_bViewDirty = true;
public:
									Camera(); // Constructor
									~Camera();

#pragma region setMethods
	void							setPosition(float x, float y, float z);
	void							setPosition(const DirectX::XMFLOAT3& v);
	void							setLens(float fovY, float aspect, float zn, float zf);
	void							setView();
#pragma endregion

#pragma region getMethods
	DirectX::XMVECTOR				getPosition()const;
	DirectX::XMVECTOR				getLook()const;
	DirectX::XMVECTOR				getUp()const;
	DirectX::XMMATRIX				getView()const;
	DirectX::XMFLOAT3				getPosition3f()const;
	DirectX::XMMATRIX				getRotationMatrix()const;
	DirectX::XMMATRIX				getProj()const;		// Pas setup

	DirectX::XMFLOAT4X4				getView4x4f()const; // Pas setup
	DirectX::XMFLOAT4X4				getProj4x4f()const; // Pas setup

	float							getNearZ()const;	// A setup en cas de besoin absolu
	float							getFarZ()const;		// Idem
	float							getAspect()const;	// Idem
	float							getFovY()const;		// Idem
	float							getFovX()const;		// Idem

	float							getNearWindowWidth()const;
	float							getNearWindowHeight()const;
	float							getFarWindowWidth()const;
	float							getFarWindowHeight()const;
#pragma endregion

#pragma region Other methods
	void							updateViewMatrix();

	void							Move(float dx, float dy, float dz);

	void							Pitch(float angle);
	void							Yaw(float angle);
	void							Roll(float angle);

	void							RotateX(float angle); // Potentiellement inutile donc non définit
	void							RotateY(float angle); // Défini par défaut
	void							RotateZ(float angle); // Potentiellement inutile donc non définit par défaut

	void							Strafe(float d);
	void							Walk(float d);
#pragma endregion
};