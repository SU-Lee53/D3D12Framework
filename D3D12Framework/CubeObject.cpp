#include "stdafx.h"
#include "CubeObject.h"
#include "Mesh.h"

void GenerateCube(std::vector<DiffusedVertex> vtx, std::vector<UINT> idx, float fWidth, float fHeight, float fDepth)
{
	vtx.resize(8);

	float fx = fWidth * 0.5f;
	float fy = fHeight * 0.5f;
	float fz = fDepth * 0.5f;

	vtx[0] = DiffusedVertex(XMFLOAT3(-fx, +fy, -fz), XMFLOAT4(1.0, 0.f, 0.f, 1.f));
	vtx[1] = DiffusedVertex(XMFLOAT3(+fx, +fy, -fz), XMFLOAT4(0.0, 1.f, 0.f, 1.f));
	vtx[2] = DiffusedVertex(XMFLOAT3(+fx, +fy, +fz), XMFLOAT4(0.0, 0.f, 1.f, 1.f));
	vtx[3] = DiffusedVertex(XMFLOAT3(-fx, +fy, +fz), XMFLOAT4(1.0, 0.f, 0.f, 1.f));
	vtx[4] = DiffusedVertex(XMFLOAT3(-fx, -fy, -fz), XMFLOAT4(0.0, 1.f, 0.f, 1.f));
	vtx[5] = DiffusedVertex(XMFLOAT3(+fx, -fy, -fz), XMFLOAT4(0.0, 0.f, 1.f, 1.f));
	vtx[6] = DiffusedVertex(XMFLOAT3(+fx, -fy, +fz), XMFLOAT4(1.0, 0.f, 0.f, 1.f));
	vtx[7] = DiffusedVertex(XMFLOAT3(-fx, -fy, +fz), XMFLOAT4(0.0, 1.f, 0.f, 1.f));

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
}

void CubeObject::Initialize()
{
	auto p = m_wpOwner.lock();

	std::vector<DiffusedVertex> vtx;
	std::vector<UINT> idx;
	GenerateCube(vtx, idx, 5.f, 5.f, 5.f);

	std::vector<std::vector<UINT>> idxs;
	idxs.push_back(idx);

	p->AddComponent<Mesh>(vtx, idx);


}

void CubeObject::Update()
{
}

void CubeObject::OnCollision()
{
}
