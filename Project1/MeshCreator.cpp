#include "pch.h"
#include "MeshCreator.h"

MeshCreator::MeshCreator(ComPtr<ID3D12GraphicsCommandList> mCommandList, ComPtr<ID3D12Device> md3dDevice)
{
	this->mCommandList = mCommandList;
	this->md3dDevice = md3dDevice;
}

MeshCreator::~MeshCreator()
{
}

void MeshCreator::Init()
{
	m_mMesh["plane_red"] = Plane(XMFLOAT4(Colors::Red));
	m_mMesh["plane_orange"] = Plane(XMFLOAT4(Colors::Orange));
	m_mMesh["plane_yellow"] = Plane(XMFLOAT4(Colors::Yellow));
	m_mMesh["plane_blue"] = Plane(XMFLOAT4(Colors::Blue));
	m_mMesh["plane_purple"] = Plane(XMFLOAT4(Colors::Purple));

	m_mMesh["box_red"] = Box(XMFLOAT4(Colors::Red));
	m_mMesh["box_orange"] = Box(XMFLOAT4(Colors::Orange));
	m_mMesh["box_yellow"] = Box(XMFLOAT4(Colors::Yellow));
	m_mMesh["box_blue"] = Box(XMFLOAT4(Colors::Blue));
	m_mMesh["box_purple"] = Box(XMFLOAT4(Colors::Purple));

	m_mMesh["pyramid_red"] = Pyramid(XMFLOAT4(Colors::Red));
	m_mMesh["pyramid_orange"] = Pyramid(XMFLOAT4(Colors::Orange));
	m_mMesh["pyramid_yellow"] = Pyramid(XMFLOAT4(Colors::Yellow));
	m_mMesh["pyramid_blue"] = Pyramid(XMFLOAT4(Colors::Blue));
	m_mMesh["pyramid_purple"] = Pyramid(XMFLOAT4(Colors::Purple));

	m_mMesh["missiles"] = Missiles();

}


Mesh* MeshCreator::Plane(XMFLOAT4 oColor) {
	Mesh* mBoxGeo = nullptr;
	std::array<Vertex, 4> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), oColor })
	};

	std::array<std::uint16_t, 6> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	mBoxGeo = new Mesh();
	mBoxGeo->Name = "boxGeo";

	//ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
	//CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
	//
	//ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
	//CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	mBoxGeo->VertexBufferGPU = Tools::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = Tools::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(Vertex);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;

	Submesh submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mBoxGeo->DrawArgs["plane"] = submesh;

	return mBoxGeo;
}

Mesh* MeshCreator::Box(XMFLOAT4 oColor) {
	Mesh* mBoxGeo = nullptr;
	std::array<Vertex, 8> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), oColor }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), oColor }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), oColor }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), oColor })
	};

	std::array<std::uint16_t, 36> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	mBoxGeo = new Mesh();
	mBoxGeo->Name = "boxGeo";

	//ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
	//CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
	//
	//ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
	//CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	mBoxGeo->VertexBufferGPU = Tools::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = Tools::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(Vertex);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;

	Submesh submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mBoxGeo->DrawArgs["box"] = submesh;

	return mBoxGeo;
}

Mesh* MeshCreator::Pyramid(XMFLOAT4 oColor) {
	Mesh* mBoxGeo = nullptr;
	std::array<Vertex, 5> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, 0.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(-1.0f, 0.0f, 1.0f), oColor }),
		Vertex({ XMFLOAT3(1.0f, 0.0f, 1.0f), oColor }),
		Vertex({ XMFLOAT3(1.0f, 0.0f, -1.0f), oColor }),
		Vertex({ XMFLOAT3(0.0f, 1.0f, 0.0f), oColor }),

	};

	std::array<std::uint16_t, 18> indices =
	{
		// front face
		0, 2, 1,
		0, 3, 2,

		// back face (ordre antihoraire)
		0, 1, 4,
		0, 4, 3,

		// left face (ordre antihoraire)
		2, 4, 1,
		2, 3, 4,


	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	mBoxGeo = new Mesh();
	mBoxGeo->Name = "boxGeo";

	/*ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
	CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
	CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);*/

	mBoxGeo->VertexBufferGPU = Tools::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = Tools::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(Vertex);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;

	Submesh submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mBoxGeo->DrawArgs["box"] = submesh;

	return mBoxGeo;
}

Mesh* MeshCreator::Missiles() {
	Mesh* mBoxGeo = nullptr;
	std::array<Vertex, 21> vertices =
	{
		//top
		Vertex({ XMFLOAT3(0.0f, 3.0f, 0.0f), XMFLOAT4(Colors::Red) }),


		Vertex({ XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT4(Colors::Gray) }), //A
		Vertex({ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Gray) }), //B

		Vertex({ XMFLOAT3(0.0f, 1.0f, -1.0f), XMFLOAT4(Colors::Gray) }), //C
		Vertex({ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Gray) }), //D

		Vertex({ XMFLOAT3(0.0f, -6.0f, 1.0f), XMFLOAT4(Colors::Gray) }), //A
		Vertex({ XMFLOAT3(1.0f, -6.0f, 0.0f), XMFLOAT4(Colors::Gray) }), //B

		Vertex({ XMFLOAT3(0.0f, -6.0f, -1.0f), XMFLOAT4(Colors::Gray) }), //C
		Vertex({ XMFLOAT3(-1.0f, -6.0f, 0.0f), XMFLOAT4(Colors::Gray) }), //D

		Vertex({ XMFLOAT3(0.0f, -5.5f, 0.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(0.0f, -3.5f, 0.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(2.0f, -5.5f, 2.0f), XMFLOAT4(Colors::Red) }),

		Vertex({ XMFLOAT3(0.0f, -5.5f, 0.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(0.0f, -3.5f, 0.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(-2.0f, -5.5f, 2.0f), XMFLOAT4(Colors::Red) }),

		Vertex({ XMFLOAT3(0.0f, -5.5f, 0.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(0.0f, -3.5f, 0.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(-2.0f, -5.5f, -2.0f), XMFLOAT4(Colors::Red) }),

		Vertex({ XMFLOAT3(0.0f, -5.5f, 0.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(0.0f, -3.5f, 0.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(2.0f, -5.5f, -2.0f), XMFLOAT4(Colors::Red) }),

	};
	std::array<std::uint16_t, 66> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face (ordre antihoraire)
		0, 3, 4,
		0, 4, 1,

		1, 5, 2,
		2, 5, 6,

		2, 6, 3,
		3, 6, 7,

		3, 7, 4,
		4, 7, 8,

		4, 8, 5,
		1, 4, 5,

		5, 7, 6,
		5, 8, 7,

		9, 10, 11,
		9, 11, 10,

		12, 13, 14,
		12, 14, 13,

		15, 16, 17,
		15, 17, 16,

		18, 19, 20,
		18, 20, 19
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	mBoxGeo = new Mesh();
	mBoxGeo->Name = "boxGeo";


	mBoxGeo->VertexBufferGPU = Tools::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = Tools::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(Vertex);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;

	Submesh submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mBoxGeo->DrawArgs["box"] = submesh;

	return mBoxGeo;
}