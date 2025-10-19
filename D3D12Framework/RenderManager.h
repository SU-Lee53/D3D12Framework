#pragma once
#include "DescriptorHeap.h"
#include "RenderPass.h"
#include "MeshRenderer.h"

/*
	2025.10.08
	- ������� ���� �ʿ�
		- RenderManager -> RenderPass -> Pipeline �� ������ �ſ� ����
		- RenderManager �� Object �� ��Ƶΰ� MeshRenderer�� �̿��Ͽ� ������
		- RenderPass �� ���ܳ���, ������ ������ �����ϴ� �ǹ̷� �Ѿ�� �ҵ�

		+ Global �� ����� Root Signature RenderManager ���� �����ϵ��� �ϴ°͵� �����ƺ���
*/

constexpr UINT DESCRIPTOR_PER_DRAW = 1000000;

enum OBJECT_RENDER_TYPE : UINT {
	OBJECT_RENDER_FORWARD = 0,
	OBJECT_RENDER_DIFFERED,

	OBJECT_RENDER_TYPE_COUNT
};

struct InstancePair {
	MeshRenderer meshRenderer;
	std::vector<Matrix> InstanceDatas;

	//std::pair<MeshRenderer, std::vector<Matrix>> GetPair() const {
	//	return { meshRenderer, InstanceDatas };
	//}

};

class RenderManager {
public:
	RenderManager(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);
	~RenderManager();

	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);

public:
	template<typename T> requires std::derived_from<T, MeshRenderer>
	void Add(std::shared_ptr<MeshRenderer> pMeshRenderer);
	void Clear();

public:
	DescriptorHeap& GetDescriptorHeap() { return m_DescriptorHeapForDraw; }

private:
	//std::vector<std::shared_ptr<RenderPass>> m_pRenderPasses = {};
	std::shared_ptr<ForwardPass> m_pForwardPass;


public:
	ComPtr<ID3D12Device14> m_pd3dDevice; // ref of D3DCore::m_pd3dDevice
	static ComPtr<ID3D12RootSignature> g_pGlobalRootSignature;
	DescriptorHeap m_DescriptorHeapForDraw;


	// Pass �� �и� �ʿ� ( Forward / Differed )
	// ����� �� ������ ��
	std::array<std::unordered_map<MeshRenderer, UINT>, OBJECT_RENDER_TYPE_COUNT> m_InstanceIndexMap;
	std::array<std::vector<InstancePair>, 2> m_InstanceDatas;

	UINT m_nInstanceIndex[2] = {0, 0};


};

template<typename T> requires std::derived_from<T, MeshRenderer>
inline void RenderManager::Add(std::shared_ptr<MeshRenderer> pMeshRenderer)
{
	const MeshRenderer& key = *pMeshRenderer;
	Matrix mtxInstanceData = pMeshRenderer->GetOwner()->GetTransform().GetWorldMatrix();
	UINT nRenderType = pMeshRenderer->m_eObjectRenderType;

	auto it = m_InstanceIndexMap[nRenderType].find(key);
	if (it == m_InstanceIndexMap[nRenderType].end()) {
		InstancePair instancePair{ key, std::vector<Matrix>{ mtxInstanceData } };

		m_InstanceIndexMap[nRenderType][key] = m_nInstanceIndex[nRenderType]++;
		m_InstanceDatas[nRenderType].push_back(instancePair);
	}
	else {
		m_InstanceDatas[nRenderType][it->second].InstanceDatas.emplace_back(mtxInstanceData);
	}
}
