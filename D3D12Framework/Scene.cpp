#include "stdafx.h"
#include "Scene.h"

void Scene::InitializeObjects()
{
	if (m_pPlayer)
		m_pPlayer->Initialize();

	for (auto& obj : m_pGameObjects) {
		obj->Initialize();
	}
}

void Scene::UpdateObjects()
{
	if (m_pPlayer)
		m_pPlayer->Update();

	for (auto& obj : m_pGameObjects) {
		obj->Update();
	}
}
