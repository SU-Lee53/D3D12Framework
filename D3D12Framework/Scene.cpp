#include "stdafx.h"
#include "Scene.h"

void Scene::UpdateShaderVariables(ConstantBuffer& CBuffer)
{
	m_pMainCamera->UpdateShaderVariables(CBuffer);
}
