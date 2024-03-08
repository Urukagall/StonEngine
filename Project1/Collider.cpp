#include "pch.h"
#include "Collider.h"

Collider::Collider(Entity * entity) {
	m_entity = entity;
	FindChunk();
}

Collider::~Collider() {
}

DirectX::XMINT3 Collider::FindChunk() {
	int chunkX = m_entity->m_mTransform.m_vPos.x / 10;
	int chunkY = m_entity->m_mTransform.m_vPos.y / 10;
	int chunkZ = m_entity->m_mTransform.m_vPos.z / 10;
	Chunk = { chunkX, chunkY, chunkZ };
	OutputDebugStringA("\nChunk: ");
	OutputDebugStringA(std::to_string(chunkX).c_str());
	OutputDebugStringA(", ");
	OutputDebugStringA(std::to_string(chunkY).c_str());
	OutputDebugStringA(", ");
	OutputDebugStringA(std::to_string(chunkZ).c_str());
	return Chunk;
}