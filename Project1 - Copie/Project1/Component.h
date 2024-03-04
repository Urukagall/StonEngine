#pragma once
#include "Mesh.h"
class Transform;
class Entity;


class Component
{
public:
	Mesh* mBoxGeo = nullptr;
	Entity* m_oEntity = nullptr;

	Component(Entity* pEntity);
	~Component();

};

