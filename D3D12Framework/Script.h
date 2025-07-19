#pragma once

class Script {
public:
	Script(std::shared_ptr<GameObject> pOwner) 
		: m_wpOwner{ pOwner } {}

	virtual void Initialize()	= 0;
	virtual void Update()		= 0;
	virtual void OnCollision()	= 0;


protected:
	std::weak_ptr<GameObject> m_wpOwner;

};


template <typename C>
concept ScriptType = std::derived_from<C, Script>;
