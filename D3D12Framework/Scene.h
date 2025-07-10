#pragma once
class Scene {
public:
	Scene() {}
	virtual ~Scene() {}

public:
	virtual void BuilObjects() = 0;

public:
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	virtual void ProcessInput() = 0;
	virtual void UpdateObjects() = 0;

protected:
	std::vector<std::shared_ptr<class GameObject>> pGameObjects = {};
	std::shared_ptr<class Camera> pMainCamera = nullptr;

};

