#pragma once
#include "pch.h"
#include "UploadBuffer.h"
#include <iostream>
#include <map>
#include "Component.h"
#include <string>
#include "Transform.h"

using Microsoft::WRL::ComPtr;


class Entity
{
public: 
	std::map<std::string, Component*> m_mComponents;
	Transform m_mTransform;

	ComPtr<ID3D12Device> md3dDevice;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;

	void CreateCube();
	Entity(ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB);
	~Entity();


};

