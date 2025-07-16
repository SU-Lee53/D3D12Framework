#pragma once

struct ConstantBuffer
{
	D3D12_CPU_DESCRIPTOR_HANDLE CBVHandle;
	D3D12_GPU_VIRTUAL_ADDRESS GPUAddress;
	UINT8* pMappedPtr;

	template<typename T>
	void WriteData(const T& data, UINT offset = 0) {
		::memcpy(pMappedPtr + (offset * sizeof(T)), &data, sizeof(T));
	}

	void Bind(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nRootParameterIndex) {
		pd3dCommandList->SetGraphicsRootConstantBufferView(nRootParameterIndex, GPUAddress);
	}
};

