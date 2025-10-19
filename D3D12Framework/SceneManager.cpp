#include "pch.h"
#include "SceneManager.h"
#include "IntroScene.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	m_upCurrentScene = std::make_unique<IntroScene>();
	m_upCurrentScene->BuildObjects();
}

void SceneManager::ProcessInput() 
{

}

void SceneManager::Update()
{

}

void SceneManager::Render()
{

}
