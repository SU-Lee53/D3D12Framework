#pragma once
#include "RenderPass.h"

class RenderManager {
public:
	RenderManager(ComPtr<ID3D12Device14> pd3dDevice);
	~RenderManager();

	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Scene> pScene);

private:
	std::vector<std::shared_ptr<RenderPass>> m_pRenderPasses = {};

};

