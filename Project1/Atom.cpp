#include "pch.h"
#include "Atom.h"
#include <string>
#include "Math.h"


Atom::Atom(XMFLOAT4 oColor, ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap) {
    life = Math::Rand(2000,3000);
    lifeMax = life;

    rotate = XMFLOAT3(XMConvertToRadians(Math::Rand(0, 360)), 0.0, XMConvertToRadians(Math::Rand(0, 360)));
    float randomScale = Math::Rand(10,80) * 0.0005;
    scale = XMFLOAT3(randomScale, randomScale , randomScale);
    scaleMax = scale;
    velocity = Math::Rand(10,200) * 0.00001 ;
    m_oEntity = new Entity(md3dDevice, mCommandList, mCbvHeap);
    m_oEntity->CreateCube(oColor);
    m_oEntity->m_mTransform.Translation(1.0f, 2.0f, 2.0f);
    m_oEntity->m_mTransform.Scale(scale.x,scale.y,scale.z);
    m_oEntity->m_mTransform.Rotate(rotate.x, rotate.y, rotate.z);
}

void Atom::Update(float deltaTime) {


    // Decrease particle life
    life -= deltaTime;
    scale.x = (life * scaleMax.x) / lifeMax ;
    scale.y = (life * scaleMax.y) / lifeMax ;
    scale.z = (life * scaleMax.z) / lifeMax ;
    m_oEntity->m_mTransform.SetScale(scale.x, scale.y, scale.z);
    m_oEntity->m_mTransform.Walk(velocity, deltaTime);
}

