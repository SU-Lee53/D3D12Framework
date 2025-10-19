#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "CubeObject.h"

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

	m_Transform.Update(m_pParent);

	for (auto& pChild : m_pChildren) {
		pChild->Update();
	}
}

void GameObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	// TODO : Render Logic Here
	for (auto& pChild : m_pChildren) {
		pChild->Render(pd3dCommandList);
	}
}
