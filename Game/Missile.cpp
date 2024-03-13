#include "Missile.h"

Missile::Missile(Entity* pEntity) : Script(pEntity) {
	m_eMissile = pEntity;
}

void Missile::OnLoad()
{
	// créer une droite qui part devant le joueur
	// check distance des ennemis à la droite
	// lock l'ennemi le plus proche
}

void Missile::Update(float dt) {
	
}

Missile::~Missile()
{
}