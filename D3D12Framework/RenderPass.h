#pragma once
#include "Pipeline.h"

class RenderPass {
public:
	RenderPass(ComPtr<ID3D12Device14> pd3dDevice) {}
	virtual ~RenderPass() {}

	virtual void Run() = 0;

protected:
	std::shared_ptr<Pipeline> m_pPipeline = nullptr;
	std::vector<ComPtr<ID3D12Resource>> m_pRTVs;			// for MRT

};

class DiffusedPass : public RenderPass {
public:
	DiffusedPass(ComPtr<ID3D12Device14> pd3dDevice) : RenderPass{ pd3dDevice } {
		m_pPipeline = std::make_shared<Pipeline>(pd3dDevice);
	}

	virtual ~DiffusedPass() {}

	virtual void Run() {




	}

};