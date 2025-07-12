#pragma once
#include "Component.h"


class GameObject : public std::enable_shared_from_this<GameObject> {
public:
	GameObject();
	~GameObject();

public:
	void Update();
	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);

public:
	void AddComponent(COMPONENT_TYPE eComponentType);

private:
	std::array<std::shared_ptr<Component>, COMPONENT_TYPE_COUNT> m_pComponents = {};
	std::vector<std::shared_ptr<class Script>> m_pScripts;

private:
	// Frame Hierarchy
	std::shared_ptr<GameObject> m_pParent = nullptr;
	std::vector<std::shared_ptr<GameObject>> m_pChild = {};
	
private:
	// Bounding Volume
	BoundingOrientedBox m_xmOBB;

};

