#include "pch.h"
#include "Particles.h"


Particles::Particles(XMFLOAT4 oColor,int maxParticles, ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap, XMFLOAT3 pos) {
    
    for (int i = 0; i < maxParticles; i++) {
        particles.push_back(new Atom(oColor,md3dDevice, mCommandList, mCbvHeap, pos));
    }
}

Particles::~Particles() {}

void Particles::Update(float deltaTime) {

    for (int i = 0; i < particles.size(); i++) {
        particles[i]->Update(deltaTime);
        if (particles[i]->life <= 0) {
            particles[i]->m_oEntity->DeleteComponent("cube");
            delete(particles[i]);
            particles.erase(particles.begin() + i);
        }
    }
}

