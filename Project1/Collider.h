#pragma once
#include "Entity.h"
class Collider
{
private:
	Entity * m_entity;
	float m_size = 1.0f;
	float m_scale = 1.0f;
	//XMINT3 Chunk;
public:
	Collider();
	Collider(Entity * entity);
	~Collider();
	void BindEntity(Entity* entity);
	void SetSize(float size);
	void SetScale(float scale);
	float GetSize();
	// Check collision between two entities
	bool CheckColl(Entity* b);
	// Will be usefull for complex collisions with bounding boxes
	//DirectX::XMINT3 FindChunk();
};