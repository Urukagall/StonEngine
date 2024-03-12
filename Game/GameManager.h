#pragma once
#include "../Project1/StonEngine.h"

class GameManager : public Script
{
public:
	GameManager(Entity* pEntity);
	void OnLoad() override;
	void Update(float dt) override;
	~GameManager();

	Entity* pEntity = nullptr;
};

