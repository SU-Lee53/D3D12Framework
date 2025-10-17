#pragma once
#include "Component.h"

class Transform {
public:
	Transform();

	void Update(std::shared_ptr<GameObject> pParent = nullptr);

public:
	void SetLocalMatrix(const Matrix& xmf4x4Local);

public:
	// 테스트 필요. 일단 제거
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
	// m_mtxFrameRelative : 계층 모델에서 부모로부터의 상대 변환
	// m_mtxTransform : 오브젝트가 수행할 변환의 누적
	// m_mtxWorld : m_mtxTransform * m_mtxFrameRelative (자식이라면 * (부모의 월드))
	//             월드 원점 기준 최종 변환 행렬

	Matrix m_mtxFrameRelative	= {};
	Matrix m_mtxTransform		= {};
	Matrix m_mtxWorld			= {};

};
