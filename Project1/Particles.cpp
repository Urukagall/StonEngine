#include "pch.h"
#include "Particles.h"


Particles::Particles(XMFLOAT4 oColor,int maxParticles, ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap) {
    
    for (int i = 0; i < maxParticles; i++) {
        particles.push_back(new Atom(oColor,md3dDevice, mCommandList, mCbvHeap));
    }
}

Particles::~Particles() {}

void Particles::Update(float deltaTime) {

    for (int i = 0; i < particles.size(); i++) {
        particles[i]->Update(deltaTime);
        if (particles[i]->life <= 0) {
            delete(particles[i]);
            particles.erase(particles.begin() + i);
        }
    }
}

