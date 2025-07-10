#pragma once
#include "RootSignature.h"
#include "Shader.h"

class Pipeline
{
public:
	Pipeline(ComPtr<ID3D12Device14> pd3dDevice) {}
	virtual ~Pipeline() {}

protected:
	ComPtr<ID3D12PipelineState> m_pPipelineState = nullptr;

	std::shared_ptr<RootSignature> m_pRootSignature = nullptr;
	std::array<std::shared_ptr<ShaderBase>, SHADER_TYPE_COUNT> m_pShaders = {};
};


class DiffusedPipeline : public Pipeline {
public:
	DiffusedPipeline(ComPtr<ID3D12Device14> pd3dDevice) : Pipeline{ pd3dDevice } {
		// 1. Init Shader
		m_pShaders[SHADER_TYPE_VERTEX] = std::make_shared<VertexShader>(L"HLSL/DiffusedShader.hlsl", "VSDiffused");
		m_pShaders[SHADER_TYPE_PIXEL] = std::make_shared<PixelShader>(L"HLSL/DiffusedShader.hlsl", "PSDiffused");

		// 2. Init RootSignature
		m_pRootSignature = std::make_shared<DiffusedRootSignature>(pd3dDevice);

		// 3. Init Pipeline

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





};

