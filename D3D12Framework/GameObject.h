#pragma once
#include "Component.h"
#include "Script.h"


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
	
	template<ComponentType T>
	std::shared_ptr<T> GetComponent();

	template<ScriptType T>
	void AddScript();

private:
	std::array<std::shared_ptr<Component>, COMPONENT_TYPE_COUNT> m_pComponents = {};
	std::vector<std::shared_ptr<Script>> m_pScripts;

private:
	// Frame Hierarchy
	// ���� ���� ��쿡�� �ش� GameObject �� Root �̰� parent �� child �� ����
	// ���� ���� ��� Root �� Mesh �� ����ְ�, �ڽĵ��� ��ȸ�ϸ� Mesh ���� ������ ��

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
	m_pComponents[Component_Type<T>::componentType] = std::make_shared<T>(shared_from_this(), std::forward<Args>(args)...);
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
