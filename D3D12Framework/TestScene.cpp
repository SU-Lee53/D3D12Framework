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
	p1->Initialize();
	
	p1->AddScript<CubeObject>();

	m_pMainCamera = std::make_shared<Camera>();

}

void TestScene::Update()
{
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
