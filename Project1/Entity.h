#pragma once
#include "pch.h"
#include "Init.h"
#include "UploadBuffer.h"
#include <iostream>
#include <map>
#include <string>
#include "Transform.h"
#include "MeshRenderer.h"
#include "Collider.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class Component;
class Collider;

class Entity
{
public: 
	std::map<std::string, Component*> m_mComponents;
	std::map<std::string, MeshRenderer*> m_oMeshRenderers;
	Transform m_mTransform;
	Collider* m_collider;
	
	ComPtr<ID3D12Device> md3dDevice;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;


	void CreateCube(XMFLOAT4 oColor);
	void CreatePyramid(XMFLOAT4 oColor);
	void SetScale(float x, float y, float z);
	void SetPosition(float x, float y, float z);
	void SetRotate(float yaw, float pitch, float roll);
	void SetDirection(float velocity, float deltaTime);
	bool DeleteComponent(std::string name);
	Entity(ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap);
	~Entity();


};

