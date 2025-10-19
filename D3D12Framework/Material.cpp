#include "pch.h"
#include "Material.h"

Material::Material(const MATERIALLOADINFO& materialLoadInfo)
{
	m_MaterialColors.xmf4Ambient = materialLoadInfo.xmf4Ambient;
	m_MaterialColors.xmf4Diffuse = materialLoadInfo.xmf4Diffuse;
	m_MaterialColors.xmf4Specular = materialLoadInfo.xmf4Specular;
	m_MaterialColors.xmf4Emissive = materialLoadInfo.xmf4Emissive;

	m_MaterialColors.fGlossiness = materialLoadInfo.fGlossiness;
	m_MaterialColors.fSmoothness = materialLoadInfo.fSmoothness;
	m_MaterialColors.fSpecularHighlight = materialLoadInfo.fSpecularHighlight;
	m_MaterialColors.fMetallic = materialLoadInfo.fMetallic;
	m_MaterialColors.fGlossyReflection = materialLoadInfo.fGlossyReflection;

	/*
	if(materialLoadInfo.m_strAlbedoMapName.length() != 0){
		m_pTextures.push_back(RESOURCE->CreateTextureFromFile(::ToWSting(materialLoadInfo.m_strAlbedoMapName)));
	}

	if(materialLoadInfo.m_strSpecularMapName.length() != 0){
		m_pTextures.push_back(RESOURCE->CreateTextureFromFile(::ToWSting(materialLoadInfo.m_strSpecularMapName)));
	}

	if(materialLoadInfo.m_strMetallicMapName.length() != 0){
		m_pTextures.push_back(RESOURCE->CreateTextureFromFile(::ToWSting(materialLoadInfo.m_strMetallicMapName)));
	}

	if(materialLoadInfo.m_strNormalMapName.length() != 0){
		m_pTextures.push_back(RESOURCE->CreateTextureFromFile(::ToWSting(materialLoadInfo.m_strNormalMapName)));
	}

	if(materialLoadInfo.m_strEmissionMapName.length() != 0){
		m_pTextures.push_back(RESOURCE->CreateTextureFromFile(::ToWSting(materialLoadInfo.m_strEmissionMapName)));
	}

	if(materialLoadInfo.m_strDetailAlbedoMapName.length() != 0){
		m_pTextures.push_back(RESOURCE->CreateTextureFromFile(::ToWSting(materialLoadInfo.m_strDetailAlbedoMapName)));
	}

	if (materialLoadInfo.m_strDetailNormalMapName.length() != 0) {
		m_pTextures.push_back(RESOURCE->CreateTextureFromFile(::ToWSting(materialLoadInfo.m_strDetailNormalMapName)));
	}
	*/

}

Material::~Material()
{
}

void Material::SetShader(std::shared_ptr<Shader> pShader)
{
	m_pShader = pShader;
}

std::shared_ptr<Texture> Material::GetTexture(int nIndex)
{
	assert(nIndex < m_pTextures.size());
	return m_pTextures[nIndex];
}

//////////////////////////////////////////////////////////////////////////////////
// DiffusedMaterial

DiffusedMaterial::DiffusedMaterial(const MATERIALLOADINFO& materialLoadInfo)
	:Material(materialLoadInfo)
{
}

DiffusedMaterial::~DiffusedMaterial()
{
}

void DiffusedMaterial::UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* dataForBind)
{
}

void DiffusedMaterial::UpdateShaderVariables(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, DescriptorHandle& descHandle)
{
}

//////////////////////////////////////////////////////////////////////////////////
// TexturedMaterial

TexturedMaterial::TexturedMaterial(const MATERIALLOADINFO& materialLoadInfo)
	:Material(materialLoadInfo)
{
	m_pTextures.resize(1);
	m_pTextures[0] = TEXTURE->CreateTextureFromFile(::StringToWString(materialLoadInfo.m_strAlbedoMapName));	// Diffused
}

TexturedMaterial::~TexturedMaterial()
{
}

void TexturedMaterial::UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* dataForBind)
{
}

void TexturedMaterial::UpdateShaderVariables(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, DescriptorHandle& descHandle)
{
	pd3dDevice->CopyDescriptorsSimple(1, descHandle.cpuHandle, m_pTextures[0]->GetDescriptorHeap().GetDescriptorHandleFromHeapStart().cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descHandle.cpuHandle.ptr += D3DCore::g_nCBVSRVDescriptorIncrementSize;

	pd3dCommandList->SetGraphicsRootDescriptorTable(4, descHandle.gpuHandle);
	descHandle.gpuHandle.Offset(1, D3DCore::g_nCBVSRVDescriptorIncrementSize);

}

//////////////////////////////////////////////////////////////////////////////////
// TexturedNormalMaterial

TexturedNormalMaterial::TexturedNormalMaterial(const MATERIALLOADINFO& materialLoadInfo)
	:Material(materialLoadInfo)
{
	m_pTextures.resize(2);
	m_pTextures[0] = TEXTURE->CreateTextureFromFile(::StringToWString(materialLoadInfo.m_strAlbedoMapName));	// Diffused
	m_pTextures[1] = TEXTURE->CreateTextureFromFile(::StringToWString(materialLoadInfo.m_strNormalMapName));	// Normal
}

TexturedNormalMaterial::~TexturedNormalMaterial()
{
}

void TexturedNormalMaterial::UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* dataForBind)
{
	// 일단 dataForBind 에 Root Parameter Index 가 왔다고 가정
	// 나중에 변경 필요하면 바꿀것

	int nRootParameterIndex1 = *((int*)dataForBind);
	int nRootParameterIndex2 = *((int*)dataForBind + 1);

	pd3dCommandList->SetGraphicsRootShaderResourceView(nRootParameterIndex1, m_pTextures[0]->GetTexture().GetGPUAddress());
	pd3dCommandList->SetGraphicsRootShaderResourceView(nRootParameterIndex2, m_pTextures[1]->GetTexture().GetGPUAddress());

}

void TexturedNormalMaterial::UpdateShaderVariables(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, DescriptorHandle& descHandle)
{
	pd3dDevice->CopyDescriptorsSimple(1, descHandle.cpuHandle, m_pTextures[0]->GetDescriptorHeap().GetDescriptorHandleFromHeapStart().cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descHandle.cpuHandle.ptr += D3DCore::g_nCBVSRVDescriptorIncrementSize;
	pd3dDevice->CopyDescriptorsSimple(1, descHandle.cpuHandle, m_pTextures[1]->GetDescriptorHeap().GetDescriptorHandleFromHeapStart().cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	pd3dCommandList->SetGraphicsRootDescriptorTable(4, descHandle.gpuHandle);
	descHandle.gpuHandle.Offset(1, D3DCore::g_nCBVSRVDescriptorIncrementSize);
}
