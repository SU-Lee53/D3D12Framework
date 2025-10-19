#include "pch.h"
#include "FullScreenObject.h"
#include "Texture.h"

void GenerateBoard(OUT MESHLOADINFO& meshLoadInfo, float fWidth, float fHeight, float fDepth)
{
	std::vector<Vector3> v3Positions;

	v3Positions.resize(4);

	v3Positions[0] = Vector3(-1.f, 1.f, 0.f);
	v3Positions[1] = Vector3(1.f, 1.f, 0.f);
	v3Positions[2] = Vector3(-1.f, -1.f, 0.f);	// Triangle 1
	v3Positions[3] = Vector3(1.f, -1.f, 0.f);


	std::vector<UINT> idx;

	idx.resize(6);

	// Front
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 2; idx[4] = 1; idx[5] = 3;

	meshLoadInfo.v3Positions = v3Positions;

	meshLoadInfo.nIndices.reserve(1);
	meshLoadInfo.SubMeshes.reserve(1);
	meshLoadInfo.nIndices.push_back(idx.size());
	meshLoadInfo.SubMeshes.push_back(idx);
}

FullScreenObject::FullScreenObject()
{
}

FullScreenObject::~FullScreenObject()
{
}

void FullScreenObject::Initialize()
{
	MESHLOADINFO meshLoadInfo;
	std::vector<MATERIALLOADINFO> materialLoadInfos(1);

	GenerateBoard(meshLoadInfo, 5.0f, 5.0f, 5.0f);

	materialLoadInfos[0].m_strAlbedoMapName = "../Resource/tuk_credit.dds";

	AddComponent<MeshRenderer>(MeshRenderer::FFullScreenTextured(nullptr, meshLoadInfo, materialLoadInfos));

	GameObject::Initialize();
}

void FullScreenObject::Update()
{
	GameObject::Update();
}

void FullScreenObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	D3D12_VIEWPORT viewport{
		.TopLeftX = 0.f,
		.TopLeftY = 0.f,
		.Width = (float)WinCore::sm_dwClientWidth,
		.Height = (float)WinCore::sm_dwClientHeight,
		.MinDepth = 0.f,
		.MaxDepth = 1.f,
	};

	D3D12_RECT d3dRect{
		.left = 0,
		.top = 0,
		.right = (LONG)WinCore::sm_dwClientWidth,
		.bottom = (LONG)WinCore::sm_dwClientHeight
	};

	pd3dCommandList->RSSetViewports(1, &viewport);
	pd3dCommandList->RSSetScissorRects(1, &d3dRect);


	auto& pMeshRenderer = GetComponent<MeshRenderer>();
	DescriptorHeap& descHeapForDraw = RENDER->GetDescriptorHeap();

	pd3dCommandList->SetDescriptorHeaps(1, descHeapForDraw.Get().GetAddressOf());

	pMeshRenderer->GetMaterials()[0]->GetShader()->SetRootSignature(pd3dCommandList);

	FULLSCREENDATA fullscreen;
	fullscreen.Width = (float)WinCore::sm_dwClientWidth;
	fullscreen.Height = (float)WinCore::sm_dwClientHeight;

	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 2, &fullscreen, 0);

	std::shared_ptr<Texture> pTex = pMeshRenderer->GetMaterials()[0]->GetTexture(TEXTURE_TYPE_DIFFUSED);

	D3DCORE->GetDevice()->CopyDescriptorsSimple(1, descHeapForDraw.GetDescriptorHandleFromHeapStart().cpuHandle, pTex->GetDescriptorHeap().GetDescriptorHandleFromHeapStart().cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	pd3dCommandList->SetGraphicsRootDescriptorTable(1, descHeapForDraw.GetDescriptorHandleFromHeapStart().gpuHandle);


	pd3dCommandList->SetPipelineState(pMeshRenderer->GetMaterials()[0]->GetShader()->GetPipelineStates()[0].Get());

	pMeshRenderer->GetMesh()->Render(pd3dCommandList, 0, 1);


	GameObject::Render(pd3dCommandList);
}
