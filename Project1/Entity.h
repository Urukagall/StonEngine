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
#include "MeshCreator.h"
#include "Script.h"
using Microsoft::WRL::ComPtr;
using namespace DirectX;


class Component;
class Collider;
class Render;

class Entity
{
public: 
	std::map<std::string, Component*> m_mComponents;
	std::map<std::string, MeshRenderer*> m_oMeshRenderers;
	MeshRenderer* m_oMeshRenderer = nullptr;
	Transform m_mTransform;
	Collider* m_collider;
	Render* m_pRender;

	bool m_bIsAlive;
	
	ComPtr<ID3D12Device> md3dDevice;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

	Script* m_script = nullptr;

	void CreatePlane(string sColor, MeshCreator* mc);
	void Dead();
	void CreateCube(string sColor, MeshCreator* mc);
	void CreateMissiles(MeshCreator* mc);
	void CreatePyramid(string sColor, MeshCreator* mc);
	void CreateEnemy(MeshCreator* mc);
	void SetScale(float x, float y, float z, bool scaleColliderSize=true);
	void SetPosition(float x, float y, float z);
	void SetRotate(float yaw, float pitch, float roll);
	void SetDirection(float velocity, float deltaTime);
	bool DeleteComponent();
	Entity(ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12DescriptorHeap> mCbvHeap, Render* pRender);
	~Entity();

	void CreateScript(Script* pScript);


};

