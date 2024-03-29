#include "pch.h"
#include "Init.h"
#include "CustomMath.h"
#include "UploadBuffer.h"
#include <vector>
#include "Entity.h"
#include "MeshRenderer.h"
#include "Timer.h"
#include "Particles.h"
#include "MeshCreator.h"
#include "Camera.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

class Render : public Init
{
public:
    Render(HINSTANCE hInstance);
    Render(const Render& rhs) = delete;
    Render& operator=(const Render& rhs) = delete;
    ~Render();

    virtual bool Initialize()override;
    Entity* CreateEntity(float x, float y, float z);
    Entity* CreateEntityCube(float x, float y, float z, string sColor);
    Entity* CreateEntityMissiles(float x, float y, float z);
    Entity* CreateEntityPyramid(float x, float y, float z, string sColor);
    Entity* CreateEntityEnemy(float x, float y, float z);
    void CreateParticle(float x, float y, float z, string sColor, int minLife, int maxLife, int minScale, int maxScale, int minSpeed, int maxSpeed, int particleNumber);
    void CreateParticlesExplosion(float x, float y, float z);
    void CreateParticlesFire(float x, float y, float z);
    Input* GetInput();

    Camera camera;
    MeshCreator* mc = nullptr;
    std::vector<Entity*> m_Entities;
private:
    virtual void OnResize()override;
    virtual void Update(Timer& gt)override;
    virtual void Draw(const Timer& gt)override;

    void HandleInput(Timer& gt);
    void UpdateCameraPosition();

    void BuildDescriptorHeaps();
    //void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildPSO();



private:

    float moveSpeed = 50.0f;
    float cameraX = 0.0f;
    float cameraY = 0.0f;
    float cameraZ = 0.0f;

    std::vector<MeshGeometry*> m_vEntities;

    std::vector<Particles*> m_Particles;
    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

    std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;

    MeshGeometry* mBoxGeo = nullptr;

    ComPtr<ID3DBlob> mvsByteCode = nullptr;
    ComPtr<ID3DBlob> mpsByteCode = nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    ComPtr<ID3D12PipelineState> mPSO = nullptr;

    XMFLOAT4X4 mWorld = Math::Identity4x4();
    XMFLOAT4X4 mView = Math::Identity4x4();
    XMFLOAT4X4 mProj = Math::Identity4x4();

    float mTheta = 1.5f * XM_PI;
    float mPhi = XM_PIDIV4;
    float mRadius = 5.0f;

    float x = mRadius * DirectX::XMScalarSin(mPhi) * DirectX::XMScalarCos(mTheta);
    float z = mRadius * DirectX::XMScalarSin(mPhi) * DirectX::XMScalarSin(mTheta);
    float y = mRadius * DirectX::XMScalarCos(mPhi);

    POINT mLastMousePos;
};
