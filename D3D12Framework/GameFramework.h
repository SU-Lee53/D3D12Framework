#pragma once
#include "Scene.h"
#include "D3DCore.h"

#include "RenderManager.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "SceneManager.h"

class GameFramework {
public:
	GameFramework(BOOL bEnableDebugLayer, BOOL bEnableGBV);

	void Update();
	void Render();

private:
	//std::unique_ptr<Scene> m_pScene = nullptr;

public:
	// Core & Managers
	static std::unique_ptr<D3DCore>				g_pD3DCore;
	static std::unique_ptr<ResourceManager>		g_pResourceManager;
	static std::unique_ptr<RenderManager>		g_pRenderManager;
	static std::unique_ptr<ShaderManager>		g_pShaderManager;
	static std::unique_ptr<TextureManager>		g_pTextureManager;
	static std::unique_ptr<SceneManager>		g_pSceneManager;

};

#define D3DCORE			GameFramework::g_pD3DCore
#define RESOURCE		GameFramework::g_pResourceManager
#define RENDER			GameFramework::g_pRenderManager
#define SHADER			GameFramework::g_pShaderManager
#define TEXTURE			GameFramework::g_pTextureManager
#define SCENE			GameFramework::g_pSceneManager


#define CUR_SCENE		GameFramework::g_pSceneManager->GetCurrentScene()
