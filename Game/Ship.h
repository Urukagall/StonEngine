#pragma once
#include "../Project1/StonEngine.h"
class Ship : public Script
{
public:	
	Ship(Entity* pEntity);
	~Ship();
	
	Entity* m_eCamera;
	std::vector<Entity*> m_eEntity;

	void Update(float dt) override;
	void OnLoad() override;
	std::vector<Entity*>* GetShips();
};


