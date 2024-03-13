#include "pch.h"
#include "Texture.h"
#include "DDSTextureLoader.h"
#include "Init.h"


TextureEntity::TextureEntity() {
	md3dDevice = nullptr;
	m_rUploadHeap = nullptr;
	m_dDescriptorHeap = nullptr;
	m_uDescriptorSize = 0;
}

TextureEntity::~TextureEntity() {
	m_tTexture.clear();
	md3dDevice->Release();
	m_dDescriptorHeap->Release();
}

void TextureEntity::Init(ID3D12Device* dDevice) {
	md3dDevice = dDevice;
	m_uDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TextureEntity::LoadTexture(std::string sName, std::wstring sFile) {
	m_sName = sName;
	if (FAILED(DirectX::CreateDDSTextureFromFile12(md3dDevice, Init::GetApp()->GetCommandList(), sFile.c_str(), m_rResource, m_rUploadHeap))) {
		return;
	}

	m_tTexture[m_sName] = std::move(this);
}

bool TextureEntity::BuildDescriptorHeaps(std::string sName, ID3D12DescriptorHeap* dDescriptorHeap) {
	m_dDescriptorHeap = dDescriptorHeap;
	m_dDescriptorHdl = m_dDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	auto texture = m_tTexture[sName]->m_rResource;

	m_dSrcVDesc = {};
	m_dSrcVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_dSrcVDesc.Format = texture->GetDesc().Format;
	m_dSrcVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	m_dSrcVDesc.Texture2D.MostDetailedMip = 0;
	m_dSrcVDesc.Texture2D.MipLevels = texture->GetDesc().MipLevels;
	m_dSrcVDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	md3dDevice->CreateShaderResourceView(texture.Get(), &m_dSrcVDesc, m_dDescriptorHdl);

	return true;
}

void TextureEntity::Offset(std::string sName) {
	auto offset = m_tTexture[sName]->m_rResource;

	m_dDescriptorHdl.Offset(1, m_uDescriptorSize);
	m_dSrcVDesc.Format = offset->GetDesc().Format;
	m_dSrcVDesc.Texture2D.MipLevels = offset->GetDesc().MipLevels;

	md3dDevice->CreateShaderResourceView(offset.Get(), &m_dSrcVDesc, m_dDescriptorHdl);
}
