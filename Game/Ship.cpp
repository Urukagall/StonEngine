#include "Ship.h"

Ship::Ship(Entity* pEntity) : Script(pEntity) {
	m_eCamera = m_oEntity->m_pRender->CreateEntity(m_oEntity->m_pRender->camera.getPosition3f().x, m_oEntity->m_pRender->camera.getPosition3f().y, m_oEntity->m_pRender->camera.getPosition3f().z);
}

void Ship::OnLoad()
{	
	for (int i = 0; i < 100; i++) {
		CreateShip();

	}

}

void Ship::CreateShip() {
	XMFLOAT3 pos = { static_cast<float>(Math::Rand(-500, 500)) , static_cast<float>(Math::Rand(-500, 500)), static_cast<float>(Math::Rand(-500, 500)) };

	while ((sqrt(pow(pos.x + m_oEntity->m_pRender->camera.getPosition3f().x, 2) + pow(pos.y + m_oEntity->m_pRender->camera.getPosition3f().y, 2) + pow(pos.z + m_oEntity->m_pRender->camera.getPosition3f().z, 2))) < 50) {
		pos = { static_cast<float>(Math::Rand(-100, 100))  , static_cast<float>(Math::Rand(-100, 100))  , static_cast<float>(Math::Rand(-100, 100)) };
	}

	XMFLOAT3 rotate = XMFLOAT3(XMConvertToRadians(Math::Rand(0, 360)), 0.0, XMConvertToRadians(Math::Rand(0, 360)));
	Entity* enemy = m_oEntity->m_pRender->CreateEntityEnemy(pos.x, pos.y, pos.z);
	enemy->SetRotate(rotate.x, rotate.y, rotate.z);
	enemy->m_collider->SetSize(1.5);
	enemy->m_mTransform.VelocityWalk(0.05f);
	enemy->SetRotate(XMConvertToRadians(270), 0.0, XMConvertToRadians(90));
	enemy->m_mTransform.SetDeceleration(0.0f);
	enemy->SetScale(5.0, 5.0, 5.0);
	m_eEntity.push_back(enemy);
}

void Ship::Update(float dt) {
	m_eCamera->m_mTransform = *m_oEntity->m_pRender->camera.m_transform;
	if (m_eEntity.size() == 0) {
		PostQuitMessage(0);
	}
	for (int i = 0; i < m_eEntity.size(); i++) {
		if (m_eEntity[i]->m_collider->CheckColl(m_eCamera)) {
			PostQuitMessage(0);
		}
		else if ( fabs(m_eEntity[i]->m_mTransform.GetPosFloat().x) >= 520  || fabs(m_eEntity[i]->m_mTransform.GetPosFloat().y) >= 520 || fabs(m_eEntity[i]->m_mTransform.GetPosFloat().z) >= 520) {
			m_eEntity[i]->m_mTransform.m_fSpeedMultiplier = -m_eEntity[i]->m_mTransform.m_fSpeedMultiplier;
			m_eEntity[i]->SetRotate(XMConvertToRadians(90), 0.0, 0.0);
			m_eEntity[i]->m_mTransform.ApplyVelocity(dt);
		}
		else {
			m_eEntity[i]->m_mTransform.ApplyVelocity(dt);
			for (int j = 0; j < m_vGun->size(); j++) {
				if (m_eEntity[i]->m_collider->CheckColl(m_vGun->at(j))) {
					m_oEntity->m_pRender->CreateParticlesExplosion(m_eEntity[i]->m_mTransform.GetPosFloat().x, m_eEntity[i]->m_mTransform.GetPosFloat().y, m_eEntity[i]->m_mTransform.GetPosFloat().z);
					m_eEntity[i]->Dead();
					m_eEntity.erase(m_eEntity.begin() + i);
					m_vGun->at(j)->Dead();
					m_vGun->erase(m_vGun->begin() + j);
					
				}
			}
			for (int j = 0; j < m_vMissiles->size(); j++) {
				if (m_eEntity[i]->m_collider->CheckColl(m_vMissiles->at(j))) {
					m_oEntity->m_pRender->CreateParticlesExplosion(m_eEntity[i]->m_mTransform.GetPosFloat().x, m_eEntity[i]->m_mTransform.GetPosFloat().y, m_eEntity[i]->m_mTransform.GetPosFloat().z);
					m_eEntity[i]->Dead();
					m_eEntity.erase(m_eEntity.begin() + i);
					m_vMissiles->at(j)->Dead();
					m_vMissiles->erase(m_vMissiles->begin() + j);

				}
			}

		}

	}
}

std::vector<Entity*>* Ship::GetShips() {
	return &m_eEntity;
}

Ship::~Ship()
{
}