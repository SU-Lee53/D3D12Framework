#pragma once
#include "DescriptorHeap.h"
#include "RenderPass.h"
#include "MeshRenderer.h"

/*
	2025.10.08
	- 대대적인 개편 필요
		- RenderManager -> RenderPass -> Pipeline 의 구조는 매우 복잡
		- RenderManager 에 Object 를 모아두고 MeshRenderer를 이용하여 렌더링
		- RenderPass 는 남겨놓고, 렌더링 단위를 변경하는 의미로 넘어가야 할듯

		+ Global 로 사용할 Root Signature RenderManager 에서 관리하도록 하는것도 괜찮아보임
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


	// Pass 별 분리 필요 ( Forward / Differed )
	// 방법은 더 연구할 것
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
