#pragma once
#include "../Project1/StonEngine.h"
class Shoot : public Script
{
public:
	Shoot(Entity* pEntity);
	~Shoot();

	std::vector<Entity*> m_vMissiles;

	void Update(float dt) override;
	void OnLoad() override;
};

