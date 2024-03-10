#pragma once
#include "Entity.h"
class Collider
{
private:
	Entity * m_entity;
	//XMINT3 Chunk;
public:
	Collider();
	Collider(Entity * entity);
	~Collider();
	void BindEntity(Entity* entity);
	// Check collision between two entities
	bool CheckColl(Entity* b);
	// Will be usefull for complex collisions with bounding boxes
	//DirectX::XMINT3 FindChunk();
};