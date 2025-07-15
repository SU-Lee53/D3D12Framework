#include "stdafx.h"
#include "Pipeline.h"
#include "DescriptorHeap.h"

void DiffusedPipeline::Run()
{
}

void Pipeline::Bind(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nRootParameterIndex, const Descriptor& descriptor) const
{
	pd3dCommandList->SetGraphicsRootDescriptorTable(nRootParameterIndex, descriptor.gpuHandle);
}

void Pipeline::Bind(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nRootParameterIndex, const ConstantBuffer& CBuffer) const
{
	pd3dCommandList->SetGraphicsRootConstantBufferView(nRootParameterIndex, CBuffer.pGPUAddress);
}

void Pipeline::Bind(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nRootParameterIndex, const D3D12_GPU_VIRTUAL_ADDRESS& GPUAddress) const
{
	pd3dCommandList->SetGraphicsRootConstantBufferView(nRootParameterIndex, GPUAddress);
}
