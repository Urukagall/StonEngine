#include "Script.h"

Script::Script(Render* theApp)
{
	this->theApp = theApp;
    Update();
}

Script::~Script()
{
}

void Script::Update()
{
    theApp->CreateParticlesExplosion(3.0, 3.0, 3.0);
    theApp->CreateEntityCube(2.0, 2.0, 2.0, "blue");
    theApp->CreateEntityMissiles(3.0, 3.0, 3.0);
    theApp->CreateEntityEnemy(5.0, 5.0, 5.0);
}