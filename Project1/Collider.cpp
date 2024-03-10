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
	OutputDebugStringA("\ncollisionDistance: ");
	OutputDebugStringA(std::to_string(collisionDistance).c_str());

	XMVECTOR dist = a->m_mTransform.GetPos() - b->m_mTransform.GetPos();
	XMFLOAT4 distF; //the float where we copy the up vector members
	XMStoreFloat4(&distF, dist); //the function used to copy

	/*OutputDebugStringA("\nx: ");
	OutputDebugStringA(std::to_string(distF.x).c_str());
	OutputDebugStringA("y: ");
	OutputDebugStringA(std::to_string(distF.y).c_str());
	OutputDebugStringA("z: ");
	OutputDebugStringA(std::to_string(distF.z).c_str());*/

	if (distF.x < collisionDistance || distF.y < collisionDistance || distF.z < collisionDistance) {
		return true;
	}
	return false;
}