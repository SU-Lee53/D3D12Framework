#include "stdafx.h"
#include "DiffusedRenderer.h"

DiffusedRenderer::DiffusedRenderer(std::shared_ptr<GameObject> pOwner, std::shared_ptr<Mesh> pMesh, std::vector<std::shared_ptr<Material>> pMaterials)
	: MeshRenderer(pOwner)
{
}

DiffusedRenderer::DiffusedRenderer(std::shared_ptr<GameObject> pOwner, const MESHLOADINFO& meshLoadInfo, const std::vector<MATERIALLOADINFO>& materialLoadInfo)
	: MeshRenderer(pOwner)
{
	m_pMesh = std::make_shared<DiffusedMesh>(meshLoadInfo);
	m_pMaterials.emplace_back(materialLoadInfo);
}