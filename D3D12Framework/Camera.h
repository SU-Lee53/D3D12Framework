#pragma once



class Camera {
public:
	Camera();
	~Camera();

	void Update();

public:
	void SetPosition(float x, float y, float z);
	void SetPosition(const Vector3& xmf3Position);

public:
	Matrix GetViewProjectMatrix() const;

	Vector3 GetPosition() const;
	Vector3 GetRight() const;
	Vector3 GetUp() const;
	Vector3 GetLook() const;

public:
	void Rotate(float fPitch, float fYaw, float fRoll);
	void Rotate(const Vector3& pxmf3Axis, float fAngle);

	void GenerateViewMatrix();
	void GenerateViewMatrix(Vector3 xmf3Position, Vector3 xmf3LookAt, Vector3 xmf3Up);
	void RegenerateViewMatrix();

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	virtual void SetViewportsAndScissorRects(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);

private:
	BoundingFrustum m_xmFrustum = {};

	Matrix m_xmf4x4View;
	Matrix m_xmf4x4Projection;

	float m_ffovY = 0.f;
	float m_fAspectRatio = 0.f;
	float m_fNear = 0.f;
	float m_fFar = 0.f;

	Vector3 m_xmf3Right = {};
	Vector3 m_xmf3Up = {};
	Vector3 m_xmf3Look = {};
	Vector3 m_xmf3LookAtWorld = {};

	Vector3 m_xmf3Position = {};
	Vector3 m_xmf3Rotation = {};

	D3D12_VIEWPORT	m_d3dViewport = {};
	D3D12_RECT		m_d3dScissorRect = {};

	ComPtr<ID3D12GraphicsCommandList> m_pd3dViewportScissorrectBindingBundleCommand = nullptr;

	std::shared_ptr<GameObject> m_wpOwner = nullptr;

	std::string m_strBindSemantic = "CAMERA";
};

