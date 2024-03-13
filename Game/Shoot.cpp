#include "Shoot.h"

Shoot::Shoot(Entity* pEntity) : Script(pEntity) {

}

void Shoot::OnLoad()
{
}

void Shoot::Update(float dt) {
	Input* input;

	input = m_oEntity->m_pRender->GetInput();
	if (input->getKeyDown(ARROW_RIGHT)) {
		OutputDebugStringA("Left\n");
		XMFLOAT3 pos;

		//XMStoreFloat3(&pos, m_oEntity->m_mTransform.GetPos());
		XMStoreFloat3(&pos, m_oEntity->m_pRender->camera.m_transform->GetPos());

		float x = pos.x;
		float y = pos.y;
		float z = pos.z;
		OutputDebugStringA(std::to_string(x).c_str());
		OutputDebugStringA(std::to_string(y).c_str());
		OutputDebugStringA(std::to_string(z).c_str());

		XMFLOAT4X4 rot = m_oEntity->m_pRender->camera.m_transform->GetRotate();

		Entity* pEntity = m_oEntity->m_pRender->CreateEntityMissiles(x, y, z);
		pEntity->m_mTransform.SetRot(rot);
		pEntity->m_mTransform.Rotate(0, 0, XMConvertToRadians(90));
		m_vMissiles.push_back(pEntity);
	}

	for (Entity* eEntity : m_vMissiles)
	{
		//eEntity->m_mTransform.Walk(1, dt);
	}
}

Shoot::~Shoot()
{
}