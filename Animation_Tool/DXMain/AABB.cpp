#include "stdafx.h"
#include "AABB.h"

void AABB::SetDebugBufferInfo(void ** ppMappedResources, int & nInstance, shared_ptr<CCamera> pCamera){
	//����ȯ
	VS_VB_AABB_INSTANCE* pnInstances = (VS_VB_AABB_INSTANCE*)ppMappedResources[0];

	//���� ����
	pnInstances[nInstance].m_xmf3Pos = m_BoundingBox.Center;
	pnInstances[nInstance].m_xmf3Extend = m_BoundingBox.Extents;
	pnInstances[nInstance].m_xmf4Quaternion = m_xmf4RotationQuaternion;

	//CGameObject::Begin();
}

void AABB::SetBufferInfo(void ** ppMappedResources, int & nInstance, shared_ptr<CCamera> pCamera){
	//����ȯ
	VS_VB_AABB_INSTANCE* pnInstances = (VS_VB_AABB_INSTANCE*)ppMappedResources[0];

	//XMStoreFloat4(&m_xmf4RotationQuaternion, XMQuaternionRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f));
	//XMStoreFloat4(&m_xmf4RotationQuaternion, XMQuaternionIdentity());
	//���� ����
	pnInstances[nInstance].m_xmf3Pos = m_BoundingBox.Center;
	pnInstances[nInstance].m_xmf3Extend = m_BoundingBox.Extents;
	pnInstances[nInstance].m_xmf4Quaternion = m_xmf4RotationQuaternion;
}

void AABB::SetBoundingBoxInfo(CGameObject * pObject){
	m_BoundingBox = pObject->GetBoundingBox();
}

AABB::AABB() :CGameObject("aabb"){
	XMStoreFloat4(&m_xmf4RotationQuaternion, XMQuaternionIdentity());
}

AABB::~AABB()
{
}
