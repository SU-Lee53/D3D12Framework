#pragma once
#include "Component.h"

class Transform : public Component {
public:
	Transform(std::shared_ptr<GameObject> pOwner);

	virtual void Update() override;

public:
	void SetLocalMatrix(const XMFLOAT4X4& xmf4x4Local);

public:
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& xmf3Position);

public:
	void Rotate(float fPitch, float fYaw, float fRoll);
	void Rotate(const XMFLOAT3& pxmf3Axis, float fAngle);

public:
	XMFLOAT3 GetLocalRight() const;
	XMFLOAT3 GetLocalUp() const;
	XMFLOAT3 GetLocalLook() const;

	XMFLOAT3 GetWorldPosition() const;
	XMFLOAT3 GetWorldRight() const;
	XMFLOAT3 GetWorldUp() const;
	XMFLOAT3 GetWorldLook() const;
	
	XMFLOAT4X4 GetLocalMatrix() const;
	XMFLOAT4X4 GetWorldMatrix() const;

private:
	XMFLOAT4X4 m_xmf4x4Local = {};	// relative
	XMFLOAT4X4 m_xmf4x4World = {};	// world
	
	std::string m_strBindSemantic = "TRANSFORM";
};

template <>
struct Component_Type<Transform> {
	constexpr static COMPONENT_TYPE componentType = COMPONENT_TYPE_TRANSFORM;
};