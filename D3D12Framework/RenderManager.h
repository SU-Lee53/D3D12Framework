#pragma once
#include "RenderPass.h"

/*
	2025.10.08
	- ������� ���� �ʿ�
		- RenderManager -> RenderPass -> Pipeline �� ������ �ſ� ����
		- RenderManager �� Object �� ��Ƶΰ� MeshRenderer�� �̿��Ͽ� ������
		- RenderPass �� ���ܳ���, ������ ������ �����ϴ� �ǹ̷� �Ѿ�� �ҵ�

		+ Global �� ����� Root Signature RenderManager ���� �����ϵ��� �ϴ°͵� �����ƺ���
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

	// Pass �� �и� �ʿ� ( Forward / Differed )
	// ����� �� ������ ��
	std::unordered_map<MeshRenderer, UINT> m_InstanceIndexMap;	
	std::vector<std::pair<MeshRenderer, std::vector<Matrix>>> m_Instances;
	UINT m_nInstanceIndex = 0;

};

