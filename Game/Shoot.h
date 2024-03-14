#pragma once
#include "../Project1/StonEngine.h"
class Shoot : public Script
{
public:
	Shoot(Entity* pEntity, std::vector<Entity*>* vecPtr);
	~Shoot();
	int seekerLockRadius = 50;
	std::vector<Entity*> m_vMissiles;
	std::vector<Entity*> m_vGun;
	std::vector<Entity*>* ShipsRef;
	vector<Entity*>& Ships = *ShipsRef; // vector is not copied here

	std::vector<int> m_vGunLife;
	std::vector<int> m_vMissilesLife;

	int m_iGunDelay;
	int m_iMissilesDelay;

	void Update(float dt) override;
	void OnLoad() override;
	bool IsTargetInFront(const XMVECTOR targetPos);
};

