#pragma once
#include "Component.h"
#include "pch.h"
#include "Mesh.h"
#include <array>
#include "Tools.h"
#include "Init.h"
using namespace DirectX;
using Microsoft::WRL::ComPtr;
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

class MeshRenderer : public Component
{
public : 

	ComPtr<ID3D12Device> md3dDevice;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	MeshRenderer(Entity* pEntity);
	~MeshRenderer();
	void Box();
};

