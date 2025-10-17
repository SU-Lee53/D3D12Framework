#pragma once
#include "ShaderResource.h"
#include "InputLayout.h"

/*
	- Mesh 개편
		- 서로 다른 Input Element는 별개의 정점버퍼를 갖도록 한다
			- 이유 : 한번에 모든 Input Element를 하나의 VB로 묶으면 
			         Prepass 등에서 원하는 요소만 바인딩할 수 없음

		- 기존 방식도 일단 유지
*/

struct MESHLOADINFO {
	std::string				strMeshName;

	UINT					nType;			// MESH_ELEMENT_TYPE

	Vector3				xmf3AABBCenter = Vector3(0.0f, 0.0f, 0.0f);
	Vector3				xmf3AABBExtents = Vector3(0.0f, 0.0f, 0.0f);

	std::vector<Vector3>	v3Positions;
	std::vector<Vector4>	v4Colors;
	std::vector<Vector3>	v3Normals;
	std::vector<Vector3>	v3Tangents;
							
	std::vector<Vector2>	v2TexCoord0;
	std::vector<Vector2>	v2TexCoord1;
	std::vector<Vector2>	v2TexCoord2;
	std::vector<Vector2>	v2TexCoord3;
	
	std::vector<XMINT4>		xmi4BlendIndices;
	std::vector<Vector4>	v4BlendWeights;

	std::vector<UINT>		Indices;

	std::vector<std::vector<UINT>> SubMeshes;

};

class Mesh {
public:
	Mesh(const MESHLOADINFO& meshLoadInfo, D3D12_PRIMITIVE_TOPOLOGY d3dTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	virtual ~Mesh() {}

	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nSubSet, UINT nInstanceCount = 1) = 0;

protected:
	VertexBuffer					m_PositionBuffer;
	std::vector<IndexBuffer>		m_IndexBuffers;	// vector for submeshes

protected:
	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology;
	UINT							m_nSlot = 0;
	UINT							m_nVertices = 0;
	UINT							m_nOffset = 0;

	UINT							m_nType = 0;

protected:
	// Bounding Volume
	BoundingOrientedBox m_xmOBB;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DiffusedMesh

class DiffusedMesh : public Mesh{
public:
	DiffusedMesh(const MESHLOADINFO& meshLoadInfo, D3D12_PRIMITIVE_TOPOLOGY d3dTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nSubSet, UINT nInstanceCount = 1) override;

protected:
	VertexBuffer	m_ColorBuffer;

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TexturedMesh

class TexturedMesh : public Mesh {
public:
	TexturedMesh(const MESHLOADINFO& meshLoadInfo, D3D12_PRIMITIVE_TOPOLOGY d3dTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nSubSet, UINT nInstanceCount = 1) override;

protected:
	VertexBuffer	m_NormalBuffer;
	VertexBuffer	m_TexCoordBuffer;

};
