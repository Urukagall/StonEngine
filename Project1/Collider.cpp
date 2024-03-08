#include "pch.h"
#include "Collider.h"

Collider::Collider(Entity * entity) {
	m_entity = entity;
}

Collider::~Collider() {
}

DirectX::XMINT3 Collider::FindChunk() {
	int chunkX = m_entity->m_mTransform.m_vPos.x / 10;
	int chunkY = m_entity->m_mTransform.m_vPos.y / 10;
	int chunkZ = m_entity->m_mTransform.m_vPos.z / 10;
	Chunk = { chunkX, chunkY, chunkZ };
}