#pragma once
#include "../Project1/StonEngine.h"
class Missile : public Script
{
public:
	Missile(Entity* pEntity, Entity* pTarget);
	~Missile();

	std::vector<Entity*> m_vMissiles;

	void Update(float dt) override;
	void OnLoad() override;
private:
	Entity* m_eTarget;
	Entity* m_eMissile;
	float m_fSpeed;
	XMFLOAT3 tgt_pos_previous = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 msl_pos_previous = { 0.0f,0.0f,0.0f };
	float LOS_Rate;
	float N = 5; // Navigation gain
	float timer = 0;
};

