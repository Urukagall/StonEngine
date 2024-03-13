#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <map>

class TextureEntity {
protected:
    std::map<std::string, Microsoft::WRL::ComPtr<ID3D12Resource>> m_tTexture;
    ID3D12Device* md3dDevice;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_rResource;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_rUploadHeap;

    CD3DX12_CPU_DESCRIPTOR_HANDLE m_dDescriptorHdlCPU;
    CD3DX12_GPU_DESCRIPTOR_HANDLE m_dDescriptorHdlGPU;
    D3D12_SHADER_RESOURCE_VIEW_DESC m_dSrcVDesc;
    UINT m_uDescriptorSize;
    ID3D12DescriptorHeap* m_dDescriptorHeap;

public:
    TextureEntity();
    ~TextureEntity();

    void Init(ID3D12Device* dDevice);
    void LoadTexture(std::string sName, std::wstring sFile, ID3D12DescriptorHeap* dDescriptorHeap);
    bool BuildSrvDesc(ID3D12DescriptorHeap* dDescriptorHeap, int iSize);
    //bool BuildDescriptorHeaps(std::string sName, ID3D12DescriptorHeap* dDescriptorHeap);
    //void Offset(std::string sName);

    ID3D12DescriptorHeap* getDescriptorHeap() { return m_dDescriptorHeap; }
    UINT* getDescriptorSize() { return &m_uDescriptorSize; }
    D3D12_CPU_DESCRIPTOR_HANDLE* getDescriptorHandle() { return &m_dDescriptorHdlCPU; }
    D3D12_GPU_DESCRIPTOR_HANDLE* getDescriptorHandleGPU() { return &m_dDescriptorHdlGPU; }
    std::map<std::string, Microsoft::WRL::ComPtr<ID3D12Resource>>* getTexture() { return &m_tTexture; };
};

