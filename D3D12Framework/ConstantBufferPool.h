#pragma once
#include "ConstantBuffer.h"

// ================================================================================
// ConstantBufferPool
// - �ϳ��� ID3D12Resource �� �̿��Ͽ� �������� ConstantBuffer �� ����ϱ� ����
// - ID3D12Resource �� ID3D12DescriptorHeap �� ������ ���� (struct ConstantBuffer)
//		- Root DescriptorHandle �� �����Ϸ��� �ٷ� GPU �ּҸ� ���� �� ����
//		- DescriptorHandle Table �� �����Ϸ��� �Ʒ��� ������ ����� ��
//			- D3D12_DESCRIPTOR_HEAP_FLAG_NONE �� �ƴϹǷ� ����� ���� 
//			  D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE �� DescriptorHandle Heap �� 
//			  CopyDescriptorsSimple�� �����ؾ� ��
// - ���߿� �� �ʿ��� �ִ� ũ�⸸ŭ�� Pool ũ�⸦ ��Ƽ� ���
// ================================================================================

class ConstantBufferPool {
public:
	ConstantBufferPool();

	void Initialize(ComPtr<ID3D12Device14> pd3dDevice, UINT CBVSize, size_t nMaxCBVCount);

	template<typename T>
	ConstantBuffer& Allocate();
	void Reset();

private:
	std::vector<ConstantBuffer>		m_CBuffers = {};
	
	ComPtr<ID3D12DescriptorHeap>	m_pCBVHeap = nullptr;
	ComPtr<ID3D12Resource>			m_pResource = nullptr;
	UINT8*							m_pMappedPtr = nullptr;

	UINT	m_nCBVSize = 0;
	UINT	m_nAllocated = 0;
	UINT	m_nMaxCBVCount = 0;

};

template<typename T>
inline ConstantBuffer& ConstantBufferPool::Allocate()
{
	std::div_t sizeDevideByCBufferSize = std::div(ConstantBufferSize<T>::value, 256);
	int nRequired = sizeDevideByCBufferSize.rem > 0 ? sizeDevideByCBufferSize.quot + 1 : sizeDevideByCBufferSize.quot;
	UINT allocIndex = m_nAllocated;
	m_nAllocated += nRequired;
	return m_CBuffers[allocIndex];
}
