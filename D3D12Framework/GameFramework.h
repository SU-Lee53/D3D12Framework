#pragma once
#include "D3DCore.h"

class GameFramework {
public:
	GameFramework(BOOL bEnableDebugLayer, BOOL bEnableGBV);

	void Update();
	void Render();

private:
	std::shared_ptr<D3DCore> pD3DCore = nullptr;

};
