#include "Shoot.h"
#include "Missile.h"

Shoot::Shoot(Entity* pEntity, std::vector<Entity*>* vecPtr) : Script(pEntity) {
	ShipsRef = vecPtr;
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
		pEntity->m_mTransform = *m_oEntity->m_pRender->camera.m_transform;
		pEntity->m_mTransform.VelocityWalk(0.5f);
		pEntity->m_mTransform.SetDeceleration(0.0f);
		m_vGun.push_back(pEntity);
		m_vGunLife.push_back(3000);
		//m_oEntity->m_pRender->CreateParticlesFire(pEntity->m_mTransform.GetPosFloat().x, pEntity->m_mTransform.GetPosFloat().y, pEntity->m_mTransform.GetPosFloat().z);
	}

	// MISSILE
	if (input->getKeyDown(ARROW_RIGHT)) {

		// créer une droite qui part devant le joueur
		// check distance des ennemis à la droite
		// lock l'ennemi le plus proche
		FXMVECTOR point1 = m_oEntity->m_pRender->camera.m_transform->GetDir() + m_oEntity->m_pRender->camera.m_transform->GetPos();
		FXMVECTOR point2 = m_oEntity->m_pRender->camera.m_transform->GetDir() * 10 + m_oEntity->m_pRender->camera.m_transform->GetPos();;

		Entity* target = nullptr;
		float shortestLen = 999999999999999;

		for (int i = 0; i < ShipsRef->size(); i++)
		{
			//XMVECTOR point3 = Ships[i]->m_mTransform.GetPos
			XMVECTOR point3 = ShipsRef->at(i)->m_mTransform.GetPos();

			XMVECTOR distV = XMVector3LinePointDistance(point1, point2, point3);
			XMFLOAT3 distF;
			XMStoreFloat3(&distF, distV);

			
			if (distF.x < seekerLockRadius && distF.y < seekerLockRadius && distF.z < seekerLockRadius) {
				// Calculate length (dist) to target
				float len = sqrtf(distF.x * distF.x + distF.y * distF.y + distF.z * distF.z);
				if (len < shortestLen) {

					// Check if target in front or behind
					if (IsTargetInFront(ShipsRef->at(i)->m_mTransform.GetPos())) {
						shortestLen = len;
						target = ShipsRef->at(i);
					}
				}
				//OutputDebugStringA("\nlen: ");
				//OutputDebugStringA(std::to_string(len).c_str());
				//OutputDebugStringA("\nKILL HIIIM ! Put the nose on him and Kill Him ! C'mon he's out in front, Shoot Him, Shoot HIM ! and then the maniacal laughter after that.");
			}
		}

		// Target debug
		if (target != nullptr) {
			//Entity* debugCube = m_oEntity->m_pRender->CreateEntityCube(target->m_mTransform.m_vPos.x, target->m_mTransform.m_vPos.y, target->m_mTransform.m_vPos.z, "purple");
			//debugCube->m_mTransform = target->m_mTransform;


			// CREATE MISSILE

			XMFLOAT3 pos;

			//XMStoreFloat3(&pos, m_oEntity->m_mTransform.GetPos());
			XMStoreFloat3(&pos, m_oEntity->m_pRender->camera.m_transform->GetPos());

			XMFLOAT4X4 rot = m_oEntity->m_pRender->camera.m_transform->GetRotate();

			Entity* pEntity = m_oEntity->m_pRender->CreateEntityMissiles(pos.x, pos.y, pos.z);
			Transform newTransform = *m_oEntity->m_pRender->camera.m_transform;
			newTransform.Rotate(XMConvertToRadians(-90.0f), 0.0f, 0.0f);
			pEntity->m_mTransform = newTransform;

			Missile* missileScript = new Missile(pEntity, target);
			//pEntity->m_mTransform.VelocityWalk(0.5f);
			pEntity->CreateScript(missileScript);

			m_vMissiles.push_back(pEntity);
			m_vMissilesLife.push_back(10000);
		}
	}
	//pour missile
	//m_oEntity->m_pRender->CreateParticlesFire(m_vMissiles.at(i)->m_mTransform.GetPosFloat().x, m_vMissiles.at(i)->m_mTransform.GetPosFloat().y, m_vMissiles.at(i)->m_mTransform.GetPosFloat().z);
	for (int i = 0; i < m_vGun.size(); i++)
	{


		m_vGun.at(i)->m_mTransform.ApplyVelocity(dt);
		
		float dist = (sqrt(pow(m_vGun.at(i)->m_mTransform.GetPosFloat().x + m_oEntity->m_pRender->camera.m_transform->GetPosFloat().x, 2) + pow(m_vGun.at(i)->m_mTransform.GetPosFloat().y + m_oEntity->m_pRender->camera.m_transform->GetPosFloat().y, 2) + pow(m_vGun.at(i)->m_mTransform.GetPosFloat().z + m_oEntity->m_pRender->camera.m_transform->GetPosFloat().z, 2)));
		//OutputDebugStringA("dist : ");
		//OutputDebugStringA(std::to_string(dist).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA(std::to_string(m_oEntity->m_pRender->camera.m_transform->GetPosFloat().x).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA(std::to_string(m_oEntity->m_pRender->camera.m_transform->GetPosFloat().y).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA(std::to_string(m_oEntity->m_pRender->camera.m_transform->GetPosFloat().z).c_str());
		//OutputDebugStringA("\n");
		if ( dist < 5 ) {
			//m_oEntity->m_pRender->CreateParticlesFire(m_vGun.at(i)->m_mTransform.GetPosFloat().x, m_vGun.at(i)->m_mTransform.GetPosFloat().y, m_vGun.at(i)->m_mTransform.GetPosFloat().z);
		}
		m_vGunLife.at(i) -= dt;
		if (m_vGunLife.at(i) <= 0)
		{
			m_vGun.at(i)->Dead();
			m_vGun.erase(m_vGun.begin() + i);
			m_vGunLife.erase(m_vGunLife.begin() + i);
		}
		
	}

	for (int i = 0; i < m_vMissiles.size(); i++)
	{
		m_vMissilesLife.at(i) -= dt;
		if (m_vMissilesLife.at(i) <= 0)
		{
			m_vMissiles.at(i)->Dead();
			m_vMissiles.erase(m_vMissiles.begin() + i);
			m_vMissilesLife.erase(m_vMissilesLife.begin() + i);
		}
	}
}

bool Shoot::IsTargetInFront(const XMVECTOR targetPos) {
	const XMVECTOR playerPos = m_oEntity->m_pRender->camera.m_transform->GetPos();
	XMVECTOR playerFwd = m_oEntity->m_pRender->camera.m_transform->GetDir();
	// Calculate vector from player to target
	XMVECTOR toTarget = XMVectorSubtract(targetPos, playerPos);
	// Normalize the vectors
	XMVECTOR playerFwdNorm = XMVector3Normalize(playerFwd);
	XMVECTOR toTargetNorm = XMVector3Normalize(toTarget);

	// Calculate the dot product to get the angle between vectors
	float dotProduct = XMVectorGetX(XMVector3Dot(playerFwdNorm, toTargetNorm));

	// Convert dot product to angle (in radians)
	float angle = acosf(dotProduct);

	// Convert angle from radians to degrees
	angle = XMConvertToDegrees(angle);

	// DEBUG OUTPUT
	OutputDebugStringA("\nAngle to target: ");
	OutputDebugStringA(std::to_string(angle).c_str());


	// Check if angle is within 90 degrees
	return angle <= 90.0f;
}

Shoot::~Shoot()
{
}