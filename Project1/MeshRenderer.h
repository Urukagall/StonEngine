#pragma once
#include "pch.h"
#include "Init.h"
#include "UploadBuffer.h"
#include "Component.h"
#include "Mesh.h"
#include <array>
#include "Tools.h"

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
	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
	MeshRenderer(Entity* pEntity);
	~MeshRenderer();
	void Box();
	void BuildConstantBuffers();
	void Update();
};

