// D3D12Framework.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "GameFramework.h"

GameFramework::GameFramework(BOOL bEnableDebugLayer, BOOL bEnableGBV)
	: pD3DCore{ std::make_shared<D3DCore>(bEnableDebugLayer, bEnableGBV) }
{
}

void GameFramework::Update()
{
}

void GameFramework::Render()
{
	pD3DCore->RenderBegin();

	// TODO : Render Logic Here

	pD3DCore->RenderEnd();
	pD3DCore->Present();
}
