#include "stdafx.h"
#include "RenderManager.h"

RenderManager::RenderManager(ComPtr<ID3D12Device14> pd3dDevice)
{
	std::shared_ptr<DiffusedPass> pDiffusedPass = std::make_shared<DiffusedPass>(pd3dDevice);
	m_pRenderPasses.push_back(pDiffusedPass);
}

RenderManager::~RenderManager()
{
}

void RenderManager::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Scene> pScene)
{
	for (auto& pass : m_pRenderPasses) {
		pass->Run(pd3dCommandList, pScene);
	}
}
