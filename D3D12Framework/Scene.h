#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"

class Scene {
public:
	virtual void BuildObjects() = 0;

public:
	void AddObject(std::shared_ptr<GameObject> pObj) {
		m_pGameObjects.push_back(pObj);
	}

	template<typename... Objs, 
		typename = std::enable_if_t<(std::is_same_v<Objs, std::shared_ptr<GameObject>> && ...)>>
	void AddObjects(Objs... pObjs) {
		(m_pGameObjects.push_back(std::forward<Objs>(pObjs)), ...);
	}

public:
	virtual void ProcessInput() = 0;
	virtual void Update() = 0;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommansList) = 0;

public:
	std::shared_ptr<GameObject> GetPlayer() const { return m_pPlayer; }
	std::shared_ptr<Camera> GetCamera() const { return m_pMainCamera; }
	std::vector<std::shared_ptr<GameObject>>& GetObjectsInScene() { return m_pGameObjects; }

	CB_LIGHT_DATA MakeLightData();

protected:
	void InitializeObjects();
	void UpdateObjects();
	void RenderObjects(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);

protected:
	std::vector<std::shared_ptr<GameObject>> m_pGameObjects = {};
	std::vector<std::shared_ptr<Light>> m_pLights = {};
	
	std::shared_ptr<GameObject> m_pPlayer = nullptr;
	std::shared_ptr<Camera> m_pMainCamera = nullptr;

};

