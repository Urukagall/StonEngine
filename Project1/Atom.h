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

    Atom(string sColor, MeshCreator* mc, XMFLOAT3 pos, ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap, int minLife = 6000, int maxLife = 10000, int minScale = 10, int maxScale = 80, int minSpeed = 10, int maxSpeed = 200);
    void Update(float deltaTime);
};

