#pragma once
#include "ConstantBuffer.h"

// ================================================================================
// ConstantBufferPool
// - �ϳ��� ID3D12Resource �� �̿��Ͽ� �������� ConstantBuffer �� ����ϱ� ����
// - ID3D12Resource �� ID3D12DescriptorHeap �� ������ ���� (struct ConstantBuffer)
//		- Root Descriptor �� �����Ϸ��� �ٷ� GPU �ּҸ� ���� �� ����
//		- Descriptor Table �� �����Ϸ��� �Ʒ��� ������ ����� ��
//			- D3D12_DESCRIPTOR_HEAP_FLAG_NONE �� �ƴϹǷ� ����� ���� 
//			  D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE �� Descriptor Heap �� 
//			  CopyDescriptorsSimple�� �����ؾ� ��
// - ���߿� �� �ʿ��� �ִ� ũ�⸸ŭ�� Pool ũ�⸦ ��Ƽ� ���
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