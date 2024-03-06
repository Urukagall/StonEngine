#pragma once
#include "pch.h"
class Atom
{
public:
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 velocity;
    float life;

    Atom( float startLife);
    void Update(float deltaTime);
};

