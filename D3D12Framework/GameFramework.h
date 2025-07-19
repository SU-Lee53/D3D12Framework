#pragma once
#include "Scene.h"
#include "D3DCore.h"

class GameFramework {
public:
	GameFramework(BOOL bEnableDebugLayer, BOOL bEnableGBV);

	void Update();
	void Render();

private:
	std::shared_ptr<D3DCore> m_pD3DCore = nullptr;
	std::shared_ptr<Scene> m_pScene = nullptr;

public:
	// Managers
	static std::shared_ptr<ResourceManager> g_pResourceManager;
	static std::shared_ptr<RenderManager> g_pRenderManager;

};

#define RESOURCE		GameFramework::g_pResourceManager
#define RENDER			GameFramework::g_pRenderManager

