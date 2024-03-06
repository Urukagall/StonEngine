#include "pch.h"
#include "Atom.h"



Atom::Atom(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 startVel, float startLife)
    : position(startPos), velocity(startVel), life(startLife) {}

void Atom::Update(float deltaTime) {
    // Update particle position based on velocity
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    position.z += velocity.z * deltaTime;

    // Decrease particle life
    life -= deltaTime;
}