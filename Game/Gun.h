#pragma once
#include "../Project1/StonEngine.h"

class Gun : public Script
{
	public:
	Gun(Entity* pEntity);
	~Gun();

	float m_fLifeTime = 0.0f;

	void Update(float dt) override;
	void OnLoad() override;
};

