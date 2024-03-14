#include "pch.h"
#include "Particles.h"


Particles::Particles(Render* pRender, string sColor, MeshCreator* mc, int maxParticles, ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap, XMFLOAT3 pos, int minLife, int maxLife, int minScale, int maxScale, int minSpeed, int maxSpeed) {
    
    for (int i = 0; i < maxParticles; i++) {
        particles.push_back(new Atom(pRender, sColor, mc, pos, md3dDevice, mCommandList, mCbvHeap, minLife, maxLife, minScale, maxScale, minSpeed, maxSpeed));
    }
}

Particles::~Particles() {}

void Particles::Update(float deltaTime) {

    for (int i = 0; i < particles.size(); i++) {
        particles[i]->Update(deltaTime);
        if (particles[i]->life <= 0) {
            particles[i]->m_oEntity->DeleteComponent();
            delete(particles[i]);
            particles.erase(particles.begin() + i);
        }
    }
}

