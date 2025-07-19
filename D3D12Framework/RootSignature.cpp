#include "stdafx.h"
#include "RootSignature.h"

RootSignature::RootSignature(ComPtr<ID3D12Device14> pd3dDevice, std::span<SHADER_RESOURCE_BIND_INFO> binders, D3D12_ROOT_SIGNATURE_FLAGS flags)
	: m_RootSignatureFlags{ flags }
{
	std::ranges::sort(binders, {}, &SHADER_RESOURCE_BIND_INFO::eParameterType);
	std::ranges::stable_sort(binders, {}, &SHADER_RESOURCE_BIND_INFO::eResourceType);

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

UINT RootSignature::GetBindPosition(SHADER_RESOURCE_TYPE shaderResourceType, ROOT_PARAMETER_TYPE rootParameterType, std::string_view svBindSemantic) const
{
	std::string strSemantic{ svBindSemantic };

	auto it = m_ResourceBindInfos[shaderResourceType][rootParameterType].find(strSemantic);
	if (it != m_ResourceBindInfos[shaderResourceType][rootParameterType].end()) {
		return it->second;
	}
	else {
		__debugbreak();
		return std::numeric_limits<UINT>::max();
	}
}

std::vector<CD3DX12_ROOT_PARAMETER> RootSignature::CreateRootParameter(std::span<SHADER_RESOURCE_BIND_INFO> binders)
{
	auto i = binders.begin();
	UINT nRootParameterIndexBase = 0;

	std::vector<CD3DX12_ROOT_PARAMETER> rootRarameters;
	rootRarameters.reserve(binders.size());
	while (true) {
		i = std::ranges::adjacent_find(i, binders.end(), {}, &SHADER_RESOURCE_BIND_INFO::eResourceType);
		if (i == binders.end()) {
			break;
		}

		auto j = std::ranges::find_if_not(i + 1, binders.end(), [i](const SHADER_RESOURCE_BIND_INFO& binder) {return i->eResourceType == binder.eResourceType; });

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
			for (auto binder = i; binder != j; ++binder) {
				CD3DX12_ROOT_PARAMETER rootParameter;
				switch (binder->eParameterType)
				{
				case ROOT_PARAMETER_TYPE_ROOT_DESCRIPTOR:
					rootParameter.InitAsConstantBufferView(binder->RootDescriptorInfo.nRegisterSlot);
					break;

				case ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
				{
					CD3DX12_DESCRIPTOR_RANGE descRange;
					descRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, binder->DescriptorTableInfo.nDescriptors, binder->DescriptorTableInfo.nRegisterSlot);
					rootParameter.InitAsDescriptorTable(1, &descRange, D3D12_SHADER_VISIBILITY_ALL);
					break;
				}

				default:
					__debugbreak();
					break;
				}
				rootRarameters.push_back(rootParameter);

				m_ResourceBindInfos[binder->eResourceType][binder->eParameterType][binder->strBindSemantic] = nRootParameterIndexBase;
				++nRootParameterIndexBase;
			}

			break;
		}
		case SHADER_RESOURCE_TYPE_TEXTURE:
		{
			for (auto binder = i; binder != j; ++binder) {
				CD3DX12_ROOT_PARAMETER rootParameter;
				switch (binder->eParameterType)
				{
				case ROOT_PARAMETER_TYPE_ROOT_DESCRIPTOR:
					rootParameter.InitAsShaderResourceView(binder->RootDescriptorInfo.nRegisterSlot);
					break;

				case ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
				{
					CD3DX12_DESCRIPTOR_RANGE descRange;
					descRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, binder->DescriptorTableInfo.nDescriptors, binder->DescriptorTableInfo.nRegisterSlot);
					rootParameter.InitAsDescriptorTable(1, &descRange, D3D12_SHADER_VISIBILITY_ALL);
					break;
				}

				default:
					__debugbreak();
					break;
				}
				rootRarameters.push_back(rootParameter);

				m_ResourceBindInfos[binder->eResourceType][binder->eParameterType][binder->strBindSemantic] = nRootParameterIndexBase;
				++nRootParameterIndexBase;
			}

			break;
		}
		case SHADER_RESOURCE_TYPE_STRUCTURED_BUFFER:
		{
			for (auto binder = i; binder != j; ++binder) {
				CD3DX12_ROOT_PARAMETER rootParameter;
				switch (binder->eParameterType)
				{
				case ROOT_PARAMETER_TYPE_ROOT_DESCRIPTOR:
					rootParameter.InitAsShaderResourceView(binder->RootDescriptorInfo.nRegisterSlot);
					break;

				// Structured Buffer 로 들어갈 만큼 큰 버퍼는 Descriptor Table 로 넣으면 컴퓨터 맛탱이 가므로 하지말것
				default:
					__debugbreak();
					break;
				}
				rootRarameters.push_back(rootParameter);

				m_ResourceBindInfos[binder->eResourceType][binder->eParameterType][binder->strBindSemantic] = nRootParameterIndexBase;
				++nRootParameterIndexBase;
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
