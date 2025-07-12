#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (auto& component : m_pComponents) {
		if (component) {
			component->Update();
		}
	}

	//	for (auto& script : m_pScripts) {
	//		script->Update();
	//	}
}

void GameObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
}

void GameObject::AddComponent(COMPONENT_TYPE eComponentType)
{
}
