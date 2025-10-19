#include "pch.h"
#include "RenderManager.h"
#include "MeshRenderer.h"

ComPtr<ID3D12RootSignature> RenderManager::g_pGlobalRootSignature = nullptr;

/*

							  +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+--
 D3D12_DESCRIPTOR_HEAP_TYPE   |    CBV    |    CBV    |    SRV    |    CBV    |    SRV    |    SRV    |    CBV    |    CBV    |    SRV    |    SRV    |  ...
							  +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+--
					  Value   |   Camera  |   Lights  |  CubeMap  |  Material |  Diffused |   Normal  |   World   |  Material |  Diffused |InstanceBuf|  ...
							  +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+--
				  data type   |        여기까지 Scene Data        |            Object 예시1(인스턴싱 X)           |       Object 예시2(인스턴싱)      |  ...
							  +-----------------------------------+-----------------------------------+-----------+-----------------------+-----------+--
				바인딩 함수   | SetGraphicsRootDescriptorTable(0) | SetGraphicsRootDescriptorTable(1) |   ...(2)  |        ...(1)         |   ...(3)  |
							  +-----------------------------------+-----------------------------------+-----------+-----------------------+-----------+
							  - 그림처럼 Object(MeshRenderer) 마다 구성이 다를 수 있음
							  - Bind 가 안된다고 안돌아가는게 아닌것으로 암 -> Shader 에서 접근을 안하면 Bind 안되도 가능한 것으로 알고있음
							  - 실제로 될지는 나도 아직 모름ㅎㅎ

	- 10.18 계획 수정
		- 인스턴싱 하고 안하고 구분은 없음
		- 1개짜리도 InstanceBuffer 에 월드행렬을 때려박아 Per-Pass Data 로 Bind
		- 나중에 Differed 할때가 되면 G-Buffer 를 Per-Shader 데이터에 박으면 됨 (지금은 일단 World만)
		- 그러면 그림은 아래처럼 바뀜

							  +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+------------+--
 D3D12_DESCRIPTOR_HEAP_TYPE   |    CBV    |    CBV    |    SRV    |    SRV    |    CBV    |    SRV    |    SRV    |    CBV    |    SRV    |     SRV    |  ...
							  +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+------------+--
					  Value   |   Camera  |   Lights  |  CubeMap  | World Inst|  Material |  Diffused |   Normal  |  Material |  Diffused | World Inst |  ...
							  +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+------------+--
				  data type   |             Scene Data            | Pass Data |            Object 예시1           |      Object 예시2     | Pass Data  |  ...
							  +-----------------------+-----------+-----------+-----------+-----------------------+-----------------------+------------+--
				바인딩 함수   |          ...(0)       |   ...(1)  |   ...(2)  |   ...(3)  |        ...(4)         |   ...(3)  |   ...(4)  |   ...(1)   |
							  +-----------------------+-----------+-----------+-----------+-----------------------+-----------------------+------------+


*/

RenderManager::RenderManager(ComPtr<ID3D12Device14> pd3dDevice, ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	m_pd3dDevice = pd3dDevice;

	m_pForwardPass = std::make_shared<ForwardPass>(pd3dDevice, pd3dCommandList);

	// Global Root Signature
	CD3DX12_DESCRIPTOR_RANGE d3dDescriptorRanges[5];
	d3dDescriptorRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 2, 0, 0, 0);	// Per Scene (Camera, Lights)
	d3dDescriptorRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, 0);	// Per Scene (Cubemap)
	d3dDescriptorRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, 0, 0);	// Per Pass	 (World 변환)
	d3dDescriptorRanges[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 2, 2, 0, 0);	// Per Object (Material, instance base) + 필요한경우 World
	d3dDescriptorRanges[4].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 2, 0, 0);	// Diffuse, Normal/Height

	CD3DX12_ROOT_PARAMETER d3dRootParameters[5];
	d3dRootParameters[0].InitAsDescriptorTable(1, &d3dDescriptorRanges[0], D3D12_SHADER_VISIBILITY_ALL);	// Scene (Camera & Light)
	d3dRootParameters[1].InitAsDescriptorTable(1, &d3dDescriptorRanges[1], D3D12_SHADER_VISIBILITY_ALL);	// Scene (Cubemap(Skybox))
	d3dRootParameters[2].InitAsDescriptorTable(1, &d3dDescriptorRanges[2], D3D12_SHADER_VISIBILITY_ALL);	// Pass (World)
	d3dRootParameters[3].InitAsDescriptorTable(1, &d3dDescriptorRanges[3], D3D12_SHADER_VISIBILITY_ALL);	// Material, Texture(Diffused, Normal/Height)
	d3dRootParameters[4].InitAsDescriptorTable(1, &d3dDescriptorRanges[4], D3D12_SHADER_VISIBILITY_ALL);	// Material, Texture(Diffused, Normal/Height)

	CD3DX12_STATIC_SAMPLER_DESC d3dSamplerDesc;
	d3dSamplerDesc.Init(0);
	d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc.ShaderRegister = 0;
	d3dSamplerDesc.RegisterSpace = 0;
	d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
		| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
		| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
		| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc{};
	{
		d3dRootSignatureDesc.NumParameters = _countof(d3dRootParameters);
		d3dRootSignatureDesc.pParameters = d3dRootParameters;
		d3dRootSignatureDesc.NumStaticSamplers = 1;
		d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc;
		d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;
	}

	ComPtr<ID3DBlob> pd3dSignatureBlob = nullptr;
	ComPtr<ID3DBlob> pd3dErrorBlob = nullptr;

	HRESULT hr = D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, pd3dSignatureBlob.GetAddressOf(), pd3dErrorBlob.GetAddressOf());
	if (pd3dErrorBlob) {
		char* pErrorString = (char*)pd3dErrorBlob->GetBufferPointer();
		HWND hWnd = ::GetActiveWindow();
		MessageBoxA(hWnd, pErrorString, NULL, 0);
		OutputDebugStringA(pErrorString);
		__debugbreak();
	}

	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(g_pGlobalRootSignature.GetAddressOf()));

	// DescriptorHandle Heap For Draw
	D3D12_DESCRIPTOR_HEAP_DESC d3dHeapDesc;
	{
		d3dHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		d3dHeapDesc.NumDescriptors = DESCRIPTOR_PER_DRAW;
		d3dHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		d3dHeapDesc.NodeMask = 0;
	}

	m_DescriptorHeapForDraw.Initialize(pd3dDevice, d3dHeapDesc);
}

