#pragma once
#include "Atom.h"
#include "MeshCreator.h"

class Particles
{
public:
    Particles(Render* pRender, string sColor, MeshCreator* mc,  int maxParticles, ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap, XMFLOAT3 pos, int minLife = 6000, int maxLife = 10000, int minScale = 10, int maxScale = 80, int minSpeed = 10, int maxSpeed = 200);
    ~Particles();

    void Update(float deltaTime);

    std::vector<Atom*> particles;
   
};


