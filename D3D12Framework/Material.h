#pragma once
#include "Texture.h"
#include "Shader.h"

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

class Material {
public:
	Material(const MATERIALLOADINFO& materialLoadInfo);
	virtual ~Material();

public:
	const MaterialColors& GetMaterialColors() const { return m_MaterialColors; }
	const std::shared_ptr<Shader>& GetShader() const { return m_pShader; }
	void SetShader(std::shared_ptr<Shader> pShader);

	std::shared_ptr<Texture> GetTexture(int nIndex);


public:
	virtual void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* dataForBind) = 0;
	virtual void UpdateShaderVariables(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, DescriptorHandle& descHandle) = 0;

protected:
	MaterialColors m_MaterialColors{};
	std::vector<std::shared_ptr<Texture>> m_pTextures;

	std::shared_ptr<Shader> m_pShader;

};

//////////////////////////////////////////////////////////////////////////////////
// DiffusedMaterial

class DiffusedMaterial : public Material {
public:
	DiffusedMaterial(const MATERIALLOADINFO& materialLoadInfo);
	virtual ~DiffusedMaterial();

	virtual void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* dataForBind) override;
	virtual void UpdateShaderVariables(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, DescriptorHandle& descHandle) override;


};

//////////////////////////////////////////////////////////////////////////////////
// TexturedMaterial

class TexturedMaterial : public Material {
public:
	TexturedMaterial(const MATERIALLOADINFO& materialLoadInfo);
	virtual ~TexturedMaterial();

	virtual void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* dataForBind) override;
	virtual void UpdateShaderVariables(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, DescriptorHandle& descHandle) override;

};

//////////////////////////////////////////////////////////////////////////////////
// TexturedNormalMaterial

class TexturedNormalMaterial : public Material {
public:
	TexturedNormalMaterial(const MATERIALLOADINFO& materialLoadInfo);
	virtual ~TexturedNormalMaterial();

	virtual void UpdateShaderVariables(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, void* dataForBind) override;
	virtual void UpdateShaderVariables(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, DescriptorHandle& descHandle) override;

};
