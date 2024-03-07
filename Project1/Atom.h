#pragma once
#include "pch.h"
#include "Entity.h"
class Atom
{
public:
    DirectX::XMFLOAT3 rotate;
    DirectX::XMFLOAT3 scale;
    float life;
    Entity* m_oEntity;

    Atom( float startLife, ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap);
    void Update(float deltaTime);
};

