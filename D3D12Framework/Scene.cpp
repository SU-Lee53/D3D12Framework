#include "pch.h"
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

void Scene::RenderObjects(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	if (m_pPlayer)
		m_pPlayer->Render(pd3dCommandList);

	for (auto& obj : m_pGameObjects) {
		obj->Render(pd3dCommandList);
	}
}
