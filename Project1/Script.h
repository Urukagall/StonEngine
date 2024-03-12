#pragma once
#include "Component.h"


class Script : public Component
{
public:
	Script(Entity* pEntity);
	~Script();
	virtual void Update(float dt);
};

