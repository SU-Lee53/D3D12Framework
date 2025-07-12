#pragma once

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

