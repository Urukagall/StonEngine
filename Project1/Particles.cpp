#include "pch.h"
#include "Particles.h"


Particles::Particles(int maxParticles, ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap) {
    float startLife = 3000.0f;
    for (int i = 0; i < maxParticles; i++) {
        particles.push_back(new Atom(startLife, md3dDevice, mCommandList, mCbvHeap));
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

