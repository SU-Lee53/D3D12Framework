#pragma once

class GameObject;

class Component {
public:
	Component(std::shared_ptr<GameObject> pOwner);

	virtual void Initialize () = 0;
	virtual void Update() = 0;

	std::shared_ptr<GameObject> GetOwner() {
		return m_wpOwner.lock();
	}

	void SetOwner(std::shared_ptr<GameObject> pOwner) {
		m_wpOwner = pOwner;
	}

	bool IsOwnerExpired() {
		return m_wpOwner.expired();
	}

protected:
	std::weak_ptr<GameObject> m_wpOwner;

};

template<typename C>
concept ComponentType = std::derived_from<C, Component>;

template <ComponentType T>
struct Component_Type {
	constexpr static COMPONENT_TYPE componentType = COMPONENT_TYPE_BASE;
};
