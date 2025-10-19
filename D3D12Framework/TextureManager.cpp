#include "pch.h"
#include "TextureManager.h"

TextureManager::TextureManager(ComPtr<ID3D12Device14> pDevice)
	: m_pd3dDevice{ pDevice }
{
	CreateCommandList();
	CreateFence();
}

TextureManager::~TextureManager()
{
}

std::shared_ptr<Texture> TextureManager::Load(const std::string& strTextureName, const std::wstring& wstrTexturePath)
{
	auto it = m_pTexturePool.find(strTextureName);
	if (it == m_pTexturePool.end()) {
		std::shared_ptr<Texture> pTexture = CreateTextureFromFile(wstrTexturePath);
		m_pTexturePool[strTextureName] = pTexture;
	}

	return m_pTexturePool[strTextureName];
}

std::shared_ptr<Texture> TextureManager::Get(const std::string& strTextureName)
{
	auto it = m_pTexturePool.find(strTextureName);
	if (it != m_pTexturePool.end()) {
		return it->second;
	}

	return nullptr;
}

std::shared_ptr<Texture> TextureManager::CreateTextureFromFile(const std::wstring& wstrTexturePath)
{
	namespace fs = std::filesystem;

	fs::path texPath{ wstrTexturePath };
	if (!fs::exists(texPath)) {
		OutputDebugStringA(std::format("{} - {} : {}", __FILE__, __LINE__, "Texture file not exist").c_str());
		return nullptr;
	}

	std::shared_ptr<Texture> pTexture = std::make_shared<Texture>();

	std::unique_ptr<uint8_t[]> ddsData = nullptr;
	std::vector<D3D12_SUBRESOURCE_DATA> subresources(0);
	if (texPath.extension().string() == ".dds" || texPath.extension().string() == ".DDS") {
		LoadFromDDSFile(pTexture->m_pTexResource.pResource.GetAddressOf(), wstrTexturePath, ddsData, subresources);
	}
	else {
		LoadFromWICFile(pTexture->m_pTexResource.pResource.GetAddressOf(), wstrTexturePath, ddsData, subresources[0]);
	}

	D3D12_RESOURCE_DESC d3dTextureResourceDesc = pTexture->m_pTexResource.pResource->GetDesc();
	UINT nSubResources = (UINT)subresources.size();
	UINT64 nBytes = GetRequiredIntermediateSize(pTexture->m_pTexResource.pResource.Get(), 0, nSubResources);
	//	UINT nSubResources = d3dTextureResourceDesc.DepthOrArraySize * d3dTextureResourceDesc.MipLevels;
	//	UINT64 nBytes = 0;
	//	pd3dDevice->GetCopyableFootprints(&d3dTextureResourceDesc, 0, nSubResources, 0, NULL, NULL, NULL, &nBytes);

	ComPtr<ID3D12Resource> pd3dUploadBuffer = nullptr;
	m_pd3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(nBytes),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(pd3dUploadBuffer.GetAddressOf())
	);

	// BinaryResource -> Upload Buffer -> Texture Buffer
	ResetCommandList();
	{
		pTexture->m_pTexResource.StateTransition(m_pd3dCommandList, D3D12_RESOURCE_STATE_COPY_DEST);
		{
			::UpdateSubresources(m_pd3dCommandList.Get(), pTexture->m_pTexResource.pResource.Get(), pd3dUploadBuffer.Get(), 0, 0, nSubResources, subresources.data());
		}
		pTexture->m_pTexResource.StateTransition(m_pd3dCommandList, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);
	}
	ExcuteCommandList();
	Fence();
	WaitForGPUComplete();

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	{
		heapDesc.NumDescriptors = 1;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
	}
	pTexture->m_d3dDescriptorHeap.Initialize(m_pd3dDevice, heapDesc);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	{
		srvDesc.Format = d3dTextureResourceDesc.Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = d3dTextureResourceDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.PlaneSlice = 0;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	}

	m_pd3dDevice->CreateShaderResourceView(pTexture->m_pTexResource.pResource.Get(), &srvDesc, pTexture->m_d3dDescriptorHeap.GetDescriptorHandleFromHeapStart().cpuHandle);

	return pTexture;
}

