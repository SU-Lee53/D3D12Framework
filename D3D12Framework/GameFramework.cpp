#include "pch.h"
#include "GameFramework.h"
#include "TestScene.h"
#include "Scene.h"

std::unique_ptr<D3DCore> GameFramework::g_pD3DCore = nullptr;
std::unique_ptr<ResourceManager> GameFramework::g_pResourceManager = nullptr;
std::unique_ptr<RenderManager> GameFramework::g_pRenderManager = nullptr;
std::unique_ptr<ShaderManager> GameFramework::g_pShaderManager = nullptr;
std::unique_ptr<TextureManager> GameFramework::g_pTextureManager = nullptr;
std::unique_ptr<SceneManager> GameFramework::g_pSceneManager = nullptr;

GameFramework::GameFramework(BOOL bEnableDebugLayer, BOOL bEnableGBV)
{
	g_pD3DCore = std::make_unique<D3DCore>(bEnableDebugLayer, bEnableGBV);

	// Init managers
	g_pResourceManager = std::make_unique<ResourceManager>(g_pD3DCore->GetDevice());
	g_pShaderManager = std::make_unique<ShaderManager>(g_pD3DCore->GetDevice());
	g_pTextureManager = std::make_unique<TextureManager>(g_pD3DCore->GetDevice());
	g_pRenderManager = std::make_unique<RenderManager>(g_pD3DCore->GetDevice(), g_pD3DCore->GetCommandList());
	g_pSceneManager = std::make_unique<SceneManager>();

	g_pShaderManager->Initialize();
	g_pSceneManager->Initialize();

	// Init Scene
	//m_pScene = std::make_shared<TestScene>();
	//m_pScene->BuildObjects();
	//g_pResourceManager->ExcuteCommandList();
}

void GameFramework::Update()
{
	SCENE->Update();
}

void GameFramework::Render()
{
	g_pD3DCore->RenderBegin();

	// TODO : Render Logic Here
	g_pRenderManager->Render(g_pD3DCore->GetCommandList());
	CUR_SCENE->Render(g_pD3DCore->GetCommandList());

	g_pD3DCore->RenderEnd();

	g_pResourceManager->ResetCBufferBool();

	g_pD3DCore->Present();
	g_pD3DCore->MoveToNextFrame();
}
