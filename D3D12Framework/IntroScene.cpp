#include "pch.h"
#include "IntroScene.h"
#include "FullScreenObject.h"
#include "TestScene.h"

void IntroScene::BuildObjects()
{
	std::shared_ptr<FullScreenObject> pFullScreen = std::make_shared<FullScreenObject>();
	AddObject(pFullScreen);

	InitializeObjects();
}

void IntroScene::Update()
{
	UpdateObjects();
}

void IntroScene::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommansList)
{
	RenderObjects(pd3dCommansList);
}

void IntroScene::ProcessInput()
{
	if (INPUT->GetButtonDown(VK_SPACE)) {
		SCENE->ChangeScene<TestScene>();
	}
}
