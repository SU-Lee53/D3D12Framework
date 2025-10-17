#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shader

class Shader {
public:
	/// <summary>
	/// Shader 를 초기화
	/// Derived Class 에서 반드시 Override
	/// </summary>
	/// <param name="pd3dDevice"></param>
	/// <param name="pd3dRootSignature">
	/// nullptr 로 유지할 경우 RenderManager 의 Global Root Signature 를 사용하도록 함
	/// </param>
	virtual void Initialize(ComPtr<ID3D12Device14> pd3dDevice, 
		ComPtr<ID3D12RootSignature> pd3dRootSignature = nullptr) = 0;

protected:
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout() { return D3D12_INPUT_LAYOUT_DESC{}; }
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader() = 0;
	virtual D3D12_SHADER_BYTECODE CreatePixelShader() = 0;

	D3D12_SHADER_BYTECODE CompileShaderFromFile(const std::wstring& wstrFileName, const std::string& strShaderName, const std::string& strShaderProfile, ID3DBlob** ppd3dShaderBlob);
	D3D12_SHADER_BYTECODE ReadCompiledShaderFromFile(const std::wstring& wstrFileName, ID3DBlob** ppd3dShaderBlob);


protected:
	ComPtr<ID3DBlob> m_pd3dVertexShaderBlob = nullptr;
	ComPtr<ID3DBlob> m_pd3dPixelShaderBlob = nullptr;

	std::vector<ComPtr<ID3D12PipelineState>>	m_pd3dPipelineStates = {};

	std::vector<D3D12_INPUT_ELEMENT_DESC>		m_d3dInputElements = {};

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DiffusedShader

class DiffusedShader : public Shader {
public:
	virtual void Initialize(ComPtr<ID3D12Device14> pd3dDevice, 
		ComPtr<ID3D12RootSignature> pd3dRootSignature = nullptr) override;

protected:
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout() override;

	virtual D3D12_SHADER_BYTECODE CreateVertexShader() override;
	virtual D3D12_SHADER_BYTECODE CreatePixelShader() override;

	D3D12_SHADER_BYTECODE CreateInstancedVertexShader();

	// m_PipelineStates[0] -> No Instancing
	// m_PipelineStates[1] -> Inscancing

};