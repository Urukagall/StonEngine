#pragma once
#include "Atom.h"

class Particles
{
public:
    Particles(int maxParticles);
    ~Particles();

    void Update(float deltaTime);

private:
    std::vector<Atom> particles;
};


