#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <map>

class TextureEntity {
protected:
    std::string m_sName;
    std::map<std::string, TextureEntity*> m_tTexture;
    ID3D12Device* md3dDevice;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_rResource;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_rUploadHeap;

    CD3DX12_CPU_DESCRIPTOR_HANDLE m_dDescriptorHdl;
    D3D12_SHADER_RESOURCE_VIEW_DESC m_dSrcVDesc;
    UINT m_uDescriptorSize;
    ID3D12DescriptorHeap* m_dDescriptorHeap;

public:
    TextureEntity();
    ~TextureEntity();

    void Init(ID3D12Device* dDevice);
    void LoadTexture(std::string sName, std::wstring sFile);
    bool BuildDescriptorHeaps(std::string sName, ID3D12DescriptorHeap* dDescriptorHeap);
    void Offset(std::string sName);

    ID3D12DescriptorHeap* getDescriptorHeap() { return m_dDescriptorHeap; }
    UINT* getDescriptorSize() { return &m_uDescriptorSize; }
    D3D12_CPU_DESCRIPTOR_HANDLE* getDescriptorHandle() { return &m_dDescriptorHdl; }
};
