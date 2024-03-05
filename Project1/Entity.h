#pragma once
#include "pch.h"
#include "Init.h"
#include "UploadBuffer.h"
#include <iostream>
#include <map>
#include <string>
#include "Transform.h"
#include "MeshRenderer.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class Component;

class Entity
{
public: 
	std::map<std::string, Component*> m_mComponents;
	Transform m_mTransform;

	ComPtr<ID3D12Device> md3dDevice;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

	MeshRenderer* m_oMeshRenderer = nullptr;

	void CreateCube();
	Entity(ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap);
	~Entity();


};

