#pragma once
#include "../Project1/StonEngine.h"
class Missile : public Script
{
public:
	Missile(Entity* pEntity);
	~Missile();

	std::vector<Entity*> m_vMissiles;

	void Update(float dt) override;
	void OnLoad() override;
private:
	Entity* m_eTarget;
	Entity* m_eMissile;
	float m_fSpeed;
};

