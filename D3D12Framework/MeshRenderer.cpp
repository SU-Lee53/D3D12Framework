#include "stdafx.h"
#include "MeshRenderer.h"

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

void MeshRenderer::Render(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dGraphicsCommandList)
{
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

#endif
}
