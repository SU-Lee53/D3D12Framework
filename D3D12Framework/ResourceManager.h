#pragma once
#include "ConstantBufferPool.h"

// ========================
// Input Assembly Resources
// ========================

struct VertexBuffer {
	ShaderResource VertexBuffer;
	UINT nVertices;
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView;
};

struct IndexBuffer {
	ShaderResource IndexBuffer;
	UINT nIndices;
	D3D12_INDEX_BUFFER_VIEW IndexBufferView;
};

// ===========================================================================================
// ResourceManager
// - Shader 변수들을 생성 및 관리
//		- VertexBuffer, IndexBuffer
//		- ConstantBuffer (ConstantBufferPool 소유)
//		- StructuredBuffer	
//		- Texture
// - 직접 생성보다 Manager 를 거쳐 생성할 것 (추후 ResourcePool 을 이용하여 관리할 예정이므로)
// ===========================================================================================

constexpr static size_t MAX_CB_POOL_SIZE = 1024;

class ResourceManager {
public:
	ResourceManager(ComPtr<ID3D12Device14> pDevice);
	~ResourceManager();

public:
	template<typename T>
	VertexBuffer CreateVertexBuffer(std::span<T> vertices);
	IndexBuffer CreateIndexBuffer(std::span<UINT> Indices);

	void ExcuteCommandList();

private:
	void CreateCommandList();
	void CreateFence();
	void WaitForGPUComplete();

private:
	ComPtr<ID3D12Device14>				m_pd3dDevice = nullptr;		// Reference to D3DCore::m_pd3dDevice
	ComPtr<ID3D12GraphicsCommandList>	m_pd3dCommandList = nullptr;
	ComPtr<ID3D12CommandAllocator>		m_pd3dCommandAllocator = nullptr;
	ComPtr<ID3D12CommandQueue>			m_pd3dCommandQueue = nullptr;

	ComPtr<ID3D12Fence>		m_pd3dFence = nullptr;
	HANDLE					m_hFenceEvent = nullptr;
	UINT64					m_nFenceValue = 0;

private:
	std::shared_ptr<ConstantBufferPool<MAX_CB_POOL_SIZE>> m_pConstantBufferPool = nullptr;

};

template<typename T>
inline VertexBuffer ResourceManager::CreateVertexBuffer(std::span<T> vertices)
{
	HRESULT hr;

	ShaderResource Buffer = nullptr;
	ComPtr<ID3D12Resource> pUploadBuffer = nullptr;
	UINT nVertices = vertices.size();
	UINT VertexBufferSize = sizeof(T) * nVertices;

	hr = Buffer.Create(
		m_pd3dDevice,
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(VertexBufferSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr
	);

	if (FAILED(hr)) {
		__debugbreak();
	}

	if (!vertices.empty()) {
		hr = m_pd3dDevice->CreateCommittedResource(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			CD3DX12_RESOURCE_DESC::Buffer(VertexBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(pUploadBuffer.GetAddressOf());
		);


		if (FAILED(hr)) {
			__debugbreak();
		}


		UINT8* pVertexDataBegin = nullptr;
		CD3DX12_RANGE readRange(0, 0);
		pUploadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
		{
			memcpy(pVertexDataBegin, vertices.data(), VertexBufferSize);
		}
		pUploadBuffer->Unmap(0, nullptr);


		Buffer.StateTransition(m_pd3dCommandList, D3D12_RESOURCE_STATE_COPY_DEST);
		{
			m_pd3dCommandList->CopyBufferRegion(Buffer.pResource.Get(), 0, pUploadBuffer.Get(), 0, VertexBufferSize);
		}
		Buffer.StateTransition(m_pd3dCommandList, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		m_pd3dCommandList->Close();

	}

	D3D12_VERTEX_BUFFER_VIEW VertexBufferView;
	VertexBufferView.BufferLocation = Buffer.GetGPUAddress();
	VertexBufferView.StrideInBytes = sizeof(T);
	VertexBufferView.SizeInBytes = VertexBufferSize;

	return { Buffer, nVertices, VertexBufferView };
}
