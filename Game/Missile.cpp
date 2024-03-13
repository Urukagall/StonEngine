#include "Missile.h"

Missile::Missile(Entity* pEntity) : Script(pEntity) {
	m_eMissile = pEntity;
}

void Missile::OnLoad()
{
	
}

void Missile::Update(float dt) {
	
}

Missile::~Missile()
{
}