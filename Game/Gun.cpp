#include "Gun.h"

Gun::Gun(Entity* pEntity) : Script(pEntity)
{
	m_fLifeTime = 5;
}

Gun::~Gun()
{
}

void Gun::OnLoad()
{
}

void Gun::Update(float dt)
{
	m_fLifeTime -= dt;
}