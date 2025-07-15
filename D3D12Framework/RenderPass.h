#pragma once
#include "Pipeline.h"

class RenderPass {
public:
	RenderPass(ComPtr<ID3D12Device14> pd3dDevice) {}
	virtual ~RenderPass() {}

	virtual void Run(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Scene> pScene) = 0;

protected:


protected:
	std::shared_ptr<Pipeline> m_pPipeline = nullptr;
	std::vector<ComPtr<ID3D12Resource>> m_pRTVs;			// for MRT
	std::shared_ptr<DescriptorHeap> m_DescriptorHeaps = nullptr;

};

class DiffusedPass : public RenderPass {
public:
	DiffusedPass(ComPtr<ID3D12Device14> pd3dDevice) : RenderPass{ pd3dDevice } {
		m_pPipeline = std::make_shared<DiffusedPipeline>(pd3dDevice);
	}

	virtual ~DiffusedPass() {}

	virtual void Run(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Scene> pScene) override;

};