#pragma once
#include "MeshRenderer.h"
#include "pch.h"

class Particles : public MeshRenderer
{
public:
	float m_fVelocity = 2.0f;
	float m_fLife = 2.0f;
	float m_fSpawn = 1.0f;
	Particles(Entity* pEntity);


};

