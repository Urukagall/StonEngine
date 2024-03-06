#include "pch.h"
#include "Particles.h"


Particles::Particles(int maxParticles) {
    // Seed the random number generator
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Initialize particles with random positions and velocities
    for (int i = 0; i < maxParticles; ++i) {
        DirectX::XMFLOAT3 startPos(
            static_cast<float>(std::rand() % 100 - 50) / 50.0f,
            static_cast<float>(std::rand() % 100 - 50) / 50.0f,
            static_cast<float>(std::rand() % 100 - 50) / 50.0f
        );

        DirectX::XMFLOAT3 startVel(
            static_cast<float>(std::rand() % 100 - 50) / 50.0f,
            static_cast<float>(std::rand() % 100 - 50) / 50.0f,
            static_cast<float>(std::rand() % 100 - 50) / 50.0f
        );

        float startLife = static_cast<float>(std::rand() % 100) / 100.0f;

        particles.emplace_back(startPos, startVel, startLife);
    }
}

Particles::~Particles() {}

void Particles::Update(float deltaTime) {
    // Update all particles
    for (auto& particle : particles) {
        particle.Update(deltaTime);
    }

    // Remove particles with life <= 0
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](const Atom& p) { return p.life <= 0; }),
        particles.end());
}

