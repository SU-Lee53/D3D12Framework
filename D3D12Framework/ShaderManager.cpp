#include "pch.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager(ComPtr<ID3D12Device14> pDevice)
{
	m_pd3dDevice = pDevice;
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::Initialize()
{
	Load<DiffusedShader>();
	Load<FullScreenShader>();
}
