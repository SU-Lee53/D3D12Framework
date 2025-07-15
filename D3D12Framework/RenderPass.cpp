#include "stdafx.h"
#include "RenderPass.h"
#include "Mesh.h"
#include "Transform.h"

void DiffusedPass::Run(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Scene> pScene)
{
	ConstantBuffer& cbCamera = RESOURCE->AllocConstantBuffer();
	auto pCamera = pScene->GetCamera();
	pCamera->UpdateShaderVariables(cbCamera);

	for (auto& obj : pScene->GetObjectsInScene()) {
		auto pTransform = obj->GetComponent<Transform>();
		ConstantBuffer& cbTransform = RESOURCE->AllocConstantBuffer();
		pTransform->UpdateShaderVariables(cbTransform);

		auto pMesh = obj->GetComponent<Mesh<DiffusedVertex>>();
		pMesh->Render(pd3dCommandList);
	}

}
