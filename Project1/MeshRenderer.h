#pragma once
#include "Component.h"
#include "pch.h"
#include "Mesh.h"
#include <array>
#include "Tools.h"
#include "Init.h"
#include "UploadBuffer.h"
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
	MeshRenderer(Entity* pEntity);
	~MeshRenderer();

	void Box();
	void BuildDescriptorHeaps();
	void BuildConstantBuffers();
private:
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;
	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;

	ComPtr<ID3D12Resource> mUploadBuffer;
	BYTE* mMappedData = nullptr;
};

