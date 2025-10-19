#include "pch.h"
#include "CubeObject.h"

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


	meshLoadInfo.nIndices.reserve(1);
	meshLoadInfo.SubMeshes.reserve(1);
	meshLoadInfo.nIndices.push_back(idx.size());
	meshLoadInfo.SubMeshes.push_back(idx);

}

CubeObject::CubeObject()
{
}

CubeObject::~CubeObject()
{
}

void CubeObject::Initialize()
{
	MESHLOADINFO meshLoadInfo;
	std::vector<MATERIALLOADINFO> materialLoadInfos;

	GenerateCube(meshLoadInfo, 5.0f, 5.0f, 5.0f);

	AddComponent<MeshRenderer>(MeshRenderer::FDiffused(nullptr, meshLoadInfo, materialLoadInfos));

	//AddComponent<FDiffusedRenderer>(meshLoadInfo, materialLoadInfos);

	GameObject::Initialize();
}

void CubeObject::Update()
{
	GameObject::Update();
}

void CubeObject::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	GameObject::Render(pd3dCommandList);
}