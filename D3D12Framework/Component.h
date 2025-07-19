#pragma once

class GameObject;

class Component {
public:
	Component(std::shared_ptr<GameObject> pOwner);

	virtual void Update() = 0;

	std::shared_ptr<GameObject> GetOwner() {
		return m_pOwner.lock();
	}

protected:
	std::weak_ptr<GameObject> m_pOwner;

};

template <typename C>
concept ComponentType = std::derived_from<C, Component>;

template <ComponentType T>
struct Component_Type {
	constexpr static COMPONENT_TYPE componentType = COMPONENT_TYPE_BASE;
};