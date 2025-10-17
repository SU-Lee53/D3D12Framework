#include "stdafx.h"
#include "RenderManager.h"
#include "MeshRenderer.h"

ComPtr<ID3D12RootSignature> RenderManager::g_pGlobalRootSignature = nullptr;

RenderManager::RenderManager(ComPtr<ID3D12Device14> pd3dDevice)
{
	std::shared_ptr<DiffusedPass> pDiffusedPass = std::make_shared<DiffusedPass>(pd3dDevice);
	m_pRenderPasses.push_back(pDiffusedPass);
}

RenderManager::~RenderManager()
{
}

void RenderManager::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Scene> pScene)
{ 
	for (auto& pass : m_pRenderPasses) {
		pass->Run(pd3dCommandList, pScene);
	}
}

void RenderManager::Add(std::shared_ptr<MeshRenderer> pMeshRenderer)
{
	MeshRenderer& key = *pMeshRenderer;
	Matrix mtxInstanceData = pMeshRenderer->GetOwner()->GetTransform().GetWorldMatrix();

	auto it = m_InstanceIndexMap.find(key);
	if (it == m_InstanceIndexMap.end()) {
		m_InstanceIndexMap[key] = m_nInstanceIndex++;
		m_Instances.push_back(std::make_pair(key, std::vector<Matrix>{}));

		m_Instances[m_InstanceIndexMap[key]].second.emplace_back(mtxInstanceData);
	}
	else {
		m_Instances[it->second].second.emplace_back(mtxInstanceData);
	}
}

void RenderManager::Clear()
{
	m_InstanceIndexMap.clear();
	m_Instances.clear();
	m_nInstanceIndex = 0;
}