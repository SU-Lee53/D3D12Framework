#pragma once

class Texture;
class Shader;

struct MATERIALLOADINFO {
	XMFLOAT4		xmf4Ambient;
	XMFLOAT4		xmf4Diffuse;
	XMFLOAT4		xmf4Specular; //(r,g,b,a=power)
	XMFLOAT4		xmf4Emissive;

	float			fGlossiness = 0.0f;
	float			fSmoothness = 0.0f;
	float			fSpecularHighlight = 0.0f;
	float			fMetallic = 0.0f;
	float			fGlossyReflection = 0.0f;

	UINT			eType = 0x00;

	std::string		m_strAlbedoMapName;
	std::string		m_strSpecularMapName;
	std::string		m_strMetallicMapName;
	std::string		m_strNormalMapName;
	std::string		m_strEmissionMapName;
	std::string		m_strDetailAlbedoMapName;
	std::string		m_strDetailNormalMapName;
}; 

struct MaterialColors {
	XMFLOAT4		xmf4Ambient;
	XMFLOAT4		xmf4Diffuse;
	XMFLOAT4		xmf4Specular; //(r,g,b,a=power)
	XMFLOAT4		xmf4Emissive;

	float			fGlossiness = 0.0f;
	float			fSmoothness = 0.0f;
	float			fSpecularHighlight = 0.0f;
	float			fMetallic = 0.0f;
	float			fGlossyReflection = 0.0f;
};

class Material {
public:
	Material(const MATERIALLOADINFO& materialLoadInfo);


private:
	MaterialColors m_MaterialColors{};
	std::vector<std::shared_ptr<Texture>> m_pTextures;

	std::shared_ptr<Shader> m_pShader;

};

