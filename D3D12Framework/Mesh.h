#pragma once
#include "ShaderResource.h"
#include "InputLayout.h"

class MeshBase {
public:
	MeshBase() {}
	virtual ~MeshBase() {}

	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nInstanceCount = 1) = 0;
};

template<typename T>
class Mesh : public MeshBase {
public:
	Mesh(std::span<T> vertices, std::span<std::span<UINT>> SubMeshes,
		D3D12_PRIMITIVE_TOPOLOGY d3dTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		UINT nSlot = 0, UINT nOffset = 0);
	virtual ~Mesh() {}


	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nInstanceCount = 1);

private:
	VertexBuffer				m_VertexBuffer;
	std::vector<IndexBuffer>	m_IndexBuffers;	// vector for submeshes

protected:
	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT							m_nSlot = 0;
	UINT							m_nVertices = 0;
	UINT							m_nOffset = 0;

};

template<typename T>
inline Mesh<T>::Mesh(std::span<T> vertices, std::span<std::span<UINT>> SubMeshes, 
	D3D12_PRIMITIVE_TOPOLOGY d3dTopology, UINT nSlot, UINT nOffset)
{
	m_nSlot = 0;
	m_nVertices = vertices.size();
	m_nOffset = 0;

	m_VertexBuffer = RESOURCE->CreateVertexBuffer(vertices);
	
	m_IndexBuffers.reserve(SubMeshes.size());
	for (const auto& indices : SubMeshes) {
		m_IndexBuffers.push_back(RESOURCE->CreateIndexBuffer(indices));
	}

}

template<typename T>
inline void Mesh<T>::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nInstanceCount)
{
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dCommandList->IASetVertexBuffers(0, 1, &m_VertexBuffer.VertexBufferView);
	
	if (!m_IndexBuffers.empty()) {
		for (const auto& IndexBuffer : m_IndexBuffers) {
			pd3dCommandList->IASetIndexBuffer(&(IndexBuffer.IndexBufferView));
			pd3dCommandList->DrawIndexedInstanced(IndexBuffer.nIndices, nInstanceCount, 0, 0, 0);
		}
	}
	else {
		pd3dCommandList->DrawInstanced(m_nVertices, nInstanceCount, m_nOffset, 0);
	}

}
