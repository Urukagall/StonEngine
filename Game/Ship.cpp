#include "Ship.h"

Ship::Ship(Entity* pEntity) : Script(pEntity) {
}

void Ship::OnLoad()
{	
	for (int i = 0; i < 20; i++) {
		float x = Math::Rand(5, 50);
		float y = Math::Rand(5, 50);
		float z = Math::Rand(5, 50);
		m_eEntity.push_back(m_oEntity->m_pRender->CreateEntityEnemy(x, y, z));
	}

}

void Ship::Update(float dt) {


}

Ship::~Ship()
{
}