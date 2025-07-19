#include "stdafx.h"
#include "Mesh.h"

void Mesh::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nInstanceCount)
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
