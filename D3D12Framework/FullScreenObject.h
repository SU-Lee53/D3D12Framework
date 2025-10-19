#pragma once
#include "GameObject.h"

struct FULLSCREENDATA
{
	float Width;
	float Height;
};

class FullScreenObject : public GameObject {
public:
	FullScreenObject();
	virtual ~FullScreenObject();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;

public:


};

