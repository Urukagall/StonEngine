#pragma once
#include "pch.h"
#include "Init.h"
#include "Tools.h"
#include "DirectXHelpers.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class TextureManager
{
	public:
		ComPtr<ID3D12GraphicsCommandList> mCommandList;

		TextureManager(ComPtr<ID3D12GraphicsCommandList> mCommandList);
		~TextureManager();
		void LoadTexture(std::wstring path, ID3D12Device* device, ComPtr<ID3D12CommandQueue> mCommandQueue);


		ComPtr<ID3D12DescriptorHeap> m_resourceDescriptors;
		ComPtr<ID3D12Resource> m_texture;

		enum Descriptors
		{
			Cat,
			Count
		};
};

