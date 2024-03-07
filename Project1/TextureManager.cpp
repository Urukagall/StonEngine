#include "pch.h"
#include "TextureManager.h"
#include "ResourceUploadBatch.h"

HRESULT CreateDDSTextureFromFile(
    ID3D12Device* device,
    ResourceUploadBatch& resourceUpload,
    const wchar_t* szFileName,
    ID3D12Resource** texture,
    bool generateMipsIfMissing = false,
    size_t maxsize = 0,
    DDS_ALPHA_MODE* alphaMode = nullptr,
    bool* isCubeMap = nullptr);

void TextureManager::LoadTexture(std::wstring path, ID3D12Device* device, ComPtr<ID3D12CommandQueue> mCommandQueue)
{
    ComPtr<ID3D12Resource> tex;

    ResourceUploadBatch resourceUpload(device);

    resourceUpload.Begin();

    ThrowIfFailed(
        CreateDDSTextureFromFile(device, resourceUpload, L"texture.dds",
            tex.ReleaseAndGetAddressOf())
    );

    // Upload the resources to the GPU.
    auto uploadResourcesFinished = resourceUpload.End(mCommandQueue.Get());

    // Wait for the upload thread to terminate
    uploadResourcesFinished.wait();
}

TextureManager::TextureManager(ComPtr<ID3D12GraphicsCommandList> mCommandList)
{
    this->mCommandList = mCommandList;
}

TextureManager::~TextureManager()
{
}

HRESULT CreateDDSTextureFromFile(ID3D12Device* device, ResourceUploadBatch& resourceUpload, const wchar_t* szFileName, ID3D12Resource** texture, bool generateMipsIfMissing, size_t maxsize, DDS_ALPHA_MODE* alphaMode, bool* isCubeMap)
{
    return E_NOTIMPL;
}
