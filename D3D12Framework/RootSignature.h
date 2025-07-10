#pragma once


// =============
// RootParameter
// =============

struct RootParameter {

};

// =============
// RootSignatrue
// =============

constexpr D3D12_ROOT_SIGNATURE_FLAGS ROOT_SIGNATURE_FLAG_DEFAULT =	D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
																	D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
																	D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
																	D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
																	D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

class RootSignature {
public:
	RootSignature(ComPtr<ID3D12Device14> pd3dDevice) {}
	RootSignature(ComPtr<ID3D12Device14> pd3dDevice, D3D12_ROOT_SIGNATURE_FLAGS flags = ROOT_SIGNATURE_FLAG_DEFAULT) {}
	virtual ~RootSignature();

	ID3D12RootSignature* Get() const { return m_pRootSignature.Get(); }

protected:
	ComPtr<ID3D12RootSignature> m_pRootSignature = nullptr;
	D3D12_ROOT_SIGNATURE_FLAGS m_RootSignatureFlags;

};

class DiffusedRootSignature : public RootSignature {
public:
	DiffusedRootSignature(ComPtr<ID3D12Device14> pd3dDevice) 
		: RootSignature{ pd3dDevice, ROOT_SIGNATURE_FLAG_DEFAULT } {
		CD3DX12_ROOT_PARAMETER pd3dRootParameters[2];
		pd3dRootParameters[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_VERTEX);	// SRT
		pd3dRootParameters[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_VERTEX);	// Camera

		CD3DX12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
		d3dRootSignatureDesc.Init(2, pd3dRootParameters, 0, NULL, m_RootSignatureFlags);

		ComPtr<ID3DBlob> pd3dSignatureBlob = nullptr;
		ComPtr<ID3DBlob> pd3dErrorBlob = nullptr;

		HRESULT hr = ::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
		if (FAILED(hr)) {
			if (pd3dErrorBlob) {
				OutputDebugStringA((char*)pd3dErrorBlob->GetBufferPointer());
			}
		}

		hr = pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(m_pRootSignature.GetAddressOf()));

		if (FAILED(hr)) __debugbreak();
	}


	virtual ~DiffusedRootSignature() {}




};