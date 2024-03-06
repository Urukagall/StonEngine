#pragma once
#include "Atom.h"
#include "Entity.h"

class Particles
{
public:
    Particles(int maxParticles, Entity* pEntity);
    ~Particles();

    void Update(float deltaTime);

    std::vector<Atom> particles;
    Entity* m_oParticles;
};


