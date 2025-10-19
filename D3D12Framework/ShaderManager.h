#pragma once

class ShaderManager {
public:
	ShaderManager(ComPtr<ID3D12Device14> pDevice);
	~ShaderManager();

public:
	void Initialize();

	template<typename T> requires std::derived_from<T, Shader>
	bool Load(ComPtr<ID3D12RootSignature> pRootSignature = RenderManager::g_pGlobalRootSignature);

	template<typename T> requires std::derived_from<T, Shader>
	std::shared_ptr<T> Get();


private:
	ComPtr<ID3D12Device14>				m_pd3dDevice = nullptr;		// Reference to D3DCore::m_pd3dDevice

private:
	std::unordered_map<std::type_index, std::shared_ptr<Shader>> m_pShaderMap;

};

template<typename T> requires std::derived_from<T, Shader>
inline bool ShaderManager::Load(ComPtr<ID3D12RootSignature> pRootSignature)
{
	if (!m_pShaderMap.contains(typeid(T))) {
		std::shared_ptr<T> pShader = std::make_shared<T>();
		pShader->Initialize(m_pd3dDevice, pRootSignature);
		m_pShaderMap.insert({ typeid(T), pShader });
		return true;
	}

	return false;
}

template<typename T> requires std::derived_from<T, Shader>
inline std::shared_ptr<T> ShaderManager::Get()
{
	auto it = m_pShaderMap.find(typeid(T));
	if (it != m_pShaderMap.end()) {
		return static_pointer_cast<T>(it->second);
	}

	return nullptr;
}
