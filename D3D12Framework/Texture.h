#pragma once

enum TEXTURE_TYPE {
	TEXTURE_TYPE_UNDEFINED,
	TEXTURE_TYPE_ALBEDO,
	TEXTURE_TYPE_NORMAL,
	TEXTURE_TYPE_SPECULAR,
	TEXTURE_TYPE_METALLIC,
	TEXTURE_TYPE_EMISSION,
	TEXTURE_TYPE_DETAILED_ALBEDO,
	TEXTURE_TYPE_DETAILED_NORAML,
	TEXTURE_TYPE_RENDER_TARGET,

	TEXTURE_TYPE_DIFFUSED = TEXTURE_TYPE_ALBEDO
};

class Texture {
	friend class ResourceManager;

private:
	void Initialize(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, const std::wstring& wstrTexturePath);
	void Initialize(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nWidth, UINT nHeight, bool bAllowRenderTarget);


private:
	void LoadFromDDSFile(ComPtr<ID3D12Device14> pd3dDevice, ID3D12Resource** ppOutResource, const std::wstring& wstrTexturePath, std::unique_ptr<uint8_t[]>& ddsData, std::vector<D3D12_SUBRESOURCE_DATA>& subResources);

	// 왠만하면 쓸일 없도록 합시다
	void LoadFromWICFile(ComPtr<ID3D12Device14> pd3dDevice, ID3D12Resource** ppOutResource, const std::wstring& wstrTexturePath, std::unique_ptr<uint8_t[]>& ddsData, D3D12_SUBRESOURCE_DATA& subResources);

public:
	ComPtr<ID3D12Resource> GetTexture() const { return m_pd3dTexture; }
	DescriptorHeap& GetDescriptorHeap() { return m_d3dDescriptorHeap; }
	const D3D12_SHADER_RESOURCE_VIEW_DESC& GetSRVDesc() const { return m_d3dSRV; }
	TEXTURE_TYPE GetType() const { return m_eType; }

private:
	ComPtr<ID3D12Resource> m_pd3dTexture;
	DescriptorHeap m_d3dDescriptorHeap;
	D3D12_SHADER_RESOURCE_VIEW_DESC m_d3dSRV;

	TEXTURE_TYPE m_eType;

};

