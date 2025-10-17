#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

class GameObject;

class MeshRenderer : public Component {
public:
	MeshRenderer(std::shared_ptr<GameObject> pOwner);
	virtual ~MeshRenderer() {}

	MeshRenderer(const MeshRenderer& other);
	MeshRenderer(MeshRenderer&& other);
	
	MeshRenderer& operator=(const MeshRenderer& other);
	MeshRenderer& operator=(MeshRenderer&& other);

public:
	void Initialize() override;
	void Update() override;

public:
	virtual void Render(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dGraphicsCommandList, void* instanceDatas = nullptr, int nInstances = 0) = 0;

public:
	bool operator==(const MeshRenderer& rhs) const;

protected:
	std::shared_ptr<Mesh> m_pMesh = nullptr;
	std::vector<std::shared_ptr<Material>> m_pMaterials;

	friend struct std::hash<MeshRenderer>;
};

template <>
struct Component_Type<MeshRenderer> {
	constexpr static COMPONENT_TYPE componentType = COMPONENT_TYPE_MESH_RENDERER;
};

template<>
struct std::hash<MeshRenderer> {
	size_t operator()(const MeshRenderer& meshRenderer) {
		return (std::hash<decltype(meshRenderer.m_pMesh)>{}(meshRenderer.m_pMesh) ^
			std::hash<decltype(meshRenderer.m_pMaterials)::value_type>{}(meshRenderer.m_pMaterials[0]));
	}
};