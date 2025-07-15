#pragma once

struct ROOT_DESCRIPTOR_INFO {
	UINT nRegisterSlot;
};

struct DESCRIPTOR_TABLE_INFO {
	UINT nRegisterSlot;
	UINT nDescriptors;
};

/// <summary>
/// !!! BindSemantic �� �����ϸ� 15���ڸ� ���� ���� !!!
/// SSO �� ������ ���̸� ����� hash ���� ������ ����� ����
/// </summary>
struct SHADER_RESOURCE_BIND_INFO {
	std::string strBindSemantic;
	SHADER_RESOURCE_TYPE eResourceType;
	ROOT_PARAMETER_TYPE eParameterType;
	union {
		ROOT_DESCRIPTOR_INFO RootDescriptorInfo;
		DESCRIPTOR_TABLE_INFO DescriptorTableInfo;
	};

	inline void InitAsRootDescriptor(
		std::string_view svBindSemantic,
		SHADER_RESOURCE_TYPE resourceType,
		UINT nRegisterSlot)
	{
		strBindSemantic = std::string{ svBindSemantic };
		eResourceType = resourceType;
		RootDescriptorInfo.nRegisterSlot = nRegisterSlot;
	}

	inline void InitAsDescriptorTable(
		std::string_view svBindSemantic,
		SHADER_RESOURCE_TYPE resourceType,
		UINT nRegisterSlot,
		UINT nDescriptors)
	{
		strBindSemantic = std::string{ svBindSemantic };
		eResourceType = resourceType;
		DescriptorTableInfo.nRegisterSlot = nRegisterSlot;
		DescriptorTableInfo.nDescriptors = nDescriptors;
	}
};

template<>
struct std::hash<SHADER_RESOURCE_BIND_INFO> {
	size_t operator()(const SHADER_RESOURCE_BIND_INFO& s) {
		return std::hash<std::string>{}(s.strBindSemantic);
	}
};

class RootSignature {
public:
	RootSignature(ComPtr<ID3D12Device14> pd3dDevice) {}
	RootSignature(ComPtr<ID3D12Device14> pd3dDevice, D3D12_ROOT_SIGNATURE_FLAGS flags = ROOT_SIGNATURE_FLAG_DEFAULT) {}
	RootSignature(ComPtr<ID3D12Device14> pd3dDevice, std::span<SHADER_RESOURCE_BIND_INFO> binders, D3D12_ROOT_SIGNATURE_FLAGS flags = ROOT_SIGNATURE_FLAG_DEFAULT);
	virtual ~RootSignature() {}

public:
	ID3D12RootSignature* Get() const { return m_pRootSignature.Get(); }

	UINT GetBindPosition(
		SHADER_RESOURCE_TYPE shaderResourceType,
		ROOT_PARAMETER_TYPE rootParameterType,
		std::string_view svBindSemantic) const;

private:
	std::vector<CD3DX12_ROOT_PARAMETER> CreateRootParameter(std::span<SHADER_RESOURCE_BIND_INFO> binders);

protected:
	ComPtr<ID3D12RootSignature> m_pRootSignature = nullptr;
	D3D12_ROOT_SIGNATURE_FLAGS m_RootSignatureFlags;

	/*
		std::array                     std::array                       std::unordered_map
		[  CONSTANT_BUFFER  ] -------> [ ROOT_DESCRIPTOR  ] ----------> [ { Bind Semantic } - { Root Parameter Index } ] [ { Bind Semantic } - { Root Parameter Index } ] ...
		[      TEXTURE      ] ----+    [ DESCRIPTOR_TABLE ] ----------> [ { Bind Semantic } - { Root Parameter Index } ] [ { Bind Semantic } - { Root Parameter Index } ] ...
		[ STRUCTURED_BUFFER ] -+  |
							   |  |    std::array                       std::unordered_map
		                       |  +--> [ ROOT_DESCRIPTOR  ] ----------> [ { Bind Semantic } - { Root Parameter Index } ] [ { Bind Semantic } - { Root Parameter Index } ] ...
							   |       [ DESCRIPTOR_TABLE ] ----------> [ { Bind Semantic } - { Root Parameter Index } ] [ { Bind Semantic } - { Root Parameter Index } ] ...
							   |
		                       |       std::array                       std::unordered_map
		                       +-----> [ ROOT_DESCRIPTOR  ] ----------> [ { Bind Semantic } - { Root Parameter Index } ] [ { Bind Semantic } - { Root Parameter Index } ] ...
							           [ DESCRIPTOR_TABLE ] ----------> [ { Bind Semantic } - { Root Parameter Index } ] [ { Bind Semantic } - { Root Parameter Index } ] ...
	
		- O(1) �ȿ� ���ҽ� ���ε� ��ġ (Root Parameter Index)�� ã�� �� ����
		- �츮�� Root Signature �� ����� ���� ���� ���ε� ��ġ�� ������� �ʾƵ� ��
	*/

	std::array<std::array<std::unordered_map<std::string, UINT>, ROOT_PARAMETER_TYPE_COUNT>, SHADER_RESOURCE_TYPE_COUNT> m_ResourceBindInfos = {};


};
