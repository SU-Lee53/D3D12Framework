#pragma once
#include "Script.h"

class CubeObject : public Script {
public:
	CubeObject(std::shared_ptr<GameObject> pOwner) 
		: Script{ pOwner }{}

	void Initialize() override;
	void Update() override;
	void OnCollision() override;
};

