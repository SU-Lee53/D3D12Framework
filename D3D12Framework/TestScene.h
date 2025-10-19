#pragma once
#include "Scene.h"
class TestScene : public Scene {
public:
	void BuildObjects() override;
	void Update() override;
	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommansList) override;

protected:
	void ProcessInput() override;


};

