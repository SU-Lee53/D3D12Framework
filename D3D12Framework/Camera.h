#pragma once


struct CB_CAMERA_DATA
{
	Matrix	mtxView;
	Matrix	mtxProjection;
	Vector3 v3CameraPosition;
};


class Camera {
public:
	Camera();
	~Camera();

	void Update();

public:
	void SetPosition(float x, float y, float z);
	void SetPosition(const Vector3& v3Position);

public:
	Matrix GetViewProjectMatrix() const;

	Vector3 GetPosition() const;
	Vector3 GetRight() const;
	Vector3 GetUp() const;
	Vector3 GetLook() const;

public:
	void Rotate(float fPitch, float fYaw, float fRoll);
	void Rotate(const Vector3& pv3Axis, float fAngle);

	void GenerateViewMatrix();
	void GenerateViewMatrix(Vector3 v3Position, Vector3 v3LookAt, Vector3 v3Up);
	void RegenerateViewMatrix();

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	virtual void SetViewportsAndScissorRects(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);

	CB_CAMERA_DATA MakeCBData() const;

private:
	BoundingFrustum m_vrustum = {};

	Matrix m_mtxView;
	Matrix m_mtxProjection;

	float m_ffovY = 0.f;
	float m_fAspectRatio = 0.f;
	float m_fNear = 0.f;
	float m_fFar = 0.f;

	Vector3 m_v3Right = {};
	Vector3 m_v3Up = {};
	Vector3 m_v3Look = {};
	Vector3 m_v3LookAtWorld = {};

	Vector3 m_v3Position = {};
	Vector3 m_v3Rotation = {};

	D3D12_VIEWPORT	m_d3dViewport = {};
	D3D12_RECT		m_d3dScissorRect = {};

	ComPtr<ID3D12GraphicsCommandList> m_pd3dViewportScissorrectBindingBundleCommand = nullptr;

	std::shared_ptr<GameObject> m_wpOwner = nullptr;

	std::string m_strBindSemantic = "CAMERA";
};

