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
// - �⺻�����δ� �� SHADER_RESOURCE_BIND_STRUCT �� �̿��� Descriptor Table ���·� ������Ű���� ��
//		- StructuredBuffer �� ��쿡�� ShaderResourceView ���·� ������. �ȱ׷��� ��ǻ�� ����
// - �ʿ��� Ư���� ��쿡�� ��ӹ޾� ���
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
