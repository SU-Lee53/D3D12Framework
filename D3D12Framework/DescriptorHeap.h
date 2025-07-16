#pragma once


struct Descriptor {
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle;

	void Bind(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nRootParameterIndex, UINT offset = 0) {
		CD3DX12_GPU_DESCRIPTOR_HANDLE bindGPUHandle;
		pd3dCommandList->SetGraphicsRootDescriptorTable(nRootParameterIndex, CD3DX12_GPU_DESCRIPTOR_HANDLE(gpuHandle, offset, D3DCore::gnCBVSRVDescriptorIncrementSize));
	}

};

class DescriptorHeap {
public:
	DescriptorHeap() = default;
	DescriptorHeap(ComPtr<ID3D12Device14> pd3dDevice, D3D12_DESCRIPTOR_HEAP_DESC d3dHeapDesc);
	~DescriptorHeap();

	Descriptor operator[](UINT index)
	{
		if (index >= m_uiCurrentDescriptorCount) {
			__debugbreak();
		}

		return Descriptor{
			CD3DX12_CPU_DESCRIPTOR_HANDLE(m_DescriptorHandleFromStart.cpuHandle, index, m_uiDescriptorSize),
			CD3DX12_GPU_DESCRIPTOR_HANDLE(m_DescriptorHandleFromStart.gpuHandle, index, m_uiDescriptorSize),
		};
	}

	Descriptor Alloc() {
		return (*this)[++m_uiAllocated];
	}

	void CopySimpleTo(ComPtr<ID3D12Device14> pd3dDevice, UINT nDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE& src, D3D12_DESCRIPTOR_HEAP_TYPE d3dHeapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) {
		pd3dDevice->CopyDescriptorsSimple(nDescriptors, m_DescriptorHandleFromStart.cpuHandle, src, d3dHeapType);
	}

private:
	ComPtr<ID3D12DescriptorHeap> m_pd3dDescriptorHeap = nullptr;

	Descriptor	m_DescriptorHandleFromStart = {};
	UINT		m_uiDescriptorSize = 0;
	UINT		m_uiCurrentDescriptorCount = 0;
	UINT		m_uiAllocated = 0;

};

