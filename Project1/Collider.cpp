#include "pch.h"
#include "Collider.h"

Collider::Collider() {
}

Collider::Collider(Entity * entity) {
	m_entity = entity;
	//FindChunk();
}

Collider::~Collider() {
}

void Collider::BindEntity(Entity* entity) {
	m_entity = entity;
}

void Collider::SetSize(float size) {
	m_size = size;
}

void Collider::SetScale(float scale) {
	m_scale = scale;
}

float Collider::GetSize() {
	return m_size * m_scale;
}

//DirectX::XMINT3 Collider::FindChunk() {
//	int chunkX = m_entity->m_mTransform.m_vPos.x / 10;
//	int chunkY = m_entity->m_mTransform.m_vPos.y / 10;
//	int chunkZ = m_entity->m_mTransform.m_vPos.z / 10;
//	Chunk = { chunkX, chunkY, chunkZ };
//	OutputDebugStringA("\nChunk: ");
//	OutputDebugStringA(std::to_string(chunkX).c_str());
//	OutputDebugStringA(", ");
//	OutputDebugStringA(std::to_string(chunkY).c_str());
//	OutputDebugStringA(", ");
//	OutputDebugStringA(std::to_string(chunkZ).c_str());
//	return Chunk;
//}

bool Collider::CheckColl(Entity* b) {
	Entity* a = m_entity;

	float collisionDistance = GetSize() + b->m_collider->GetSize();


	XMVECTOR dist = a->m_mTransform.GetPos() - b->m_mTransform.GetPos();
	XMFLOAT4 distF; //the float where we copy the up vector members
	XMStoreFloat4(&distF, dist); //the function used to copy




	if (fabs(distF.x) < collisionDistance && fabs(distF.y) < collisionDistance && fabs(distF.z) < collisionDistance) {
		return true;
	}
	return false;
}