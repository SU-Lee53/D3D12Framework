// D3D12Framework.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "GameFramework.h"
#include "TestScene.h"
#include "Scene.h"

std::shared_ptr<ResourceManager> GameFramework::m_pResourceManager = nullptr;

GameFramework::GameFramework(BOOL bEnableDebugLayer, BOOL bEnableGBV)
	: m_pD3DCore{ std::make_shared<D3DCore>(bEnableDebugLayer, bEnableGBV) }
{
	// Init managers
	m_pResourceManager = std::make_shared<ResourceManager>(m_pD3DCore->GetDevice());


	// Init Scene
	m_pScene = std::make_shared<TestScene>();
	m_pScene->BuilObjects();
	m_pResourceManager->ExcuteCommandList();
}

void GameFramework::Update()
{
	m_pScene->Update();
}

void GameFramework::Render()
{
	m_pD3DCore->RenderBegin();

	// TODO : Render Logic Here

	pD3DCore->RenderEnd();
	pD3DCore->Present();
}
