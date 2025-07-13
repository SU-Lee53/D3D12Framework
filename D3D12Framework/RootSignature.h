#pragma once

struct SHADER_RESOURCE_BIND_STRUCT {
	std::string strBindSemantics;
	SHADER_RESOURCE_TYPE eResourceType;
	UINT nRegisterSlot;
};

template<>
struct std::hash<SHADER_RESOURCE_BIND_STRUCT> {
	size_t operator()(const SHADER_RESOURCE_BIND_STRUCT& s) {
		return std::hash<std::string>{}(s.strBindSemantics);
	}
};

// ===============================================================================================
// Root Signature
// - 기본적으로는 위 SHADER_RESOURCE_BIND_STRUCT 를 이용해 Descriptor Table 형태로 생성시키도록 함
//		- StructuredBuffer 의 경우에만 ShaderResourceView 형태로 생성함. 안그러면 컴퓨터 맛감
// - 필요한 특수한 경우에는 상속받아 사용
// ===============================================================================================

class RootSignature {
public:
	RootSignature(ComPtr<ID3D12Device14> pd3dDevice) {}
	RootSignature(ComPtr<ID3D12Device14> pd3dDevice, D3D12_ROOT_SIGNATURE_FLAGS flags = ROOT_SIGNATURE_FLAG_DEFAULT) {}
	RootSignature(ComPtr<ID3D12Device14> pd3dDevice, std::span<SHADER_RESOURCE_BIND_STRUCT> binders, D3D12_ROOT_SIGNATURE_FLAGS flags = ROOT_SIGNATURE_FLAG_DEFAULT);
	virtual ~RootSignature() {}

public:
	ID3D12RootSignature* Get() const { return m_pRootSignature.Get(); }
	std::pair<SHADER_RESOURCE_TYPE, UINT> GetBindSlot(std::string_view svBindSemantics);
	std::pair<SHADER_RESOURCE_TYPE, UINT> GetRootParameterIndex(std::string_view svBindSemantics);

private:
	std::vector<CD3DX12_ROOT_PARAMETER> CreateRootParameter(std::span<SHADER_RESOURCE_BIND_STRUCT> binders);

protected:
	ComPtr<ID3D12RootSignature> m_pRootSignature = nullptr;
	D3D12_ROOT_SIGNATURE_FLAGS m_RootSignatureFlags;

	std::array<std::unordered_map<std::string, UINT>, SHADER_RESOURCE_TYPE_COUNT> m_ResourceBindInfos = {};
};
