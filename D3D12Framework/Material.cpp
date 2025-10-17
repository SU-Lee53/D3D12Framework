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

	m_MaterialColors.eType = materialLoadInfo.eType;

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

}
