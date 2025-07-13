#pragma once
#include "Pipeline.h"
#include "DescriptorHeap.h"

class RenderPass {
public:
	RenderPass(ComPtr<ID3D12Device14> pd3dDevice) {}
	virtual ~RenderPass() {}

	void BindShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::string_view svBindSemantic, const Descriptor& DescriptorToBind) {
		m_pPipeline->BindShaderVariables(pd3dCommandList, svBindSemantic, DescriptorToBind);
	}

	void BindShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::string_view svBindSemantic, ComPtr<ID3D12Resource> pd3dResource) {
		m_pPipeline->BindShaderVariables(pd3dCommandList, svBindSemantic, pd3dResource);
	}


	virtual void Run(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Scene> pScene) = 0;

protected:
	std::shared_ptr<Pipeline> m_pPipeline = nullptr;
	std::vector<ComPtr<ID3D12Resource>> m_pRTVs;			// for MRT
	std::shared_ptr<DescriptorHeap> m_DescriptorHeaps = nullptr;

};

class DiffusedPass : public RenderPass {
public:
	DiffusedPass(ComPtr<ID3D12Device14> pd3dDevice) : RenderPass{ pd3dDevice } {
		m_pPipeline = std::make_shared<Pipeline>(pd3dDevice);
	}

	virtual ~DiffusedPass() {}

	virtual void Run(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Scene> pScene) override;

};