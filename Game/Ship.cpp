#include "Ship.h"

Ship::Ship(Entity* pEntity) : Script(pEntity) {
	m_eCamera = m_oEntity->m_pRender->CreateEntity(m_oEntity->m_pRender->camera.getPosition3f().x, m_oEntity->m_pRender->camera.getPosition3f().y, m_oEntity->m_pRender->camera.getPosition3f().z);
}

void Ship::OnLoad()
{	
	for (int i = 0; i < 100; i++) {

		XMFLOAT3 pos = { static_cast<float>(Math::Rand(-100, 100)) , static_cast<float>(Math::Rand(-100, 100)), static_cast<float>(Math::Rand(-100, 100)) };

		while ( (sqrt( pow( pos.x + m_oEntity->m_pRender->camera.getPosition3f().x, 2) + pow(pos.y + m_oEntity->m_pRender->camera.getPosition3f().y, 2) + pow(pos.z + m_oEntity->m_pRender->camera.getPosition3f().z, 2))) <10){
			pos = { static_cast<float>(Math::Rand(-100, 100))  , static_cast<float>(Math::Rand(-100, 100))  , static_cast<float>(Math::Rand(-100, 100)) };
		}

		XMFLOAT3 rotate = XMFLOAT3(XMConvertToRadians(Math::Rand(0, 360)), 0.0, XMConvertToRadians(Math::Rand(0, 360)));
		Entity* enemy = m_oEntity->m_pRender->CreateEntityEnemy(pos.x, pos.y, pos.z);
		enemy->SetRotate(rotate.x, rotate.y, rotate.z);
		enemy->m_collider->SetSize(2.0);
		enemy->m_mTransform.VelocityWalk(0.01f);
		enemy->SetRotate(XMConvertToRadians(270), 0.0, XMConvertToRadians(90));
		enemy->m_mTransform.SetDeceleration(0.0f);
		m_eEntity.push_back(enemy);
	}

}

void Ship::Update(float dt) {
	m_eCamera->m_mTransform = *m_oEntity->m_pRender->camera.m_transform;
	for (int i = 0; i < m_eEntity.size(); i++) {
		if (m_eEntity[i]->m_collider->CheckColl(m_eCamera)) {
			PostQuitMessage(0);
		}
		else if ( fabs(m_eEntity[i]->m_mTransform.GetPosFloat().x) > 120  || fabs(m_eEntity[i]->m_mTransform.GetPosFloat().y) > 120 || fabs(m_eEntity[i]->m_mTransform.GetPosFloat().z) > 120) {
			m_eEntity[i]->m_mTransform.m_fSpeedMultiplier = -m_eEntity[i]->m_mTransform.m_fSpeedMultiplier;
			m_eEntity[i]->SetRotate(XMConvertToRadians(180), 0.0, 0.0);
		}
		m_eEntity[i]->m_mTransform.ApplyVelocity(dt);
	}
}

std::vector<Entity*>* Ship::GetShips() {
	return &m_eEntity;
}

Ship::~Ship()
{
}