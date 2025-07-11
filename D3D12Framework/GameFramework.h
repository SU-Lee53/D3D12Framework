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
	static std::shared_ptr<ResourceManager> m_pResourceManager;

};

#define RESOURCE		GameFramework::m_pResourceManager

