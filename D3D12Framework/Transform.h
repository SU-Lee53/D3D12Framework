#pragma once
#include "Component.h"

class Transform {
public:
	Transform();

	void Update(std::shared_ptr<GameObject> pParent = nullptr);

public:
	void SetLocalMatrix(const Matrix& xmf4x4Local);

public:
	// �׽�Ʈ �ʿ�. �ϴ� ����
	void SetPosition(float x, float y, float z);
	void SetPosition(const Vector3& xmf3Position);

	void Move(Vector3 v3MoveDirection, float fAmount);

public:
	void Rotate(float fPitch, float fYaw, float fRoll);
	void Rotate(const Vector3& xmf3Axis, float fAngle);

public:
	Vector3 GetPosition() const;
	Vector3 GetRight() const;
	Vector3 GetUp() const;
	Vector3 GetLook() const;
	
	Matrix GetWorldMatrix() const;

private:
	// m_mtxFrameRelative : ���� �𵨿��� �θ�κ����� ��� ��ȯ
	// m_mtxTransform : ������Ʈ�� ������ ��ȯ�� ����
	// m_mtxWorld : m_mtxTransform * m_mtxFrameRelative (�ڽ��̶�� * (�θ��� ����))
	//             ���� ���� ���� ���� ��ȯ ���

	Matrix m_mtxFrameRelative	= {};
	Matrix m_mtxTransform		= {};
	Matrix m_mtxWorld			= {};

};
