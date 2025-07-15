#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"

Transform::Transform(std::shared_ptr<GameObject> pOwner)
	: Component{ pOwner }
{
	XMStoreFloat4x4(&m_xmf4x4Local, XMMatrixIdentity());
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
}

void Transform::UpdateShaderVariables(ConstantBuffer& CBuffer)
{
	XMFLOAT4X4 xmf4x4Final;
	XMStoreFloat4x4(&xmf4x4Final, XMMatrixTranspose(XMMatrixMultiply(XMLoadFloat4x4(&m_xmf4x4Local), XMLoadFloat4x4(&m_xmf4x4World))));
	
	::memcpy(CBuffer.pMappedPtr, &xmf4x4Final, sizeof(xmf4x4Final));
}


void Transform::Update()
{
}

void Transform::SetLocalMatrix(const XMFLOAT4X4& xmf4x4Local)
{
	m_xmf4x4Local = xmf4x4Local;
}

// =========
// Translate
// =========

void Transform::SetPosition(float x, float y, float z)
{
	m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;
}

void Transform::SetPosition(const XMFLOAT3& xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

// ======
// Rotate
// ======

void Transform::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(mtxRotate, XMLoadFloat4x4(&m_xmf4x4World)));
}

void Transform::Rotate(const XMFLOAT3& pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&pxmf3Axis), XMConvertToRadians(fAngle));
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(mtxRotate, XMLoadFloat4x4(&m_xmf4x4World)));
}

// =====
// Basis
// =====

XMFLOAT3 Transform::GetLocalRight() const
{
	XMVECTOR xmvLook = XMVectorSet(m_xmf4x4Local._11, m_xmf4x4Local._12, m_xmf4x4Local._13, 0.f);
	XMFLOAT3 xmf3Look;
	XMStoreFloat3(&xmf3Look, xmvLook);
	return xmf3Look;
}

XMFLOAT3 Transform::GetLocalUp() const
{
	XMVECTOR xmvLook = XMVectorSet(m_xmf4x4Local._21, m_xmf4x4Local._22, m_xmf4x4Local._23, 0.f);
	XMFLOAT3 xmf3Look;
	XMStoreFloat3(&xmf3Look, xmvLook);
	return xmf3Look;
}

XMFLOAT3 Transform::GetLocalLook() const
{
	XMVECTOR xmvLook = XMVectorSet(m_xmf4x4Local._31, m_xmf4x4Local._32, m_xmf4x4Local._33, 0.f);
	XMFLOAT3 xmf3Look;
	XMStoreFloat3(&xmf3Look, xmvLook);
	return xmf3Look;
}

XMFLOAT3 Transform::GetWorldPosition() const
{
	return XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
}

XMFLOAT3 Transform::GetWorldRight() const
{
	XMVECTOR xmvLook = XMVectorSet(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13, 0.f);
	XMFLOAT3 xmf3Look;
	XMStoreFloat3(&xmf3Look, xmvLook);
	return xmf3Look;
}

XMFLOAT3 Transform::GetWorldUp() const
{
	XMVECTOR xmvLook = XMVectorSet(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23, 0.f);
	XMFLOAT3 xmf3Look;
	XMStoreFloat3(&xmf3Look, xmvLook);
	return xmf3Look;
}

XMFLOAT3 Transform::GetWorldLook() const
{
	XMVECTOR xmvLook = XMVectorSet(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33, 0.f);
	XMFLOAT3 xmf3Look;
	XMStoreFloat3(&xmf3Look, xmvLook);
	return xmf3Look;
}