#include "Missile.h"

Missile::Missile(Entity* pEntity) : Script(pEntity) {
	m_eMissile = pEntity;
}

void Missile::OnLoad()
{
	// cr�er une droite qui part devant le joueur
	// check distance des ennemis � la droite
	// lock l'ennemi le plus proche
}

void Missile::Update(float dt) {
	
}

Missile::~Missile()
{
}