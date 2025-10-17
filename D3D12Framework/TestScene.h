#pragma once
#include "Scene.h"
class TestScene : public Scene {
public:
	TestScene();
	virtual ~TestScene();

public:
	void BuilObjects() override;
	void Update() override;
	void Render() override;

protected:
	void ProcessInput() override;


};

