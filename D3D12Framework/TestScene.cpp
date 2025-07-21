#include "stdafx.h"
#include "TestScene.h"
#include "CubeObject.h"

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
}

void TestScene::BuilObjects()
{
	std::shared_ptr<GameObject> p1 = std::make_shared<GameObject>();
	p1->AddScript<CubeObject>();
	p1->Initialize();


	AddObject(p1);

	m_pMainCamera = std::make_shared<Camera>();
	m_pMainCamera->SetViewport(0, 0, WinCore::sm_dwClientWidth, WinCore::sm_dwClientHeight, 0.f, 1.f);
	m_pMainCamera->SetScissorRect(0, 0, WinCore::sm_dwClientWidth, WinCore::sm_dwClientHeight);
	m_pMainCamera->GenerateViewMatrix(XMFLOAT3(0.f, 0.f, -15.f), XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(0.f, 1.f, 0.f));
	m_pMainCamera->GenerateProjectionMatrix(1.01f, 500.f, (WinCore::sm_dwClientWidth / WinCore::sm_dwClientHeight), 90.f);
}

void TestScene::Update()
{
	//__debugbreak();

	for (auto& obj : m_pGameObjects) {
		obj->Update();
	}

}

void TestScene::Render()
{
}

void TestScene::ProcessInput()
{
}

void TestScene::UpdateObjects()
{
}
