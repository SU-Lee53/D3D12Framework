#include "pch.h"
#include "ConstantBufferPool.h"

ConstantBufferPool::ConstantBufferPool()
{
}

void ConstantBufferPool::Initialize(ComPtr<ID3D12Device14> pd3dDevice, UINT CBVSize, size_t nMaxCBVCount)
{
	HRESULT hr;

	m_nMaxCBVCount = nMaxCBVCount;
	m_nCBVSize = CBVSize;
	UINT ByteWidth = m_nCBVSize * nMaxCBVCount;

	hr = pd3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(ByteWidth),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_pResource.GetAddressOf())
	);

	if (FAILED(hr)) {
		__debugbreak();
	}

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	{
		heapDesc.NumDescriptors = nMaxCBVCount;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
	}

	hr = pd3dDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_pCBVHeap.GetAddressOf()));
	if (FAILED(hr)) {
		__debugbreak();
	}

	CD3DX12_RANGE writeRange(0, 0);
	m_pResource->Map(0, &writeRange, reinterpret_cast<void**>(&m_pMappedPtr));

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = m_pResource->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = m_nCBVSize;

	UINT8* pMappdedPtr = m_pMappedPtr;
	CD3DX12_CPU_DESCRIPTOR_HANDLE CBVHandle(m_pCBVHeap->GetCPUDescriptorHandleForHeapStart());
	UINT DescriptorSize = pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	m_CBuffers.resize(m_nMaxCBVCount);
	//::memset(m_CBuffers.data(), 0, sizeof(ConstantBuffer) * m_nMaxCBVCount);

	for (UINT i = 0; i < nMaxCBVCount; ++i) {
		pd3dDevice->CreateConstantBufferView(&cbvDesc, CBVHandle);

		m_CBuffers[i].CBVHandle = CBVHandle;
		m_CBuffers[i].GPUAddress = cbvDesc.BufferLocation;
		m_CBuffers[i].pMappedPtr = pMappdedPtr;

		CBVHandle.Offset(1, DescriptorSize);
		cbvDesc.BufferLocation += m_nCBVSize;
		pMappdedPtr += m_nCBVSize;
	}
}

void ConstantBufferPool::Reset()
{
	m_nAllocated = 0;
}
