#pragma once



class Camera {
public:
	Camera();
	~Camera();

	void Update();

public:
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& xmf3Position);

public:
	void Rotate(float fPitch, float fYaw, float fRoll);
	void Rotate(const XMFLOAT3& pxmf3Axis, float fAngle);

	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);
	void RegenerateViewMatrix();

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);

public:
	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetRight() const;
	XMFLOAT3 GetUp() const;
	XMFLOAT3 GetLook() const;

public:
	void UpdateShaderVariables(ConstantBuffer& CBuffer);

private:
	XMFLOAT4X4 m_xmf4x4View;
	XMFLOAT4X4 m_xmf4x4Projection;

    float m_ffovY = 0.f;
    float m_fAspectRatio = 0.f;
    float m_fNear = 0.f;
    float m_fFar = 0.f;

	D3D12_VIEWPORT	m_d3dViewport = {};
	D3D12_RECT		m_d3dScissorRect = {};

	std::shared_ptr<GameObject> m_pOwner = nullptr;

	std::string m_strBindSemantic = "CAMERA";
};

