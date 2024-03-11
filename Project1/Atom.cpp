#include "pch.h"
#include "Atom.h"
#include <string>
#include "CustomMath.h"


Atom::Atom(string sColor, MeshCreator* mc, XMFLOAT3 pos, ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap, int minLife, int maxLife, int minScale, int maxScale, int minSpeed, int maxSpeed) {
    life = Math::Rand(minLife,maxLife); //2000 - 3000
    lifeMax = life;

    rotate = XMFLOAT3(XMConvertToRadians(Math::Rand(0, 360)), 0.0, XMConvertToRadians(Math::Rand(0, 360)));
    float randomScale = Math::Rand(minScale, maxScale) * 0.0005; // 10 - 80
    scale = XMFLOAT3(randomScale, randomScale , randomScale);
    scaleMax = scale;
    velocity = Math::Rand(minSpeed, maxSpeed) * 0.00001 ;// 10 - 200
    m_oEntity = new Entity(md3dDevice, mCommandList, mCbvHeap);
    m_oEntity->CreatePlane(sColor, mc);
    m_oEntity->SetPosition(pos.x, pos.y, pos.z);
    m_oEntity->SetScale(scale.x,scale.y,scale.z);
    m_oEntity->SetRotate(rotate.x, rotate.y, rotate.z);
}

void Atom::Update(float deltaTime) {


    // Decrease particle life
    life -= deltaTime;
    scale.x = (life * scaleMax.x) / lifeMax ;
    scale.y = (life * scaleMax.y) / lifeMax ;
    scale.z = (life * scaleMax.z) / lifeMax ;
    m_oEntity->SetScale(scale.x, scale.y, scale.z);
    m_oEntity->SetDirection(velocity, deltaTime);
}

