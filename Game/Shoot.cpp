#include "Shoot.h"
#include "Gun.h"

Shoot::Shoot(Entity* pEntity) : Script(pEntity) {

}

void Shoot::OnLoad()
{
}

void Shoot::Update(float dt) {
	Input* input;

	input = m_oEntity->m_pRender->GetInput();
	if (input->getKey(ARROW_LEFT)) {
		XMFLOAT3 pos;

		//XMStoreFloat3(&pos, m_oEntity->m_mTransform.GetPos());
		XMStoreFloat3(&pos, m_oEntity->m_pRender->camera.m_transform->GetPos());

		float x = pos.x;
		float y = pos.y;
		float z = pos.z;

		XMFLOAT4X4 rot = m_oEntity->m_pRender->camera.m_transform->GetRotate();

		Entity* pEntity = m_oEntity->m_pRender->CreateEntityCube(x, y, z, "yellow");

		Gun* pGun = new Gun(pEntity);
		pEntity->CreateScript(pGun);

		Transform newTransform = *m_oEntity->m_pRender->camera.m_transform;
		newTransform.Scale(0.1f, 0.1f, 0.5f);
		pEntity->m_mTransform = newTransform;
		pEntity->m_mTransform.VelocityWalk(0.5f);
		pEntity->m_mTransform.SetDeceleration(0.0f);
		m_vGun.push_back(pEntity);
	}

	for (int i = 0; i < m_vGun.size(); i++)
	{
		m_vGun[i]->m_mTransform.ApplyVelocity(dt);
		
		m_vGun[i]->DeleteComponent("cube");
		delete(m_vGun[i]);
		m_vGun.erase(m_vGun.begin() + i);
	}
}

Shoot::~Shoot()
{
}