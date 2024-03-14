#include "Shoot.h"
#include "Missile.h"

Shoot::Shoot(Entity* pEntity) : Script(pEntity) {
	m_iGunDelay = 300;
}

void Shoot::OnLoad()
{
	
}

void Shoot::Update(float dt) {
	Input* input;
	
	m_iGunDelay -= dt;

	// GUN
	input = m_oEntity->m_pRender->GetInput();
	if (input->getKey(ARROW_LEFT) && m_iGunDelay <= 0) {
		XMFLOAT3 pos;
		m_iGunDelay = 100;

		//XMStoreFloat3(&pos, m_oEntity->m_mTransform.GetPos());
		XMStoreFloat3(&pos, m_oEntity->m_pRender->camera.m_transform->GetPos());

		float x = pos.x;
		float y = pos.y;
		float z = pos.z;

		XMFLOAT4X4 rot = m_oEntity->m_pRender->camera.m_transform->GetRotate();

		Entity* pEntity = m_oEntity->m_pRender->CreateEntityCube(x, y, z, "yellow");

		Transform newTransform = *m_oEntity->m_pRender->camera.m_transform;
		newTransform.Scale(0.5f, 0.1f, 0.1f);
		pEntity->m_mTransform = newTransform;
		pEntity->m_mTransform.VelocityWalk(0.5f);
		pEntity->m_mTransform.SetDeceleration(0.0f);
		m_vGun.push_back(pEntity);
		m_vGunLife.push_back(1000);
	}

	// MISSILE
	if (input->getKeyDown(ARROW_RIGHT)) {

		// créer une droite qui part devant le joueur
		// check distance des ennemis à la droite
		// lock l'ennemi le plus proche
		FXMVECTOR point1 = m_oEntity->m_mTransform.GetDir();
		FXMVECTOR point2 = point1 * 2;

		XMVector3LinePointDistance(point1, point2, point1);

		XMFLOAT3 pos;

		//XMStoreFloat3(&pos, m_oEntity->m_mTransform.GetPos());
		XMStoreFloat3(&pos, m_oEntity->m_pRender->camera.m_transform->GetPos());

		XMFLOAT4X4 rot = m_oEntity->m_pRender->camera.m_transform->GetRotate();

		Entity* pEntity = m_oEntity->m_pRender->CreateEntityMissiles(pos.x, pos.y, pos.z);
		Transform newTransform = *m_oEntity->m_pRender->camera.m_transform;
		newTransform.Rotate(XMConvertToRadians(-90.0f), 0.0f, 0.0f);
		pEntity->m_mTransform = newTransform;

		Missile* missileScript = new Missile(pEntity);
		pEntity->CreateScript(missileScript);



		m_vMissiles.push_back(pEntity);
	}

	for (int i = 0; i < m_vGun.size(); i++)
	{
		m_vGun.at(i)->m_mTransform.ApplyVelocity(dt);
		m_vGunLife.at(i) -= dt;
		if (m_vGunLife.at(i) <= 0)
		{
			//m_vGun.at(i)->DeleteComponent("cube");
			//delete(m_vGun.at(i));
			m_vGun.at(i)->Dead();
			m_vGun.erase(m_vGun.begin() + i);
			m_vGunLife.erase(m_vGunLife.begin() + i);
		}
		
	}
}

Shoot::~Shoot()
{
}