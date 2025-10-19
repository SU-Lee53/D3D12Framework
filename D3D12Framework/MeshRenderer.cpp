#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"

MeshRenderer::MeshRenderer(std::shared_ptr<GameObject> pOwner)
	: Component(pOwner)
{
}
MeshRenderer::MeshRenderer(const MeshRenderer& other)
	: Component(other.m_wpOwner.lock())
{
	*this = other;
}

MeshRenderer::MeshRenderer(MeshRenderer&& other)
	: Component(nullptr)
{
	*this = std::move(other);
}

MeshRenderer& MeshRenderer::operator=(const MeshRenderer& other)
{
	if (this == &other) {
		return *this;
	}

	m_pMesh = other.m_pMesh;
	m_pMaterials = other.m_pMaterials;

	return *this;
}

// 문제 있을 "수도" 있음 나중에 확인 필요
MeshRenderer& MeshRenderer::operator=(MeshRenderer&& other)
{
	if (this == &other) {
		return *this;
	}

	m_pMesh = std::move(other.m_pMesh);
	m_pMaterials = std::move(other.m_pMaterials);

	m_wpOwner = std::move(other.m_wpOwner);

	return *this;
}

bool MeshRenderer::operator==(const MeshRenderer& rhs) const
{
	return m_pMesh == rhs.m_pMesh && m_pMaterials == rhs.m_pMaterials;
}

void MeshRenderer::Initialize()
{
}

void MeshRenderer::Update()
{
#ifdef WITH_FRUSTUM_CULLING
	
#else
	if (m_bAddToRenderManager) {
		RENDER->Add<MeshRenderer>(shared_from_this());
	}
#endif
}

std::shared_ptr<MeshRenderer> MeshRenderer::FDiffused(std::shared_ptr<GameObject> pOwner, std::shared_ptr<Mesh> pMesh, std::vector<std::shared_ptr<Material>> pMaterials)
{
	std::shared_ptr<MeshRenderer> pMeshRenderer = std::make_shared<MeshRenderer>(pOwner);
	pMeshRenderer->m_eObjectRenderType = OBJECT_RENDER_FORWARD;
	pMeshRenderer->m_pMesh = pMesh;
	pMeshRenderer->m_pMaterials = pMaterials;

	return pMeshRenderer;
}

std::shared_ptr<MeshRenderer> MeshRenderer::FDiffused(std::shared_ptr<GameObject> pOwner, const MESHLOADINFO& meshLoadInfo, const std::vector<MATERIALLOADINFO>& materialLoadInfo)
{
	std::shared_ptr<MeshRenderer> pMeshRenderer = std::make_shared<MeshRenderer>(pOwner);
	pMeshRenderer->m_eObjectRenderType = OBJECT_RENDER_FORWARD;
	pMeshRenderer->m_pMesh = std::make_shared<DiffusedMesh>(meshLoadInfo);

	for (const auto& materialInfo : materialLoadInfo) {
		std::shared_ptr<DiffusedMaterial> pMaterial = std::make_shared<DiffusedMaterial>(materialInfo);
		pMeshRenderer->m_pMaterials.push_back(pMaterial);
	}

	for (auto& mat : pMeshRenderer->m_pMaterials) {
		mat->SetShader(SHADER->Get<DiffusedShader>());
	}

	return pMeshRenderer;
}

std::shared_ptr<MeshRenderer> MeshRenderer::FFullScreenTextured(std::shared_ptr<GameObject> pOwner, const MESHLOADINFO& meshLoadInfo, const std::vector<MATERIALLOADINFO>& materialLoadInfo)
{
	std::shared_ptr<MeshRenderer> pMeshRenderer = std::make_shared<MeshRenderer>(pOwner);
	pMeshRenderer->m_eObjectRenderType = OBJECT_RENDER_FORWARD;
	pMeshRenderer->m_pMesh = std::make_shared<FullScreenMesh>(meshLoadInfo);

	for (const auto& materialInfo : materialLoadInfo) {
		std::shared_ptr<TexturedMaterial> pMaterial = std::make_shared<TexturedMaterial>(materialInfo);
		pMeshRenderer->m_pMaterials.push_back(pMaterial);
	}

	for (auto& mat : pMeshRenderer->m_pMaterials) {
		mat->SetShader(SHADER->Get<FullScreenShader>());
	}

	pMeshRenderer->m_bAddToRenderManager = false;

	return pMeshRenderer;
}
