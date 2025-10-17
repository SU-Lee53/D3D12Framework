#include "stdafx.h"
#include "CubeObject.h"
#include "MeshRenderer.h"
#include "Transform.h"

///////////////////////////
//  HorizontalCubeObject //
///////////////////////////

void VerticalCubeObject::Initialize()
{
	auto p = m_wpOwner.lock();

	MESHLOADINFO meshLoadInfo;

	std::vector<DiffusedVertex> vtx;
	std::vector<UINT> idx;
	GenerateCube(meshLoadInfo, 5.f, 5.f, 5.f);

	std::vector<std::vector<UINT>> idxs;
	idxs.push_back(idx);

	p->AddComponent<MeshRenderer>(p, meshLoadInfo /*, materialLoadInfos*/);
}

void VerticalCubeObject::Update()
{
	auto pTransform = m_wpOwner.lock()->GetTransform();

	pTransform.Rotate(pTransform.GetRight(), 1.f);
}

void VerticalCubeObject::OnCollision()
{
}

///////////////////////////
//  HorizontalCubeObject //
///////////////////////////

void HorizontalCubeObject::Initialize()
{
	auto p = m_wpOwner.lock();

	MESHLOADINFO meshLoadInfo;

	std::vector<DiffusedVertex> vtx;
	std::vector<UINT> idx;
	GenerateCube(meshLoadInfo, 5.f, 5.f, 5.f);

	std::vector<std::vector<UINT>> idxs;
	idxs.push_back(idx);

	p->AddComponent<MeshRenderer>(p, meshLoadInfo /*, materialLoadInfos*/);
}

void HorizontalCubeObject::Update()
{
	auto pTransform = m_wpOwner.lock()->GetTransform();

	pTransform.Rotate(pTransform.GetUp(), 1.f);
}

void HorizontalCubeObject::OnCollision()
{
}
