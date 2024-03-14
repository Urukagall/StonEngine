#pragma once
#include "../Project1/StonEngine.h"

class Script
{
	public:
	Script(Render* theApp);
	~Script();

	void Update();

	Render* theApp;
};