void TextureManager::LoadFromDDSFile(ID3D12Resource** ppOutResource, const std::wstring& wstrTexturePath, std::unique_ptr<uint8_t[]>& ddsData, std::vector<D3D12_SUBRESOURCE_DATA>& subResources)
{
	HRESULT hr;
	DDS_ALPHA_MODE ddsAlphaMode = DDS_ALPHA_MODE_UNKNOWN;
	bool bIsCubeMap = false;

	hr = ::LoadDDSTextureFromFileEx(
		m_pd3dDevice.Get(),
		wstrTexturePath.c_str(),
		0,
		D3D12_RESOURCE_FLAG_NONE,
		DDS_LOADER_DEFAULT,
		ppOutResource,
		ddsData,
		subResources,
		&ddsAlphaMode,
		&bIsCubeMap
	);

	if (FAILED(hr)) {
		OutputDebugStringA(std::format("{} - {} : {}", __FILE__, __LINE__, "Failed To Load DDS File").c_str());
		return;
	}

}

void TextureManager::LoadFromWICFile(ID3D12Resource** ppOutResource, const std::wstring& wstrTexturePath, std::unique_ptr<uint8_t[]>& ddsData, D3D12_SUBRESOURCE_DATA& subResource)
{
	HRESULT hr;

	hr = ::LoadWICTextureFromFileEx(
		m_pd3dDevice.Get(),
		wstrTexturePath.c_str(),
		0,
		D3D12_RESOURCE_FLAG_NONE,
		WIC_LOADER_DEFAULT,
		ppOutResource,
		ddsData,
		subResource
	);

	if (FAILED(hr)) {
		OutputDebugStringA(std::format("{} - {} : {}", __FILE__, __LINE__, "Failed To Load WIC File").c_str());
		return;
	}
}

#pragma region D3D
void TextureManager::ResetCommandList()
{
	HRESULT hr;
	hr = m_pd3dCommandAllocator->Reset();
	hr = m_pd3dCommandList->Reset(m_pd3dCommandAllocator.Get(), NULL);
	if (FAILED(hr)) {
		SHOW_ERROR("Faied to reset CommandList");
		__debugbreak();
	}
}


void TextureManager::CreateCommandList()
{
	HRESULT hr{};

	// Create Command Queue
	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc{};
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	{
		d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	}
	hr = m_pd3dDevice->CreateCommandQueue(&d3dCommandQueueDesc, IID_PPV_ARGS(m_pd3dCommandQueue.GetAddressOf()));
	if (FAILED(hr)) {
		SHOW_ERROR("Failed to create CommandQueue");
	}

	// Create Command Allocator
	hr = m_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_pd3dCommandAllocator.GetAddressOf()));
	if (FAILED(hr)) {
		SHOW_ERROR("Failed to create CommandAllocator");
	}

	// Create Command List
	hr = m_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pd3dCommandAllocator.Get(), NULL, IID_PPV_ARGS(m_pd3dCommandList.GetAddressOf()));
	if (FAILED(hr)) {
		SHOW_ERROR("Failed to create CommandList");
	}

	// Close Command List(default is opened)
	hr = m_pd3dCommandList->Close();
}

void TextureManager::CreateFence()
{
	HRESULT hr{};

	hr = m_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_pd3dFence.GetAddressOf()));
	if (FAILED(hr)) {
		SHOW_ERROR("Failed to create fence");
	}

	m_hFenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

void TextureManager::ExcuteCommandList()
{
	m_pd3dCommandList->Close();

	ID3D12CommandList* ppCommandLists[] = { m_pd3dCommandList.Get() };
	m_pd3dCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	WaitForGPUComplete();
}

void TextureManager::Fence()
{
	m_nFenceValue++;
	m_pd3dCommandQueue->Signal(m_pd3dFence.Get(), m_nFenceValue);
}

void TextureManager::WaitForGPUComplete()
{
	const UINT64 expectedFenceValue = m_nFenceValue;

	if (m_pd3dFence->GetCompletedValue() < expectedFenceValue)
	{
		m_pd3dFence->SetEventOnCompletion(expectedFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}


#pragma endregion D3D