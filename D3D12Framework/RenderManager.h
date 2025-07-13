#pragma once
#include "RenderPass.h"

class RenderManager {
public:
	RenderManager();
	~RenderManager();

	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Scene> pScene);

private:
	// just for future extend
	//	std::vector<std::shared_ptr<RenderPass>> m_pGlobalPasses = {};
	//	std::vector<std::shared_ptr<RenderPass>> m_pLocalPassess = {};
	//	std::vector<std::shared_ptr<RenderPass>> m_pPostProcessPasses = {};

	std::vector<std::shared_ptr<RenderPass>> m_pRenderPasses = {};

};

