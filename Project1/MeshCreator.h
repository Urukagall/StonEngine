#pragma once
#include "pch.h"
#include "Init.h"
#include <array>
#include "Tools.h"
#include "Mesh.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Render;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Normal;
	XMFLOAT2 TexC;
};

class MeshCreator
{
	public:
	MeshCreator(ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12Device> md3dDevice);
	~MeshCreator();

	void Init();

	Mesh* Plane(XMFLOAT4 oColor);

	Mesh* Enemy();

	Mesh* Box(XMFLOAT4 oColor);

	Mesh* Pyramid(XMFLOAT4 oColor);

	Mesh* Missiles();



	/*Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;*/

	std::map<std::string, Mesh*> m_mMesh;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12Device> md3dDevice;


};

