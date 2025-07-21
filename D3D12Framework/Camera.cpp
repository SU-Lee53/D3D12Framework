#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
	XMStoreFloat4x4(&m_xmf4x4View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_xmf4x4Projection, XMMatrixIdentity());
}

Camera::~Camera()
{
}

void Camera::GenerateViewMatrix()
{
	XMStoreFloat4x4(&m_xmf4x4View, XMMatrixLookAtLH(
		XMLoadFloat3(&m_xmf3Position),
		XMLoadFloat3(&m_xmf3LookAtWorld),
		XMLoadFloat3(&m_xmf3Up))
	);
}

void Camera::GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up)
{
	m_xmf3Position = xmf3Position;
	m_xmf3LookAtWorld = xmf3LookAt;
	m_xmf3Up = xmf3Up;

	GenerateViewMatrix();
}

void Camera::RegenerateViewMatrix()
{
}

void Camera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	m_ffovY = fFOVAngle;
	m_fAspectRatio = fAspectRatio;
	m_fNear = fNearPlaneDistance;
	m_fFar = fFarPlaneDistance;
	
	float FOVAngleInRad = XMConvertToRadians(m_ffovY);

	XMStoreFloat4x4(&m_xmf4x4Projection,
		XMMatrixPerspectiveFovLH(
			FOVAngleInRad,
			fAspectRatio,
			fNearPlaneDistance,
			fFarPlaneDistance
		));

	BoundingFrustum::CreateFromMatrix(m_xmFrustum, XMLoadFloat4x4(&m_xmf4x4Projection));
}

void Camera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ, float fMaxZ)
{
	m_d3dViewport.TopLeftX = xTopLeft;
	m_d3dViewport.TopLeftY = yTopLeft;
	m_d3dViewport.Width = nWidth;
	m_d3dViewport.Height = nHeight;
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;
}

void Camera::SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom)
{
	m_d3dScissorRect.left = xLeft;
	m_d3dScissorRect.right = xRight;
	m_d3dScissorRect.top = yTop;
	m_d3dScissorRect.bottom = yBottom;
}

void Camera::SetViewportsAndScissorRects(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	pd3dCommandList->RSSetViewports(1, &m_d3dViewport);
	pd3dCommandList->RSSetScissorRects(1, &m_d3dScissorRect);
}

XMFLOAT4X4 Camera::GetViewProjectMatrix() const
{
	XMFLOAT4X4 ret;

	XMMATRIX xmmtxView = XMLoadFloat4x4(&m_xmf4x4View);
	XMMATRIX xmmtxProject = XMLoadFloat4x4(&m_xmf4x4Projection);

	XMStoreFloat4x4(&ret, XMMatrixMultiply(xmmtxView, xmmtxProject));
	return ret;
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
