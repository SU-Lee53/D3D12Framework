#pragma once
#include "Component.h"
#include "ShaderResource.h"
#include "InputLayout.h"

class Mesh : public Component {
public:
	Mesh() = default;

	template<typename T, typename U>
	Mesh(std::shared_ptr<GameObject> pOwner, std::vector<T> vertices, std::vector<U> SubMeshes,
		D3D12_PRIMITIVE_TOPOLOGY d3dTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		UINT nSlot = 0, UINT nOffset = 0);

	virtual ~Mesh() {}

	virtual void Update() override {}
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nInstanceCount = 1);

private:
	VertexBuffer				m_VertexBuffer;
	std::vector<IndexBuffer>	m_IndexBuffers;	// vector for submeshes

private:
	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT							m_nSlot = 0;
	UINT							m_nVertices = 0;
	UINT							m_nOffset = 0;

private:
	// Bounding Volume
	BoundingOrientedBox m_xmOBB;
};

template <>
struct Component_Type<Mesh> {
	constexpr static COMPONENT_TYPE componentType = COMPONENT_TYPE_MESH;
};

template<typename T, typename U>
inline Mesh::Mesh(std::shared_ptr<GameObject> pOwner, std::vector<T> vertices, std::vector<U> SubMeshes,
	D3D12_PRIMITIVE_TOPOLOGY d3dTopology, UINT nSlot, UINT nOffset)
	: Component{pOwner}
{
	static_assert(
		std::is_same<U, std::vector<UINT>>::value ||
		std::is_same<U, UINT>::value,
		"SubMeshes types are valid when vector<UINT> or UINT."
	);

	m_nSlot = 0;
	m_nVertices = vertices.size();
	m_nOffset = 0;

	m_VertexBuffer = RESOURCE->CreateVertexBuffer(vertices);
	
	if constexpr (std::is_same_v <U, std::vector<UINT>>) {
		m_IndexBuffers.reserve(SubMeshes.size());
		for (const auto& indices : SubMeshes) {
			m_IndexBuffers.push_back(RESOURCE->CreateIndexBuffer(indices));
		}
	}
	else {
		m_IndexBuffers.push_back(RESOURCE->CreateIndexBuffer(SubMeshes));
	}

}
