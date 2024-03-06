#include "pch.h"
#include "Particles.h"


Particles::Particles(int maxParticles, Entity* pEntity) {
    m_oParticles = pEntity;
    float startLife = 2.0f;
    for (int i = 0; i < maxParticles; i++) {
        particles.push_back(Atom(startLife));
        m_oParticles->CreateCube(XMFLOAT4(Colors::Black));
    }
}

Particles::~Particles() {}

void Particles::Update(float deltaTime) {

    for (int i = 0; i < particles.size(); i++) {
        particles[i].Update(deltaTime);
        if (particles[i].life <= 0) {
            particles.erase(particles.begin() + i);
            m_oParticles->m_mComponents.clear();
        }
    }
}

