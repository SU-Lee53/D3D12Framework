#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

class GameObject;

class MeshRenderer : public Component, public std::enable_shared_from_this<MeshRenderer> {
public:
	UINT m_eObjectRenderType = -1;
	friend struct std::hash<MeshRenderer>;

public:
	MeshRenderer(std::shared_ptr<GameObject> pOwner);
	virtual ~MeshRenderer() {}

	MeshRenderer(const MeshRenderer& other);
	MeshRenderer(MeshRenderer&& other);
	
	MeshRenderer& operator=(const MeshRenderer& other);
	MeshRenderer& operator=(MeshRenderer&& other);

public:
	const std::shared_ptr<Mesh>& GetMesh() const { return m_pMesh; }
	const std::vector<std::shared_ptr<Material>>& GetMaterials() const { return m_pMaterials; };

public:
	void Initialize() override;
	void Update() override;
	
	//virtual void Render(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dGraphicsCommandList, void* instanceDatas = nullptr, int nInstances = 0) = 0;

public:
	bool operator==(const MeshRenderer& rhs) const;

protected:
	std::shared_ptr<Mesh> m_pMesh = nullptr;
	std::vector<std::shared_ptr<Material>> m_pMaterials;
	bool m_bAddToRenderManager = true;	// 독립적인 렌더링 방법을 가지는 경우 false

public:
	static std::shared_ptr<MeshRenderer> FDiffused(std::shared_ptr<GameObject> pOwner, std::shared_ptr<Mesh> pMesh, std::vector<std::shared_ptr<Material>> pMaterials);
	static std::shared_ptr<MeshRenderer> FDiffused(std::shared_ptr<GameObject> pOwner, const MESHLOADINFO& meshLoadInfo, const std::vector<MATERIALLOADINFO>& materialLoadInfo);

	static std::shared_ptr<MeshRenderer> FFullScreenTextured(std::shared_ptr<GameObject> pOwner, const MESHLOADINFO& meshLoadInfo, const std::vector<MATERIALLOADINFO>& materialLoadInfo);
};

template <>
struct Component_Type<MeshRenderer> {
	constexpr static COMPONENT_TYPE componentType = COMPONENT_TYPE_MESH_RENDERER;
};

template<>
struct std::hash<MeshRenderer> {
	size_t operator()(const MeshRenderer& meshRenderer) const {
		return (std::hash<decltype(meshRenderer.m_pMesh)>{}(meshRenderer.m_pMesh) ^
			std::hash<decltype(meshRenderer.m_pMaterials)::value_type>{}(meshRenderer.m_pMaterials[0]));
	}
};