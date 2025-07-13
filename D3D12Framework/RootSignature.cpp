#include "stdafx.h"
#include "RootSignature.h"

RootSignature::RootSignature(ComPtr<ID3D12Device14> pd3dDevice, std::span<SHADER_RESOURCE_BIND_STRUCT> binders, D3D12_ROOT_SIGNATURE_FLAGS flags)
	: m_RootSignatureFlags{ flags }
{
	std::ranges::sort(binders, {}, &SHADER_RESOURCE_BIND_STRUCT::eResourceType);
	std::vector<CD3DX12_ROOT_PARAMETER> rootParameters = CreateRootParameter(binders);

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc;
	samplerDesc.Init(0);

	CD3DX12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	d3dRootSignatureDesc.Init(rootParameters.size(), rootParameters.data(), 1, &samplerDesc, m_RootSignatureFlags);

	ComPtr<ID3DBlob> pd3dSignatureBlob = nullptr;
	ComPtr<ID3DBlob> pd3dErrorBlob = nullptr;

	HRESULT hr = ::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	if (FAILED(hr)) {
		if (pd3dErrorBlob) {
			OutputDebugStringA((char*)pd3dErrorBlob->GetBufferPointer());
		}
	}

	hr = pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(m_pRootSignature.GetAddressOf()));

	if (FAILED(hr)) __debugbreak();
}

std::pair<SHADER_RESOURCE_TYPE, UINT> RootSignature::GetBindSlot(std::string_view svBindSemantics)
{
	std::string finder{ svBindSemantics };
	for (int resourceType = 0; resourceType < m_ResourceBindInfos.size(); ++resourceType ) {
		if (auto it = m_ResourceBindInfos[resourceType].find(finder); it != m_ResourceBindInfos[resourceType].end()) {
			return { (SHADER_RESOURCE_TYPE)resourceType, it->second };
		}
	}

	return { SHADER_RESOURCE_TYPE_UNDEFINED, std::numeric_limits<UINT>::max() };
}

std::pair<SHADER_RESOURCE_TYPE, UINT> RootSignature::GetRootParameterIndex(std::string_view svBindSemantics)
{
	UINT nSlot = 0;
	std::string finder{ svBindSemantics };
	for (int resourceType = 0; resourceType < m_ResourceBindInfos.size(); ++resourceType) {
		if (m_ResourceBindInfos[resourceType].contains(finder)) {
			return { (SHADER_RESOURCE_TYPE)resourceType, nSlot };
		}

		if (!m_ResourceBindInfos[resourceType].empty()) {
			++nSlot;
		}
	}

	return { SHADER_RESOURCE_TYPE_UNDEFINED, std::numeric_limits<UINT>::max() };
}

std::vector<CD3DX12_ROOT_PARAMETER> RootSignature::CreateRootParameter(std::span<SHADER_RESOURCE_BIND_STRUCT> binders)
{
	auto i = binders.begin();
	UINT srvRegisterBase = 0;
	std::vector<CD3DX12_ROOT_PARAMETER> rootRarameters;
	while (true) {
		i = std::ranges::adjacent_find(i, binders.end(), {}, &SHADER_RESOURCE_BIND_STRUCT::eResourceType);
		if (i == binders.end()) {
			break;
		}

		auto j = std::ranges::find_if_not(i + 1, binders.end(), [i](const SHADER_RESOURCE_BIND_STRUCT& binder) {return i->eResourceType == binder.eResourceType; });

		// 예시)
		// [CB][CB][CB][CB][SB][SB][SB][TEX][TEX][TEX][TEX][TEX] <- 여기서 ConstantBuffer 를 찾는다면
		//  ↑               ↑
		//  i               j  -> std::distance(i,j) == 4
		// [i, j) 범위는 같은 종류의 RESOURCE 임

		size_t sameRange = std::ranges::distance(i, j);
		SHADER_RESOURCE_TYPE resType = i->eResourceType;
		switch (resType)
		{
		case SHADER_RESOURCE_TYPE_CONSTANT_BUFFER:
		{
			CD3DX12_DESCRIPTOR_RANGE descRange;
			descRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, sameRange, 0);

			CD3DX12_ROOT_PARAMETER rootParameter;
			rootParameter.InitAsDescriptorTable(sameRange, &descRange, D3D12_SHADER_VISIBILITY_ALL);
			rootRarameters.push_back(rootParameter);

			size_t idx = 0;
			for (const SHADER_RESOURCE_BIND_STRUCT& binder : std::ranges::subrange{ i,j }) {
				m_ResourceBindInfos[SHADER_RESOURCE_TYPE_CONSTANT_BUFFER][binder.strBindSemantics] = idx;
				++idx;
			}

			break;
		}
		case SHADER_RESOURCE_TYPE_TEXTURE:
		{
			CD3DX12_DESCRIPTOR_RANGE descRange;
			descRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, sameRange, 0);

			CD3DX12_ROOT_PARAMETER rootParameter;
			rootParameter.InitAsDescriptorTable(sameRange, &descRange, D3D12_SHADER_VISIBILITY_ALL);
			rootRarameters.push_back(rootParameter);

			for (const SHADER_RESOURCE_BIND_STRUCT& binder : std::ranges::subrange{ i,j }) {
				m_ResourceBindInfos[SHADER_RESOURCE_TYPE_TEXTURE][binder.strBindSemantics] = srvRegisterBase;
				++srvRegisterBase;
			}

			break;
		}
		case SHADER_RESOURCE_TYPE_STRUCTURED_BUFFER:
		{
			for (int i = srvRegisterBase; i < srvRegisterBase + sameRange; ++i) {
				CD3DX12_ROOT_PARAMETER rootParameter;
				rootParameter.InitAsShaderResourceView(i);
				rootRarameters.push_back(rootParameter);
			}

			for (const SHADER_RESOURCE_BIND_STRUCT& binder : std::ranges::subrange{ i,j }) {
				m_ResourceBindInfos[SHADER_RESOURCE_TYPE_TEXTURE][binder.strBindSemantics] = srvRegisterBase;
				++srvRegisterBase;
			}

			break;
		}
		default:
			break;
		}

		// reset iterator
		i = j;
	}

	return rootRarameters;
}
