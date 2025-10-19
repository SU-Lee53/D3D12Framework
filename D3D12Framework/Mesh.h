#pragma once
#include "ShaderResource.h"

/*
	- Mesh ����
		- ���� �ٸ� Input Element�� ������ �������۸� ������ �Ѵ�
			- ���� : �ѹ��� ��� Input Element�� �ϳ��� VB�� ������ 
			         Prepass ��� ���ϴ� ��Ҹ� ���ε��� �� ����

		- ���� ��ĵ� �ϴ� ����
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

	std::vector<UINT>		nIndices;

	std::vector<std::vector<UINT>> SubMeshes;

};

class Mesh {
public:
	Mesh(const MESHLOADINFO& meshLoadInfo, D3D12_PRIMITIVE_TOPOLOGY d3dTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	virtual ~Mesh() {}

	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nSubSet, UINT nInstanceCount = 1) const = 0;

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

	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nSubSet, UINT nInstanceCount = 1) const override;

protected:
	VertexBuffer	m_ColorBuffer;

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FullScreenMesh

class FullScreenMesh : public Mesh {
public:
	FullScreenMesh(const MESHLOADINFO& meshLoadInfo, D3D12_PRIMITIVE_TOPOLOGY d3dTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nSubSet, UINT nInstanceCount = 1) const override;

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TexturedMesh

class TexturedMesh : public Mesh {
public:
	TexturedMesh(const MESHLOADINFO& meshLoadInfo, D3D12_PRIMITIVE_TOPOLOGY d3dTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nSubSet, UINT nInstanceCount = 1) const override;

protected:
	VertexBuffer	m_TexCoordBuffer;

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TexturedNormalMesh

class TexturedNormalMesh : public Mesh {
public:
	TexturedNormalMesh(const MESHLOADINFO& meshLoadInfo, D3D12_PRIMITIVE_TOPOLOGY d3dTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, UINT nSubSet, UINT nInstanceCount = 1) const override;

protected:
	VertexBuffer	m_NormalBuffer;
	VertexBuffer	m_TexCoordBuffer;

};
