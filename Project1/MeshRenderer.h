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

struct ObjectConstants
{
	XMFLOAT4X4 WorldViewProj = Math::Identity4x4();
};

class MeshRenderer : public Component
{
public : 

	ComPtr<ID3D12Device> md3dDevice;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

	MeshRenderer(Entity* pEntity);
	~MeshRenderer();

	void Box(XMFLOAT4 oColor);
	void Pyramid(XMFLOAT4 oColor);
	void BuildConstantBuffers();
	void Update(XMFLOAT4X4 objConstants);
};

