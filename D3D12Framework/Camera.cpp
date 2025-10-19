#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	XMStoreFloat4x4(&m_mtxView, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mtxProjection, XMMatrixIdentity());
}

Camera::~Camera()
{
}

void Camera::GenerateViewMatrix()
{
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(m_v3Position, m_v3LookAtWorld, m_v3Up));
}

void Camera::GenerateViewMatrix(Vector3 v3Position, Vector3 v3LookAt, Vector3 v3Up)
{
	m_v3Position = v3Position;
	m_v3LookAtWorld = v3LookAt;
	m_v3Up = v3Up;

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

	XMStoreFloat4x4(&m_mtxProjection,
		XMMatrixPerspectiveFovLH(
			FOVAngleInRad,
			fAspectRatio,
			fNearPlaneDistance,
			fFarPlaneDistance
		)
	);

	BoundingFrustum::CreateFromMatrix(m_vrustum, XMLoadFloat4x4(&m_mtxProjection));
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

CB_CAMERA_DATA Camera::MakeCBData() const
{
	CB_CAMERA_DATA camData{
		.mtxView = m_mtxView.Transpose(),
		.mtxProjection = m_mtxProjection.Transpose(),
		.v3CameraPosition = m_v3Position
	};


	return camData;
}

Matrix Camera::GetViewProjectMatrix() const
{
	Matrix ret;

	XMMATRIX xmmtxView = XMLoadFloat4x4(&m_mtxView);
	XMMATRIX xmmtxProject = XMLoadFloat4x4(&m_mtxProjection);

	XMStoreFloat4x4(&ret, XMMatrixMultiply(xmmtxView, xmmtxProject));
	return ret;
}

Vector3 Camera::GetPosition() const
{
	return m_v3Position;
}

Vector3 Camera::GetRight() const
{
	return m_v3Right;
}

Vector3 Camera::GetUp() const
{
	return m_v3Up;
}

Vector3 Camera::GetLook() const
{
	return m_v3Look;
}
