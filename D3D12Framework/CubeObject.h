#pragma once
#include "Script.h"

class VerticalCubeObject : public Script {
public:
	VerticalCubeObject(std::shared_ptr<GameObject> pOwner)
		: Script{ pOwner }{}

	void Initialize() override;
	void Update() override;
	void OnCollision() override;
};


class HorizontalCubeObject : public Script {
public:
	HorizontalCubeObject(std::shared_ptr<GameObject> pOwner) 
		: Script{ pOwner }{}

	void Initialize() override;
	void Update() override;
	void OnCollision() override;
};

