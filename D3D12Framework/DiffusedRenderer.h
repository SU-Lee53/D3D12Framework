#pragma once
#include "MeshRenderer.h"

class DiffusedRenderer : public MeshRenderer {
public:
	DiffusedRenderer(std::shared_ptr<GameObject> pOwner, std::shared_ptr<Mesh> pMesh, std::vector<std::shared_ptr<Material>> pMaterials);
	DiffusedRenderer(std::shared_ptr<GameObject> pOwner, const MESHLOADINFO& meshLoadInfo, const std::vector<MATERIALLOADINFO>& materialLoadInfo);



public:
	constexpr static int RenderQueueIndex = 0;
};