#include "stdafx.h"
#include "TestScene.h"

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
}

void TestScene::BuilObjects()
{
	std::shared_ptr<GameObject> p1 = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> p2 = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> p3 = std::make_shared<GameObject>();

	AddObjects( p1, p2, p3 );
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
