#include "pch.h"
#include "RenderPass.h"
#include "Mesh.h"
#include "Transform.h"
#include "RenderManager.h"	// for InstancePair

ForwardPass::ForwardPass(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommmandList)
{
	m_InstanceSBuffer.Create(pd3dDevice, pd3dCommmandList, 100000, sizeof(Matrix), true);
}

void ForwardPass::Run(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, std::span<InstancePair> instances, DescriptorHandle& descHandleFromPassStart)
{
	UINT uiSBufferOffset = 0;
	for (const auto& [k, v] : instances) {
		m_InstanceSBuffer.UpdateData(v, uiSBufferOffset);
		uiSBufferOffset += v.size();
	}

#ifdef WITH_UPLOAD_BUFFER
	m_InstanceDataSBuffer.UpdateResources(m_pd3dDevice, pd3dCommandList);

#endif

	pd3dDevice->CopyDescriptorsSimple(1, descHandleFromPassStart.cpuHandle,
		m_InstanceSBuffer.GetCPUDescriptorHandle(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descHandleFromPassStart.cpuHandle.Offset(1, D3DCore::g_nCBVSRVDescriptorIncrementSize);

	pd3dCommandList->SetGraphicsRootDescriptorTable(2, descHandleFromPassStart.gpuHandle);
	descHandleFromPassStart.gpuHandle.Offset(1, D3DCore::g_nCBVSRVDescriptorIncrementSize);

	int nInstanceBase = 0;
	int nInstanceCount = 0;
	for (const auto& [k, v] : instances) {
		nInstanceCount = v.size();
		const auto& materials = k.GetMaterials();
		const auto& mesh = k.GetMesh();

		for (int i = 0; i < materials.size(); ++i) {
			// Per Object CB
			PER_OBJECT_CB_DATA cbData = { materials[i]->GetMaterialColors(), nInstanceBase };
			ConstantBuffer cbuffer = RESOURCE->AllocCBuffer<PER_OBJECT_CB_DATA>();
			cbuffer.WriteData(&cbData);

			pd3dDevice->CopyDescriptorsSimple(1, descHandleFromPassStart.cpuHandle, cbuffer.CBVHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			descHandleFromPassStart.cpuHandle.Offset(1, D3DCore::g_nCBVSRVDescriptorIncrementSize);

			// Texture (�ִٸ�)
			materials[i]->UpdateShaderVariables(pd3dDevice, descHandleFromPassStart.cpuHandle);	// Texture �� �ִٸ� Descriptor �� ����� ���̰� �ƴϸ� �ȵɰ�
			pd3dCommandList->SetGraphicsRootDescriptorTable(2, descHandleFromPassStart.gpuHandle);

			const auto& pipelineStates = materials[i]->GetShader()->GetPipelineStates();
			pd3dCommandList->SetPipelineState(pipelineStates[1].Get());

			mesh->Render(pd3dCommandList, i, nInstanceCount);
		}

		nInstanceBase += nInstanceCount;
	}
}
