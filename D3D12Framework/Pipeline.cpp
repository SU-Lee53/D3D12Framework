#include "stdafx.h"
#include "Pipeline.h"
#include "DescriptorHeap.h"

void Pipeline::BindShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::string_view svBindSemantic, const Descriptor& DescriptorToBind)
{
	auto [eResourceType, slot] = m_pRootSignature->GetRootParameterIndex(svBindSemantic);

	if (eResourceType == SHADER_RESOURCE_TYPE_UNDEFINED) {
		__debugbreak();
	}

	switch (eResourceType)
	{
	case SHADER_RESOURCE_TYPE_CONSTANT_BUFFER:
	case SHADER_RESOURCE_TYPE_TEXTURE:
		pd3dCommandList->SetGraphicsRootDescriptorTable(slot, DescriptorToBind.gpuHandle);
		break;
	default:
		__debugbreak();
		break;
	}

}

void Pipeline::BindShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::string_view svBindSemantic, ComPtr<ID3D12Resource> pd3dResource)
{
	auto [eResourceType, slot] = m_pRootSignature->GetBindSlot(svBindSemantic);

	if (eResourceType == SHADER_RESOURCE_TYPE_UNDEFINED) {
		__debugbreak();
	}

	switch (eResourceType)
	{
	case SHADER_RESOURCE_TYPE_STRUCTURED_BUFFER:
		pd3dCommandList->SetGraphicsRootShaderResourceView(slot, pd3dResource->GetGPUVirtualAddress());
		break;
		
	default:
		__debugbreak();
		break;
	}
}

void DiffusedPipeline::Run()
{
}
