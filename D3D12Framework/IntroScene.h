#pragma once
#include "Scene.h"

class IntroScene : public Scene {
public:
	void ProcessInput() override;
	void BuildObjects() override;
	void Update() override;
	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommansList) override;


};

