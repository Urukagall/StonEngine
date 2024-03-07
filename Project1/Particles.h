#pragma once
#include "Atom.h"

class Particles
{
public:
    Particles(int maxParticles, ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap);
    ~Particles();

    void Update(float deltaTime);

    std::vector<Atom*> particles;
   
};


