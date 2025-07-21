#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(ComPtr<ID3D12Device14> pDevice)
	: m_pd3dDevice { pDevice }
{
	CreateCommandList();
	CreateFence();

	m_pConstantBufferPool = std::make_shared<ConstantBufferPool<MAX_CB_POOL_SIZE>>(pDevice, AlignConstantBuffersize(sizeof(XMFLOAT4)));
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::ResetCommandList()
{
	HRESULT hr;
	hr = m_pd3dCommandAllocator->Reset();
	hr = m_pd3dCommandList->Reset(m_pd3dCommandAllocator.Get(), NULL);
	if (FAILED(hr)) {
		SHOW_ERROR("Faied to reset CommandList");
		__debugbreak();
	}
}


void ResourceManager::CreateCommandList()
{
	HRESULT hr{};

	// Create Command Queue
	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc{};
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	{
		d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	}
	hr = m_pd3dDevice->CreateCommandQueue(&d3dCommandQueueDesc, IID_PPV_ARGS(m_pd3dCommandQueue.GetAddressOf()));
	if (FAILED(hr)) {
		SHOW_ERROR("Failed to create CommandQueue");
	}

	// Create Command Allocator
	hr = m_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_pd3dCommandAllocator.GetAddressOf()));
	if (FAILED(hr)) {
		SHOW_ERROR("Failed to create CommandAllocator");
	}

	// Create Command List
	hr = m_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pd3dCommandAllocator.Get(), NULL, IID_PPV_ARGS(m_pd3dCommandList.GetAddressOf()));
	if (FAILED(hr)) {
		SHOW_ERROR("Failed to create CommandList");
	}

	// Close Command List(default is opened)
	hr = m_pd3dCommandList->Close();
}

void ResourceManager::CreateFence()
{
	HRESULT hr{};

	hr = m_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_pd3dFence.GetAddressOf()));
	if (FAILED(hr)) {
		SHOW_ERROR("Failed to create fence");
	}

	m_hFenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

IndexBuffer ResourceManager::CreateIndexBuffer(std::vector<UINT> Indices)
{
	HRESULT hr;

	ShaderResource Buffer{};
	ComPtr<ID3D12Resource> pUploadBuffer = nullptr;
	UINT nIndices = Indices.size();
	UINT IndexBufferSize = sizeof(UINT) * nIndices;

	hr = Buffer.Create(
		m_pd3dDevice,
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(IndexBufferSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr
	);

	if (FAILED(hr)) {
		__debugbreak();
	}

	if (!Indices.empty()) {
		ResetCommandList();

		hr = m_pd3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(IndexBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(pUploadBuffer.GetAddressOf())
		);


		if (FAILED(hr)) {
			__debugbreak();
		}


		UINT8* pIndexDataBegin = nullptr;
		CD3DX12_RANGE readRange(0, 0);
		pUploadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin));
		{
			memcpy(pIndexDataBegin, Indices.data(), IndexBufferSize);
		}
		pUploadBuffer->Unmap(0, nullptr);


		Buffer.StateTransition(m_pd3dCommandList, D3D12_RESOURCE_STATE_COPY_DEST);
		{
			m_pd3dCommandList->CopyBufferRegion(Buffer.pResource.Get(), 0, pUploadBuffer.Get(), 0, IndexBufferSize);
		}
		Buffer.StateTransition(m_pd3dCommandList, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		ExcuteCommandList();
	}

	D3D12_INDEX_BUFFER_VIEW IndexBufferView;
	IndexBufferView.BufferLocation = Buffer.GetGPUAddress();
	IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	IndexBufferView.SizeInBytes = IndexBufferSize;

	return { Buffer, nIndices, IndexBufferView };
}

void ResourceManager::ExcuteCommandList()
{
	m_pd3dCommandList->Close();

	ID3D12CommandList* ppCommandLists[] = { m_pd3dCommandList.Get() };
	m_pd3dCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	WaitForGPUComplete();
}

void ResourceManager::WaitForGPUComplete()
{
	UINT64 nFenceValue = ++m_nFenceValue;
	HRESULT hResult = m_pd3dCommandQueue->Signal(m_pd3dFence.Get(), nFenceValue);
	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}