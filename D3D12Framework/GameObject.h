#pragma once
#include "Component.h"
#include "Script.h"
#include "Transform.h"


class GameObject : public std::enable_shared_from_this<GameObject> {
public:
	GameObject();
	~GameObject();

public:
	void Initialize();
	void Update();
	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);

public:
	template<ComponentType T>
	void AddComponent();
	
	template<ComponentType T, typename... Args>
	void AddComponent(Args&&... args);
	
	
	template<ComponentType T, typename... Args>
	void AddComponent(std::shared_ptr<T> pComponent);
	
	template<ComponentType T>
	std::shared_ptr<T> GetComponent();

	Transform& GetTransform() { return m_Transform; }

	template<ScriptType T>
	void AddScript();

private:
	Transform m_Transform{};
	std::array<std::shared_ptr<Component>, COMPONENT_TYPE_COUNT> m_pComponents = {};
	std::vector<std::shared_ptr<Script>> m_pScripts;

private:
	// Frame Hierarchy
	// 단일 모델의 경우에는 해당 GameObject 이 Root 이고 parent 와 child 는 없음
	// 계층 모델의 경우 Root 의 Mesh 는 비어있고, 자식들을 순회하며 Mesh 들을 렌더링 함

	std::shared_ptr<GameObject> m_pParent = nullptr;
	std::vector<std::shared_ptr<GameObject>> m_pChildren = {};
	
private:
	// Bounding Volume
	BoundingOrientedBox m_xmOBB;

};

template<ComponentType T>
inline void GameObject::AddComponent()
{
	m_pComponents[Component_Type<T>::componentType] = std::make_shared<T>(shared_from_this());
}

template<ComponentType T, typename... Args>
inline void GameObject::AddComponent(Args&&... args)
{
	if constexpr (std::derived_from<T, MeshRenderer>) {
		m_pComponents[MeshRenderer::componentType] = std::make_shared<T>(shared_from_this(), std::forward<Args>(args)...);
	}
	else {
		m_pComponents[Component_Type<T>::componentType] = std::make_shared<T>(shared_from_this(), std::forward<Args>(args)...);
	}
}

template<ComponentType T, typename ...Args>
inline void GameObject::AddComponent(std::shared_ptr<T> pComponent)
{
	if (pComponent->IsOwnerExpired() || pComponent->GetOwner().get() != this) {
		pComponent->SetOwner(shared_from_this());
	}

	if constexpr (std::derived_from<T, MeshRenderer>) {
		m_pComponents[MeshRenderer::componentType] = pComponent;
	}
	else {
		m_pComponents[Component_Type<T>::componentType] = pComponent;
	}
}

template<ComponentType T>
inline 	std::shared_ptr<T> GameObject::GetComponent()
{
	return static_pointer_cast<T>(m_pComponents[Component_Type<T>::componentType]);
}

template<ScriptType T>
inline void GameObject::AddScript()
{
	std::shared_ptr<T> pScript = std::make_shared<T>(shared_from_this());
	m_pScripts.push_back(pScript);
}
