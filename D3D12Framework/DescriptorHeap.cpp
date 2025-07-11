#include "stdafx.h"
#include "DescriptorHeap.h"

DescriptorHeap::DescriptorHeap(ComPtr<ID3D12Device14> pd3dDevice, D3D12_DESCRIPTOR_HEAP_DESC d3dHeapDesc)
{
	HRESULT hr;

	hr = pd3dDevice->CreateDescriptorHeap(&d3dHeapDesc, IID_PPV_ARGS(m_pd3dDescriptorHeap.GetAddressOf()));

	if (FAILED(hr)) {
		__debugbreak();
	}

	m_uiDescriptorSize = pd3dDevice->GetDescriptorHandleIncrementSize(d3dHeapDesc.Type);
	m_uiCurrentDescriptorCount = d3dHeapDesc.NumDescriptors;
	m_uiAllocated = 0;

	m_DescriptorHandleFromStart.cpuHandle = m_pd3dDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	if (d3dHeapDesc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) {
		m_DescriptorHandleFromStart.gpuHandle = m_pd3dDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	}

}

DescriptorHeap::~DescriptorHeap()
{
}
