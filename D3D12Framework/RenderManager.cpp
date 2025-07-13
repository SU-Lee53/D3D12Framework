#include "stdafx.h"
#include "RenderManager.h"

RenderManager::RenderManager()
{
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
