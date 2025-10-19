#include "pch.h"
#include "TestScene.h"
#include "CubeObject.h"
#include "Transform.h"

void TestScene::BuildObjects()
{
	std::shared_ptr<CubeObject> pCube1 = std::make_shared<CubeObject>();
	pCube1->GetTransform().SetPosition(15.f, 0.f, 30.f);
	AddObject(pCube1);
	
	std::shared_ptr<CubeObject> pCube2 = std::make_shared<CubeObject>();
	pCube2->GetTransform().SetPosition(-15.f, 0.f, 30.f);
	AddObject(pCube2);

	m_pMainCamera = std::make_shared<Camera>();
	m_pMainCamera->SetViewport(0, 0, WinCore::sm_dwClientWidth, WinCore::sm_dwClientHeight, 0.f, 1.f);
	m_pMainCamera->SetScissorRect(0, 0, WinCore::sm_dwClientWidth, WinCore::sm_dwClientHeight);
	m_pMainCamera->GenerateViewMatrix(XMFLOAT3(0.f, 0.f, -15.f), XMFLOAT3(0.f, 0.f, 1.f), XMFLOAT3(0.f, 1.f, 0.f));
	m_pMainCamera->GenerateProjectionMatrix(1.01f, 5000.0f, (WinCore::sm_dwClientWidth / WinCore::sm_dwClientHeight), 60.0f);

	//m_pLights.resize(MAX_LIGHTS);

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
