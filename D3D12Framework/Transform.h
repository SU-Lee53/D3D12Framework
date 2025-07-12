#pragma once
#include "Component.h"

class Transform : public Component {
public:
	Transform(std::shared_ptr<GameObject> pOwner);

public:
	void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> m_pd3dCommandList, UINT nSlot);

private:
	XMFLOAT4X4 m_xmf4x4LocalToModel = {};
	XMFLOAT4X4 m_xmf4x4ModelToWorld = {};
	
	std::string m_strBindSemantic = "TRANSFORM";

};

