#include "pch.h"
#include "Init.h"
#include "Math.h"
#include "UploadBuffer.h"
#include <vector>
#include "Entity.h"
#include "Timer.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

struct ObjectConstants
{
    XMFLOAT4X4 WorldViewProj = Math::Identity4x4();
};

class Render : public Init
{
public:
    Render(HINSTANCE hInstance);
    Render(const Render& rhs) = delete;
    Render& operator=(const Render& rhs) = delete;
    ~Render();

    virtual bool Initialize()override;

private:
    virtual void OnResize()override;
    virtual void Update(Timer& gt)override;
    virtual void Draw(const Timer& gt)override;

    //virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
    //virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
    //virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

    void HandleInput(Timer& gt);
    void UpdateCameraPosition();

    void BuildDescriptorHeaps();
    void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildPSO();

    void CreateEntity();

private:

    float moveSpeed = 50.0f;
    float cameraX = 0.0f;
    float cameraY = 0.0f;
    float cameraZ = 0.0f;

    std::vector<MeshGeometry*> m_vEntities;
    std::vector<Entity*> m_Entities;
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
