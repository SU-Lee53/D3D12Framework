#include "stdafx.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Initialize()
{
	for (auto& component : m_pComponents) {
		if (component) {
			component->Initialize();
		}
	}
	for (auto& script : m_pScripts) {
		script->Initialize();
	}
	
	for (auto& pChild : m_pChildren) {
		pChild->Initialize();
	}
}

void GameObject::Update()
{
	for (auto& component : m_pComponents) {
		if (component) {
			component->Update();
		}
	}

	for (auto& script : m_pScripts) {
		script->Update();
	}

	m_Transform.Update(m_pParent);

	for (auto& pChild : m_pChildren) {
		pChild->Update();
	}
}

void GameObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	// TODO : Render Logic Here
	if (m_pParent) {
		m_pParent->Render(pd3dCommandList);
	}

	for (auto& pChild : m_pChildren) {
		pChild->Render(pd3dCommandList);
	}
}
