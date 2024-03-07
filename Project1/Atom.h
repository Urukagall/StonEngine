#pragma once
#include "pch.h"
#include "Entity.h"
class Atom
{
public:
    DirectX::XMFLOAT3 rotate;
    DirectX::XMFLOAT3 scale;
    DirectX::XMFLOAT3 scaleMax;
    float velocity;
    Entity* m_oEntity;
    float life;
    float lifeMax;

    Atom(XMFLOAT4 oColor, ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap, XMFLOAT3 pos);
    void Update(float deltaTime);
};

