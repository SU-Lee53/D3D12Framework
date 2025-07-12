#include "stdafx.h"
#include "Transform.h"

Transform::Transform(std::shared_ptr<GameObject> pOwner)
	: Component{ pOwner }
{
	XMStoreFloat4x4(&m_xmf4x4LocalToModel, XMMatrixIdentity());
	XMStoreFloat4x4(&m_xmf4x4ModelToWorld, XMMatrixIdentity());
}
