#pragma once

enum COMPONENT_TYPE {
	COMPONENT_TYPE_TRANSFORM = 0,
	COMPONENT_TYPE_MOVEMENT,			// undefined
	COMPONENT_TYPE_MESH,				// undefined
	COMPONENT_TYPE_MATERIAL,			// undefined
	COMPONENT_TYPE_ANIMATION,			// undefined
	COMPONENT_TYPE_BEHAVIOR_TREE,		// undefined

	COMPONENT_TYPE_COUNT
};

template <typename T>
class Component {
public:
	Component(std::shared_ptr<GameObject> pOwner) : m_pOwner{ pOwner } {}

	virtual void Update();

	std::shared_ptr<GameObject> GetOwner() {
		return m_pOwner.lock();
	}

private:
	std::weak_ptr<GameObject> m_pOwner;

};

