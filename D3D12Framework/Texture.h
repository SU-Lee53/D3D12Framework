#pragma once

enum TEXTURE_TYPE {
	TEXTURE_TYPE_ALBEDO,
	TEXTURE_TYPE_NORMAL,
	TEXTURE_TYPE_SPECULAR,
	TEXTURE_TYPE_METALLIC,
	TEXTURE_TYPE_EMISSION,
	TEXTURE_TYPE_DETAILED_ALBEDO,
	TEXTURE_TYPE_DETAILED_NORAML,
	TEXTURE_TYPE_RENDER_TARGET,

	TEXTURE_TYPE_DIFFUSED = TEXTURE_TYPE_ALBEDO,

	TEXTURE_TYPE_UNDEFINED
};

class Texture {
	friend class TextureManager;

private:
	void Initialize(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, const std::wstring& wstrTexturePath);

public:
	void StateTransition(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, D3D12_RESOURCE_STATES d3dAfterState);

public:
	ShaderResource GetTexture() const { return m_pTexResource; }
	DescriptorHeap& GetDescriptorHeap() { return m_d3dDescriptorHeap; }
	const D3D12_SHADER_RESOURCE_VIEW_DESC& GetSRVDesc() const { return m_d3dSRV; }
	UINT GetType() const { return m_eType; }

protected:
	ShaderResource m_pTexResource;
	DescriptorHeap m_d3dDescriptorHeap;
	D3D12_SHADER_RESOURCE_VIEW_DESC m_d3dSRV;
	UINT m_eType;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
// RenderTargetTexture

class RenderTargetTexture : public Texture {
private:
	void Initialize(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nWidth, UINT nHeight);

private:
	DescriptorHeap m_d3dRTVDescriptorHeap;

};