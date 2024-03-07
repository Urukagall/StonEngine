#include "pch.h"
#include "Atom.h"
#include <string>
#include "Math.h"


Atom::Atom( float startLife, ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap) {
    life = startLife;



    rotate = XMFLOAT3(XMConvertToRadians(Math::Rand(0, 360)), 0.0, XMConvertToRadians(Math::Rand(0, 360)));
    scale = XMFLOAT3(0.1, 0.1, 0.1);

    m_oEntity = new Entity(md3dDevice, mCommandList, mCbvHeap);
    m_oEntity->CreateCube(DirectX::XMFLOAT4(Colors::Black));

    m_oEntity->m_mTransform.Scale(scale.x,scale.y,scale.z);
    m_oEntity->m_mTransform.Rotate(rotate.x, rotate.y, rotate.z);
}

void Atom::Update(float deltaTime) {


    // Decrease particle life
    life -= deltaTime;
    m_oEntity->m_mTransform.Walk(0.001f, deltaTime);
}

