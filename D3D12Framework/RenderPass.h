#pragma once
#include "Pipeline.h"

struct SHADER_RESOURCE_BIND_STRUCT {
	std::string strBindSemantics;
	SHADER_RESOURCE_TYPE resourceType;

};

struct std::hash<SHADER_RESOURCE_BIND_STRUCT> {
	size_t operator()(const SHADER_RESOURCE_BIND_STRUCT& s) {
		return std::hash<std::string>{}(s.strBindSemantics);
	}
};

class RenderPass {
public:
	RenderPass(ComPtr<ID3D12Device14> pd3dDevice) {}
	virtual ~RenderPass() {}

	virtual void Run(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::span<GameObject> pGameObject) = 0;

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

	virtual void Run(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) {




	}

};