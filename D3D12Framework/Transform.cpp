#include "pch.h"
#include "Transform.h"
#include "GameObject.h"

Transform::Transform()
{
	m_mtxTransform = Matrix::Identity;
	m_mtxWorld = Matrix::Identity;
}

void Transform::Update(std::shared_ptr<GameObject> pParent)
{
	m_mtxWorld = (pParent) ? ((m_mtxTransform * m_mtxFrameRelative) * pParent->GetTransform().m_mtxWorld) : (m_mtxTransform * m_mtxFrameRelative);
}

//void Transform::SetLocalMatrix(const Matrix& xmf4x4Local)
//{
//	m_mtxTransform = xmf4x4Local;
//}

// =========
// Translate
// =========

void Transform::SetPosition(float x, float y, float z)
{
	m_mtxTransform.Translation(Vector3(x, y, z));
}

void Transform::SetPosition(const Vector3& v3Position)
{
	m_mtxTransform.Translation(v3Position);
}

void Transform::Move(Vector3 v3MoveDirection, float fAmount)
{
	m_mtxTransform.Translation(v3MoveDirection * fAmount);	// DeltaTime required
}

// ======
// Rotate
// ======

void Transform::Rotate(float fPitch, float fYaw, float fRoll)
{
	m_mtxTransform = Matrix::CreateFromYawPitchRoll(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll)) * m_mtxTransform;
}

void Transform::Rotate(const Vector3& v3Axis, float fAngle)
{
	m_mtxTransform = Matrix::CreateFromAxisAngle(v3Axis, XMConvertToRadians(fAngle)) * m_mtxTransform;
}

// =====
// Basis
// =====

Vector3 Transform::GetPosition() const
{
	return m_mtxWorld.Translation();
}

Vector3 Transform::GetRight() const
{
	return m_mtxWorld.Right();
}

Vector3 Transform::GetUp() const
{
	return m_mtxWorld.Up();
}

Vector3 Transform::GetLook() const
{
	return m_mtxWorld.ForwardPass();
}

Matrix Transform::GetWorldMatrix() const
{
	return m_mtxWorld;
}
