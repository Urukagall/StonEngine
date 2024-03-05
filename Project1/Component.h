#pragma once
#include "pch.h"
#include "UploadBuffer.h"
#include "Mesh.h"
class Transform;
class Entity;

using namespace DirectX;


struct ObjectConstants
{
	XMFLOAT4X4 WorldViewProj = Math::Identity4x4();
};

class Component
{
public:
	Mesh* mBoxGeo = nullptr;
	Entity* m_oEntity = nullptr;

	//MeshRenderer::Update();

	Component(Entity* pEntity);
	~Component();

};

