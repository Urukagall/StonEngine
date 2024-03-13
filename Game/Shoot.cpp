#include "Shoot.h"

Shoot::Shoot(Entity* pEntity) : Script(pEntity) {

}

void Shoot::OnLoad()
{
	
}

void Shoot::Update(float dt) {
	Input* input;

	// GUN
	input = m_oEntity->m_pRender->GetInput();
	if (input->getKeyDown(ARROW_LEFT)) {
		//OutputDebugStringA("Left\n");
		XMFLOAT3 pos;

		//XMStoreFloat3(&pos, m_oEntity->m_mTransform.GetPos());
		XMStoreFloat3(&pos, m_oEntity->m_pRender->camera.m_transform->GetPos());

		XMFLOAT4X4 rot = m_oEntity->m_pRender->camera.m_transform->GetRotate();

		Entity* pEntity = m_oEntity->m_pRender->CreateEntityMissiles(pos.x, pos.y, pos.z);
		Transform newTransform = *m_oEntity->m_pRender->camera.m_transform;
		newTransform.Rotate(XMConvertToRadians(-90.0f),0.0f, 0.0f);
		pEntity->m_mTransform = newTransform;
		m_vMissiles.push_back(pEntity);
	}

	// MISSILE
	input = m_oEntity->m_pRender->GetInput();
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
}

Shoot::~Shoot()
{
}