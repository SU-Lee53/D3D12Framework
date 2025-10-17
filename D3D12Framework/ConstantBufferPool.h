#pragma once
#include "ConstantBuffer.h"

// ================================================================================
// ConstantBufferPool
// - 하나의 ID3D12Resource 를 이용하여 여러개의 ConstantBuffer 를 사용하기 위함
// - ID3D12Resource 와 ID3D12DescriptorHeap 의 쌍으로 구성 (struct ConstantBuffer)
//		- Root Descriptor 로 전달하려면 바로 GPU 주소를 보낼 수 있음
//		- Descriptor Table 로 전달하려면 아래의 절차를 따라야 함
//			- D3D12_DESCRIPTOR_HEAP_FLAG_NONE 이 아니므로 사용을 위해 
//			  D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE 인 Descriptor Heap 에 
//			  CopyDescriptorsSimple를 수행해야 함
// - 나중에 딱 필요한 최대 크기만큼함 Pool 크기를 잡아서 사용
// ================================================================================

template<size_t nMaxCBVCount>
class ConstantBufferPool {
public:
	ConstantBufferPool(ComPtr<ID3D12Device14> pd3dDevice, UINT CBVSize);

	ConstantBuffer& Allocate();
	void Reset();


private:
	std::array<ConstantBuffer, nMaxCBVCount>		m_CBuffers = {};
	
	ComPtr<ID3D12DescriptorHeap>	m_pCBVHeap = nullptr;
	ComPtr<ID3D12Resource>			m_pResource = nullptr;
	UINT8*							m_pMappedPtr = nullptr;

	UINT	m_nCBVSize = 0;
	UINT	m_nAllocated = 0;

};

template<size_t nMaxCBVCount>
inline ConstantBufferPool<nMaxCBVCount>::ConstantBufferPool(ComPtr<ID3D12Device14> pd3dDevice, UINT CBVSize)
{
	HRESULT hr;

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

	for (UINT i = 0; i < nMaxCBVCount; ++i) {
		m_CBuffers[i].CBVHandle = CBVHandle;
		m_CBuffers[i].GPUAddress = cbvDesc.BufferLocation;
		m_CBuffers[i].pMappedPtr = pMappdedPtr;

		CBVHandle.Offset(1, DescriptorSize);
		cbvDesc.BufferLocation += m_nCBVSize;
		pMappdedPtr += m_nCBVSize;
	}
}

template<size_t nMaxCBVCount>
inline ConstantBuffer& ConstantBufferPool<nMaxCBVCount>::Allocate()
{
#ifdef _DEBUG
	assert(m_nAllocated < nMaxCBVCount);
#else
	if (m_nAllocated >= nMaxCBVCount) std::exit(99);
#endif
	
	return m_CBuffers[m_nAllocated++];
}

template<size_t nMaxCBVCount>
inline void ConstantBufferPool<nMaxCBVCount>::Reset()
{
	m_nAllocated = 0;
}