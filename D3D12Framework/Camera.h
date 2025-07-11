#pragma once



class Camera {
public:
	Camera();
	~Camera();



private:
	XMFLOAT4X4 m_xmf4x4Transform;
	XMFLOAT4X4 m_xmf4x4View;
	XMFLOAT4X4 m_xmf4x4Projection;

    float m_ffovY = 0.f;
    float m_fAspectRatio = 0.f;
    float m_fNear = 0.f;
    float m_fFar = 0.f;

};

