#include "stdafx.h"
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

//////////////////////////////////////////////////////////////////////////////////
//

DiffusedMaterial::DiffusedMaterial(const MATERIALLOADINFO& materialLoadInfo)
	:Material(materialLoadInfo)
{
}

DiffusedMaterial::~DiffusedMaterial()
{
}

void DiffusedMaterial::UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* dataForBind)
{
	ConstantBuffer& cbuffer = RESOURCE->AllocCBuffer();
	cbuffer.WriteData(m_MaterialColors, 0);

}

TexturedMaterial::TexturedMaterial(const MATERIALLOADINFO& materialLoadInfo)
	:Material(materialLoadInfo)
{
	m_pDiffusedTexutre = RESOURCE->CreateTextureFromFile(::ToWSting(materialLoadInfo.m_strAlbedoMapName));
	m_pNormalTexture = RESOURCE->CreateTextureFromFile(::ToWSting(materialLoadInfo.m_strNormalMapName));
}

TexturedMaterial::~TexturedMaterial()
{
}

void TexturedMaterial::UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* dataForBind)
{
	// 일단 dataForBind 에 Root Parameter Index 가 왔다고 가정
	// 나중에 변경 필요하면 바꿀것

	int nRootParameterIndex1 = *((int*)dataForBind);
	int nRootParameterIndex2 = *((int*)dataForBind + 1);

	pd3dCommandList->SetGraphicsRootShaderResourceView(nRootParameterIndex1, m_pDiffusedTexture->GetTexture()->GetGPUVirtualAddress());
	pd3dCommandList->SetGraphicsRootShaderResourceView(nRootParameterIndex2, m_pNormalTexture->GetTexture()->GetGPUVirtualAddress());

}
