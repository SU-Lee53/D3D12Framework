#pragma once
#include "GameObject.h"

class CubeObject : public GameObject {
public:
	CubeObject();
	virtual ~CubeObject();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList) override;

private:

};