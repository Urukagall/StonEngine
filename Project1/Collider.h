#pragma once
#include "Entity.h"
class Collider
{
private:
	Entity * m_entity;
	XMINT3 Chunk;
public:
	Collider(Entity * entity);
	~Collider();

	DirectX::XMINT3 FindChunk();
};