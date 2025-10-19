#pragma once

struct ConstantBuffer
{
	D3D12_CPU_DESCRIPTOR_HANDLE CBVHandle;
	D3D12_GPU_VIRTUAL_ADDRESS GPUAddress;
	UINT8* pMappedPtr;

	template<typename T>
	void WriteData(const T* pData) {
		::memcpy(pMappedPtr, pData, sizeof(T));
	}

	void Bind(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nRootParameterIndex) {
		pd3dCommandList->SetGraphicsRootConstantBufferView(nRootParameterIndex, GPUAddress);
	}
};