RenderManager::~RenderManager()
{ 
}

void RenderManager::Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList)
{
	/*
		- 아이디어1
			1. vector 에 모조리 때려박음
			2. Pre-Pass 수행
			3. std::partition 으로 Forward, Differed 분할
			4. std::span view 를 이용하여 Forward, Differed 렌더링 각각 수행
			5. Post-Processing
			- 문제 
				- vector 에 때려박을 때 Instance Data 를 찾아가는데 시간 O(N)
				- Partition 에 O(N)

		- 아이디어2
			1. 기존 과제 방식대로 Map 과 vector를 병행
			2. vector로 Pre-Pass 수행
			3. Map에 보관된 인덱스를 따라 Forward, Differed 분할
			4. std::span view 를 이용하여 Forward, Differed 렌더링 각각 수행
			5. Post-Processing
	
		- 뭐가 더 좋을까 : 나도 모름 2번으로 일단 진행
	*/

	if (m_InstanceDatas[0].size() == 0 && m_InstanceDatas[1].size() == 0) {
		return;
	}
	pd3dCommandList->SetGraphicsRootSignature(g_pGlobalRootSignature.Get());
	pd3dCommandList->SetDescriptorHeaps(1, m_DescriptorHeapForDraw.Get().GetAddressOf());
	
	// Scene Data 바인딩
	DescriptorHandle descHandle = m_DescriptorHeapForDraw.GetDescriptorHandleFromHeapStart();
	
	CUR_SCENE->GetCamera()->SetViewportsAndScissorRects(pd3dCommandList);

	CB_CAMERA_DATA camData = CUR_SCENE->GetCamera()->MakeCBData();
	CB_LIGHT_DATA lightData = CUR_SCENE->MakeLightData();

	ConstantBuffer& camCBuffer = RESOURCE->AllocCBuffer<CB_CAMERA_DATA>();
	ConstantBuffer& lightCBuffer = RESOURCE->AllocCBuffer<CB_LIGHT_DATA>();

	camCBuffer.WriteData(&camData);
	lightCBuffer.WriteData(&lightData);

	m_pd3dDevice->CopyDescriptorsSimple(1, descHandle.cpuHandle, camCBuffer.CBVHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descHandle.cpuHandle.Offset(1, D3DCore::g_nCBVSRVDescriptorIncrementSize);
	m_pd3dDevice->CopyDescriptorsSimple(1, descHandle.cpuHandle, lightCBuffer.CBVHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descHandle.cpuHandle.Offset(1, D3DCore::g_nCBVSRVDescriptorIncrementSize);

	pd3dCommandList->SetGraphicsRootDescriptorTable(0, descHandle.gpuHandle);
	descHandle.gpuHandle.Offset(2, D3DCore::g_nCBVSRVDescriptorIncrementSize);

	// Pass 수행
	// Run 안에서 descHandle 의 offset 이 사용된만큼 움직임
	m_pForwardPass->Run(m_pd3dDevice, pd3dCommandList, m_InstanceDatas[OBJECT_RENDER_FORWARD], descHandle);
}

void RenderManager::Clear()
{
	for (int i = 0; i < 2; ++i) {
		m_InstanceIndexMap[i].clear();
		m_InstanceDatas[i].clear();
		m_nInstanceIndex[i] = 0;
	}
}