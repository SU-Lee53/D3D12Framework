#pragma once
#include "Texture.h"

//#define ASSERT_WHEN_TEXTURE_NAME_IS_BLANK

class TextureManager {
public:
	TextureManager(ComPtr<ID3D12Device14> pDevice);
	~TextureManager();

public:
	std::shared_ptr<Texture> Load(const std::string& strTextureName, const std::wstring& wstrTexturePath);
	std::shared_ptr<Texture> Get(const std::string& strTextureName);

public:
	std::shared_ptr<Texture> CreateTextureFromFile(const std::wstring& wstrTexturePath);
	std::shared_ptr<Texture> CreateBlankTexture() = delete;	// 아직 미구현
	std::shared_ptr<Texture> CreateCubeMapTexture(const std::span<std::wstring>& wstrTexturePaths) = delete;	// 아직 미구현

private:
	void LoadFromDDSFile(ID3D12Resource** ppOutResource, const std::wstring& wstrTexturePath, std::unique_ptr<uint8_t[]>& ddsData, std::vector<D3D12_SUBRESOURCE_DATA>& subResources);

	// 왠만하면 쓸일 없도록 합시다
	void LoadFromWICFile(ID3D12Resource** ppOutResource, const std::wstring& wstrTexturePath, std::unique_ptr<uint8_t[]>& ddsData, D3D12_SUBRESOURCE_DATA& subResources);

private:
	// Texture Pool
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_pTexturePool;


public:
	void ResetCommandList();

private:
	void CreateCommandList();
	void CreateFence();
	void Fence();
	void WaitForGPUComplete();

	void ExcuteCommandList();

private:
	ComPtr<ID3D12Device14>				m_pd3dDevice = nullptr;		// Reference to D3DCore::m_pd3dDevice
	ComPtr<ID3D12GraphicsCommandList>	m_pd3dCommandList = nullptr;
	ComPtr<ID3D12CommandAllocator>		m_pd3dCommandAllocator = nullptr;
	ComPtr<ID3D12CommandQueue>			m_pd3dCommandQueue = nullptr;

	ComPtr<ID3D12Fence>		m_pd3dFence = nullptr;
	HANDLE					m_hFenceEvent = nullptr;
	UINT64					m_nFenceValue = 0;

};

