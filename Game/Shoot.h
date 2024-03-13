#pragma once
#include "../Project1/StonEngine.h"
class Shoot : public Script
{
public:
	Shoot(Entity* pEntity, std::vector<Entity*>* vecPtr);
	~Shoot();
	int seekerLockRadius = 10;
	std::vector<Entity*> m_vMissiles;
	std::vector<Entity*> m_vGun;
	std::vector<Entity*>* ShipsRef;
	vector<Entity*>& Ships = *ShipsRef; // vector is not copied here

	void Update(float dt) override;
	void OnLoad() override;
};

