#include "stdafx.h"
#include "Texture.h"

void Texture::Initialize(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, const std::wstring& wstrTexturePath)
{
	namespace fs = std::filesystem;

	fs::path texPath{ wstrTexturePath };
	if (!fs::exists(texPath)) {
		OutputDebugStringA(std::format("{} - {} : {}", __FILE__, __LINE__, "Texture file not exist").c_str());
		return;
	}

	std::unique_ptr<uint8_t[]> ddsData = nullptr;
	std::vector<D3D12_SUBRESOURCE_DATA> subresources(0);
	if (texPath.extension().string() == ".dds" || texPath.extension().string() == ".DDS") {
		LoadFromDDSFile(pd3dDevice, m_pd3dTexture.GetAddressOf(), wstrTexturePath, ddsData, subresources);
	}
	else {
		LoadFromWICFile(pd3dDevice, m_pd3dTexture.GetAddressOf(), wstrTexturePath, ddsData, subresources[0]);
	}

	D3D12_RESOURCE_DESC d3dTextureResourceDesc = m_pd3dTexture->GetDesc();
	UINT nSubResources = (UINT)subresources.size();
	UINT64 nBytes = GetRequiredIntermediateSize(m_pd3dTexture.Get(), 0, nSubResources);
	//	UINT nSubResources = d3dTextureResourceDesc.DepthOrArraySize * d3dTextureResourceDesc.MipLevels;
	//	UINT64 nBytes = 0;
	//	pd3dDevice->GetCopyableFootprints(&d3dTextureResourceDesc, 0, nSubResources, 0, NULL, NULL, NULL, &nBytes);

	ComPtr<ID3D12Resource> pd3dUploadBuffer = nullptr;
	pd3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(nBytes),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(pd3dUploadBuffer.GetAddressOf())
	);

	// BinaryResource -> Upload Buffer -> Texture Buffer
	pd3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pd3dTexture.Get(), D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES));
	{
		::UpdateSubresources(pd3dCommandList.Get(), m_pd3dTexture.Get(), pd3dUploadBuffer.Get(), 0, 0, nSubResources, subresources.data());
	}
	pd3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pd3dTexture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES));

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	{
		heapDesc.NumDescriptors = 1;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
	}
	m_d3dDescriptorHeap.Initialize(pd3dDevice, heapDesc);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	{
		srvDesc.Format = d3dTextureResourceDesc.Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = d3dTextureResourceDesc.MipLevels;
	}

	pd3dDevice->CreateShaderResourceView(m_pd3dTexture.Get(), &srvDesc, m_d3dDescriptorHeap[0].cpuHandle);


}

void Texture::Initialize(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nWidth, UINT nHeight, bool bAllowRenderTarget)
{
}

void Texture::LoadFromDDSFile(ComPtr<ID3D12Device14> pd3dDevice, ID3D12Resource** ppOutResource, const std::wstring& wstrTexturePath, std::unique_ptr<uint8_t[]>& ddsData, std::vector<D3D12_SUBRESOURCE_DATA>& subResources)
{
	HRESULT hr;
	DDS_ALPHA_MODE ddsAlphaMode = DDS_ALPHA_MODE_UNKNOWN;
	bool bIsCubeMap = false;

	hr = ::LoadDDSTextureFromFileEx(
		pd3dDevice.Get(),
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

void Texture::LoadFromWICFile(ComPtr<ID3D12Device14> pd3dDevice, ID3D12Resource** ppOutResource, const std::wstring& wstrTexturePath, std::unique_ptr<uint8_t[]>& ddsData, D3D12_SUBRESOURCE_DATA& subResource)
{
	HRESULT hr;

	hr = ::LoadWICTextureFromFileEx(
		pd3dDevice.Get(),
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
