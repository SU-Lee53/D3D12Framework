#include "pch.h"
#include "TestScene.h"
#include "CubeObject.h"
#include "Transform.h"

void TestScene::BuildObjects()
{
	InitializeObjects();
}

void TestScene::Update()
{
	UpdateObjects();
}

void TestScene::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommansList)
{
	RenderObjects(pd3dCommansList);
}

void TestScene::ProcessInput()
{
}
