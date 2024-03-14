#include "Missile.h"

Missile::Missile(Entity* pEntity, Entity* pTarget) : Script(pEntity) {
	m_eMissile = pEntity;
	m_eTarget = pTarget;
}

void Missile::OnLoad()
{
	m_eMissile->m_mTransform.SetDeceleration(0.0f);
	//m_eMissile->m_mTransform.VelocityWalk(0.1f);
}

void Missile::Update(float dt) {
	//OutputDebugStringA("\nMissile Update");
	
	m_eMissile->m_mTransform.ApplyVelocity(dt);
	m_oEntity->m_pRender->CreateParticlesFire(m_eMissile->m_mTransform.GetPosFloat().x, m_eMissile->m_mTransform.GetPosFloat().y, m_eMissile->m_mTransform.GetPosFloat().z);


	if (fabs(tgt_pos_previous.x) >= FLT_EPSILON && fabs(tgt_pos_previous.y) >= FLT_EPSILON && fabs(tgt_pos_previous.z) >= FLT_EPSILON) { // Check if prev tgt pos != 0
		if (fabs(msl_pos_previous.x) >= FLT_EPSILON && fabs(msl_pos_previous.y) >= FLT_EPSILON && fabs(msl_pos_previous.z) >= FLT_EPSILON) { // Check if prev tgt pos != 0
			OutputDebugStringA("\nGuidance Update");
			
			// Load previous positions
			XMVECTOR tgt_pos_previousV = XMLoadFloat3(&tgt_pos_previous);
			XMVECTOR msl_pos_previousV = XMLoadFloat3(&msl_pos_previous);

			// Get msl-target distances of previous frame and new frame (Rtm)
			XMVECTOR RTM_old = tgt_pos_previousV - msl_pos_previousV;
			XMVECTOR RTM_new = m_eTarget->m_mTransform.GetPos() - m_eMissile->m_mTransform.GetPos();

			// normalize RTM vectors
			XMVector3Normalize(RTM_new);
			XMVector3Normalize(RTM_old);

			float Nt = 9.8 * dt;
			XMVECTOR LOS_Delta;
			if (XMVector3Length(RTM_old).m128_f32 == 0) {
				LOS_Delta = { 0.0f, 0.0f, 0.0f };
				LOS_Rate = 0.0f;
			}
			else {
				LOS_Delta = (RTM_new - RTM_old);
				LOS_Rate = *XMVector3Length(LOS_Delta).m128_f32;
			}

			// range closing rate
			float Vc = -LOS_Rate;

			// Now, calculate the final lateral acceleration required for our missile to home into our target.
			XMVECTOR latax = RTM_new * N * Vc * LOS_Rate + LOS_Delta * Nt * (0.5 * N); // lateral acceleration
			XMFLOAT3 lataxF;

			// Debug output
			XMStoreFloat3(&lataxF, latax);
			/*OutputDebugStringA("\nLatax: ");
			OutputDebugStringA(std::to_string(-lataxF.x * m_fResponsiveness).c_str());
			OutputDebugStringA(", ");
			OutputDebugStringA(std::to_string(-lataxF.y * m_fResponsiveness).c_str());
			OutputDebugStringA(", ");
			OutputDebugStringA(std::to_string(-lataxF.z * m_fResponsiveness).c_str());*/
			m_eMissile->m_mTransform.SetVelocity(-lataxF.x * m_fResponsiveness, -lataxF.y * m_fResponsiveness, -lataxF.z * m_fResponsiveness);
		}
	}
	// Update position of objects so we can integrate forward to next frame.
	XMStoreFloat3(&msl_pos_previous, m_eMissile->m_mTransform.GetPos());
	XMStoreFloat3(&tgt_pos_previous, m_eTarget->m_mTransform.GetPos());
}

Missile::~Missile()
{
}