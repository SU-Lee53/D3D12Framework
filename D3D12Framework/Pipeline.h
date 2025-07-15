#pragma once
#include "RootSignature.h"
#include "Shader.h"
#include "DescriptorHeap.h"

struct DescruiptorHeap;

class Pipeline
{
public:
	Pipeline(ComPtr<ID3D12Device14> pd3dDevice) {}
	virtual ~Pipeline() {}

	template<typename T>
	void BindShaderVariables(
		ComPtr<ID3D12GraphicsCommandList> pd3dCommandList,
		SHADER_RESOURCE_TYPE shaderResourceType,
		ROOT_PARAMETER_TYPE rootParameterType,
		std::string_view svBindSemantic, 
		const T& DescriptorToBind) const;

	int GetShaderVariablesCount() const {
		int sum;
		for (const auto& pShader : m_pShaders) {
			if (pShader) {
				sum += pShader->m_nVariables;
			}
		}
		return sum;
	}

public:
	virtual void Run() = 0;

protected:
	inline void Bind(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nRootParameterIndex, const Descriptor& descriptor) const;
	inline void Bind(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nRootParameterIndex, const ConstantBuffer& CBuffer) const;
	inline void Bind(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nRootParameterIndex, const D3D12_GPU_VIRTUAL_ADDRESS& GPUAddress) const;

protected:
	ComPtr<ID3D12PipelineState> m_pPipelineState = nullptr;

	std::shared_ptr<RootSignature> m_pRootSignature = nullptr;
	std::array<std::shared_ptr<ShaderBase>, SHADER_TYPE_COUNT> m_pShaders = {};
};

template<typename T>
inline void Pipeline::BindShaderVariables(
	ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, 
	SHADER_RESOURCE_TYPE shaderResourceType, 
	ROOT_PARAMETER_TYPE rootParameterType, 
	std::string_view svBindSemantic, const T& reource) const
{
	UINT nRootParameterIndex = m_pRootSignature->GetBindPosition(shaderResourceType, rootParameterType, svBindSemantic);
	Bind(nRootParameterIndex, reource);
}


class DiffusedPipeline : public Pipeline {
public:
	DiffusedPipeline(ComPtr<ID3D12Device14> pd3dDevice) : Pipeline{ pd3dDevice } {
		// 1. Init Shader
		m_pShaders[SHADER_TYPE_VERTEX] = std::make_shared<VertexShader>(L"HLSL/DiffusedShader.hlsl", "VSDiffused", 2);
		m_pShaders[SHADER_TYPE_PIXEL] = std::make_shared<PixelShader>(L"HLSL/DiffusedShader.hlsl", "PSDiffused", 0);

		// 2. Init RootSignature
		std::vector<SHADER_RESOURCE_BIND_INFO> binders(2);
		binders[0].InitAsRootDescriptor("TRANSFORM", SHADER_RESOURCE_TYPE_CONSTANT_BUFFER, 0);
		binders[1].InitAsRootDescriptor("CAMERA", SHADER_RESOURCE_TYPE_CONSTANT_BUFFER, 1);

		m_pRootSignature = std::make_shared<RootSignature>(pd3dDevice, binders);

		// 3. Init Descriptor Heap (for SHADER_VISIBLE)
		
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.NumDescriptors = GetShaderVariablesCount();
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;


		// 4. Init Pipeline

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc;
		::ZeroMemory(&pipelineDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		{
			pipelineDesc.pRootSignature = m_pRootSignature->Get();
			pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			pipelineDesc.InputLayout = DiffusedVertex::GetInputLayout();
			pipelineDesc.SampleMask = UINT_MAX;
			pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			pipelineDesc.NumRenderTargets = 1;
			pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			pipelineDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
			pipelineDesc.SampleDesc.Count = 1;
			pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		}
		HRESULT hr = pd3dDevice->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(m_pPipelineState.GetAddressOf()));

		if (FAILED(hr)) {
			::SHOW_ERROR("Create Pipeline Failed");
		}
	}

	virtual ~DiffusedPipeline() {}

	void Run() override;

};
