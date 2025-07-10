#pragma once

struct ConstantBuffer
{
	D3D12_CPU_DESCRIPTOR_HANDLE CBVHandle;
	D3D12_GPU_VIRTUAL_ADDRESS pGPUAddress;
	UINT8* pMappedPtr;
};

