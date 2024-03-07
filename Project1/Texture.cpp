#include "pch.h"
#include "Texture.h"
#include "ResourceUploadBatch.h"

void Texture::LoadTexture(std::wstring path)
{
    ComPtr<ID3D12Resource> tex;

    ResourceUploadBatch resourceUpload(tex);

    resourceUpload.Begin();

    ThrowIfFailed(
        CreateDDSTextureFromFile(device, resourceUpload, L"texture.dds",
            tex.ReleaseAndGetAddressOf())
    );

    // Upload the resources to the GPU.
    auto uploadResourcesFinished = resourceUpload.End(mCommandList);

    // Wait for the upload thread to terminate
    uploadResourcesFinished.wait();
}

Texture::Texture(ComPtr<ID3D12GraphicsCommandList> mCommandList)
{
    this->mCommandList = mCommandList;
}

Texture::~Texture()
{
}
