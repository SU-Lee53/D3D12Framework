#pragma once
#include <d3dcompiler.h>


enum SHADER_TYPE
{
	SHADER_TYPE_VERTEX = 0,
	SHADER_TYPE_HULL,
	SHADER_TYPE_DOMAIN,
	SHADER_TYPE_GEOMETRY,
	SHADER_TYPE_PIXEL,
	SHADER_TYPE_COMPUTE,
	SHADER_TYPE_COUNT
};

class ShaderBase {
public:


protected:

};

template <SHADER_TYPE shaderTy>
class Shader : public ShaderBase {
public:
	Shader() = default;
	Shader(std::wstring_view wstrFileName, std::string_view strShaderName);
	virtual ~Shader() {}

	D3D12_SHADER_BYTECODE GetShaderBytecode() {
		return { m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize() };
	}

private:
	constexpr std::string_view GetShaderProfile() {
		if constexpr (shaderTy == SHADER_TYPE_VERTEX)
			return "vs_5_1";
		else if constexpr (shaderTy == SHADER_TYPE_GEOMETRY)
			return "gs_5_1";
		else if constexpr (shaderTy == SHADER_TYPE_HULL)
			return "hs_5_1";
		else if constexpr (shaderTy == SHADER_TYPE_DOMAIN)
			return "ds_5_1";
		else if constexpr (shaderTy == SHADER_TYPE_PIXEL)
			return "ps_5_1";
		else if constexpr (shaderTy == SHADER_TYPE_COMPUTE)
			return "cs_5_1";
	}

private:
	ComPtr<ID3DBlob> m_pShaderBlob = nullptr;

};

template<SHADER_TYPE shaderTy>
inline Shader<shaderTy>::Shader(std::wstring_view wstrFileName, std::string_view strShaderName)
{
	UINT nCompileFlags = 0;
#ifdef _DEBUG
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> pShaderBlob = nullptr;
	ComPtr<ID3DBlob> pErrorBlob = nullptr;

	std::string shaderProfile { GetShaderProfile() };

	HRESULT hr = ::D3DCompileFromFile(wstrFileName.data(), NULL, NULL, strShaderName.data(), shaderProfile.data(), nCompileFlags, 0, pShaderBlob.GetAddressOf(), pErrorBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA((const char*)pErrorBlob->GetBufferPointer());
			__debugbreak();
		}
	}
}

using VertexShader		= Shader<SHADER_TYPE_VERTEX>;
using HullShader		= Shader<SHADER_TYPE_HULL>;
using DomainShader		= Shader<SHADER_TYPE_DOMAIN>;
using GeometryShader	= Shader<SHADER_TYPE_GEOMETRY>;
using PixelShader		= Shader<SHADER_TYPE_PIXEL>;
using ComputeShader		= Shader<SHADER_TYPE_COMPUTE>;
