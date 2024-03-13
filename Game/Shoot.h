#pragma once
#include "../Project1/StonEngine.h"
class Shoot : public Script
{
public:
	Shoot(Entity* pEntity);
	~Shoot();

	std::vector<Entity*> m_vMissiles;
	std::vector<Entity*> m_vGun;

	std::vector<int> m_vGunLife;

	int m_iGunDelay;

	void Update(float dt) override;
	void OnLoad() override;
};

