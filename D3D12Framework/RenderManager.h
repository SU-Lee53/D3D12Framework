#pragma once
#include "RenderPass.h"

/*
	2025.10.08
	- 대대적인 개편 필요
		- RenderManager -> RenderPass -> Pipeline 의 구조는 매우 복잡
		- RenderManager 에 Object 를 모아두고 MeshRenderer를 이용하여 렌더링
		- RenderPass 는 남겨놓고, 렌더링 단위를 변경하는 의미로 넘어가야 할듯

		+ Global 로 사용할 Root Signature RenderManager 에서 관리하도록 하는것도 괜찮아보임
*/

class MeshRenderer;

class RenderManager {
public:
	RenderManager(ComPtr<ID3D12Device14> pd3dDevice);
	~RenderManager();

	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Scene> pScene);

public:
	void Add(std::shared_ptr<MeshRenderer> pMeshRenderer);
	void Clear();

private:
	std::vector<std::shared_ptr<RenderPass>> m_pRenderPasses = {};

public:
	static ComPtr<ID3D12RootSignature> g_pGlobalRootSignature;

	// Pass 별 분리 필요 ( Forward / Differed )
	// 방법은 더 연구할 것
	std::unordered_map<MeshRenderer, UINT> m_InstanceIndexMap;	
	std::vector<std::pair<MeshRenderer, std::vector<Matrix>>> m_Instances;
	UINT m_nInstanceIndex = 0;

};

