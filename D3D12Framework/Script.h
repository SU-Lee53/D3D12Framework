#pragma once

class Script {
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void OnCollision() = 0;
};

