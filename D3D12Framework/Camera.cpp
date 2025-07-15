#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::GenerateViewMatrix()
{
}

void Camera::GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up)
{
}

void Camera::RegenerateViewMatrix()
{
}

void Camera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
}

void Camera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ, float fMaxZ)
{
}

void Camera::SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom)
{
}

void Camera::SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

XMFLOAT3 Camera::GetPosition() const
{
	return XMFLOAT3();
}

XMFLOAT3 Camera::GetRight() const
{
	return XMFLOAT3();
}

XMFLOAT3 Camera::GetUp() const
{
	return XMFLOAT3();
}

XMFLOAT3 Camera::GetLook() const
{
	return XMFLOAT3();
}