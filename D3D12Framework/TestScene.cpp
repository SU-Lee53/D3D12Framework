#include "stdafx.h"
#include "TestScene.h"
#include "CubeObject.h"
#include "Transform.h"
#include "DiffusedRenderer.h"

void GenerateCube(OUT MESHLOADINFO& meshLoadInfo, float fWidth, float fHeight, float fDepth)
{
	std::vector<Vector3> v3Positions;
	std::vector<Vector4> v4Colors;
	std::vector<UINT> idx;

	v3Positions.resize(8);

	float fx = fWidth * 0.5f;
	float fy = fHeight * 0.5f;
	float fz = fDepth * 0.5f;

	v3Positions[0] = Vector3(-fx, +fy, -fz);
	v3Positions[1] = Vector3(+fx, +fy, -fz);
	v3Positions[2] = Vector3(+fx, +fy, +fz);
	v3Positions[3] = Vector3(-fx, +fy, +fz);
	v3Positions[4] = Vector3(-fx, -fy, -fz);
	v3Positions[5] = Vector3(+fx, -fy, -fz);
	v3Positions[6] = Vector3(+fx, -fy, +fz);
	v3Positions[7] = Vector3(-fx, -fy, +fz);

	v4Colors.resize(8);
	v4Colors[0] = XMFLOAT4(1.0, 0.f, 0.f, 1.f);
	v4Colors[1] = XMFLOAT4(0.0, 1.f, 0.f, 1.f);
	v4Colors[2] = XMFLOAT4(0.0, 0.f, 1.f, 1.f);
	v4Colors[3] = XMFLOAT4(1.0, 0.f, 0.f, 1.f);
	v4Colors[4] = XMFLOAT4(0.0, 1.f, 0.f, 1.f);
	v4Colors[5] = XMFLOAT4(0.0, 0.f, 1.f, 1.f);
	v4Colors[6] = XMFLOAT4(1.0, 0.f, 0.f, 1.f);
	v4Colors[7] = XMFLOAT4(0.0, 1.f, 0.f, 1.f);

	idx.resize(36);

	// Front
	idx[0] = 3; idx[1] = 1; idx[2] = 0;
	idx[3] = 2; idx[4] = 1; idx[5] = 3;

	// Top
	idx[6] = 0; idx[7] = 5; idx[8] = 4;
	idx[9] = 1; idx[10] = 5; idx[11] = 0;

	// Back
	idx[12] = 3; idx[13] = 4; idx[14] = 7;
	idx[15] = 0; idx[16] = 4; idx[17] = 3;

	// Bottom
	idx[18] = 1; idx[19] = 6; idx[20] = 5;
	idx[21] = 2; idx[22] = 6; idx[23] = 1;

	// Left
	idx[24] = 2; idx[25] = 7; idx[26] = 6;
	idx[27] = 3; idx[28] = 7; idx[29] = 2;

	// Right
	idx[30] = 6; idx[31] = 4; idx[32] = 5;
	idx[33] = 7; idx[34] = 4; idx[35] = 6;

	meshLoadInfo.v3Positions = v3Positions;
	meshLoadInfo.v4Colors = v4Colors;

	meshLoadInfo.Indices = idx;
}

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
}

void TestScene::BuilObjects()
{
	MESHLOADINFO meshLoadInfo;
	::GenerateCube(meshLoadInfo, 5.f, 5.f, 5.f);

	MATERIALLOADINFO materialLoadInfo{};

	std::shared_ptr<DiffusedRenderer> pMeshRenderer = std::make_shared<DiffusedRenderer>(nullptr, meshLoadInfo, materialLoadInfo);

	std::shared_ptr<GameObject> p1 = std::make_shared<GameObject>();
	p1->AddComponent<DiffusedRenderer>(pMeshRenderer);
	p1->AddScript<VerticalCubeObject>();
	p1->Initialize();

	std::shared_ptr<DiffusedRenderer> pMeshRendererCopy = std::make_shared<DiffusedRenderer>(*pMeshRenderer);
	std::shared_ptr<GameObject> p2 = std::make_shared<GameObject>();
	p2->AddComponent<DiffusedRenderer>(pMeshRendererCopy);
	p2->AddScript<HorizontalCubeObject>();
	p2->Initialize();

	p1->GetTransform().SetPosition(-5.f, 0.f, 0.f);
	p2->GetTransform().SetPosition(5.f, 0.f, 0.f);

	AddObjects(p1, p2);

	m_pMainCamera = std::make_shared<Camera>();
	m_pMainCamera->SetViewport(0, 0, WinCore::sm_dwClientWidth, WinCore::sm_dwClientHeight, 0.f, 1.f);
	m_pMainCamera->SetScissorRect(0, 0, WinCore::sm_dwClientWidth, WinCore::sm_dwClientHeight);
	m_pMainCamera->GenerateViewMatrix(XMFLOAT3(0.f, 0.f, -15.f), XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(0.f, 1.f, 0.f));
	m_pMainCamera->GenerateProjectionMatrix(1.01f, 500.f, (WinCore::sm_dwClientWidth / WinCore::sm_dwClientHeight), 90.f);

	InitializeObjects();
}

void TestScene::Update()
{
	UpdateObjects();

}

void TestScene::Render()
{
}

void TestScene::ProcessInput()
{
}
