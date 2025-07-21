#include "stdafx.h"
#include "RenderPass.h"
#include "Mesh.h"
#include "Transform.h"

void DiffusedPass::Run(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::shared_ptr<Scene> pScene)
{
	pd3dCommandList->SetPipelineState(m_pPipeline->Get());
	pd3dCommandList->SetGraphicsRootSignature(m_pPipeline->GetRootSignature());

	ConstantBuffer& cbCamera = RESOURCE->AllocCBuffer();
	auto pCamera = pScene->GetCamera();

	XMFLOAT4X4 xmf4x4CameraData;
	XMStoreFloat4x4(&xmf4x4CameraData, XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewProjectMatrix())));
	cbCamera.WriteData(xmf4x4CameraData);

	m_pPipeline->BindShaderVariables(pd3dCommandList, SHADER_RESOURCE_TYPE_CONSTANT_BUFFER, ROOT_PARAMETER_TYPE_ROOT_DESCRIPTOR, "CAMERA", cbCamera);
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);

	for (auto& obj : pScene->GetObjectsInScene()) {
		auto pTransform = obj->GetComponent<Transform>();
		ConstantBuffer& cbTransform = RESOURCE->AllocCBuffer();

		XMFLOAT4X4 xmf4x4TransformData;
		XMStoreFloat4x4(&xmf4x4TransformData, XMMatrixTranspose(XMMatrixMultiply(XMLoadFloat4x4(&pTransform->GetLocalMatrix()), XMLoadFloat4x4(&pTransform->GetWorldMatrix()))));
		cbTransform.WriteData(xmf4x4TransformData);

		m_pPipeline->BindShaderVariables<ConstantBuffer>(pd3dCommandList, SHADER_RESOURCE_TYPE_CONSTANT_BUFFER, ROOT_PARAMETER_TYPE_ROOT_DESCRIPTOR, "TRANSFORM", cbTransform);

		auto pMesh = obj->GetComponent<Mesh>();
		pMesh->Render(pd3dCommandList);
	}

}
