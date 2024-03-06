#pragma once
#include "pch.h"
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

	void CreateCube(XMFLOAT4 oColor);
	void CreatePyramid(XMFLOAT4 oColor);

	Entity(ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList);
	~Entity();


};

