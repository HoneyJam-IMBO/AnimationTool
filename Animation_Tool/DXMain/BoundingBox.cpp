#include "stdafx.h"
#include "BoundingBox.h"

void CBoundingBox::SetDebugBufferInfo(void ** ppMappedResources, int & nInstance, shared_ptr<CCamera> pCamera){
	//형변환
	VS_VB_BOUNDINGBOX_INSTANCE* pnInstances = (VS_VB_BOUNDINGBOX_INSTANCE*)ppMappedResources[0];

	//정보 주입
	pnInstances[nInstance].m_xmf3Pos = m_xmf3Position;
	pnInstances[nInstance].m_xmf3Extend = m_xmf3Extents;
	pnInstances[nInstance].m_xmf4Quaternion = m_xmf4Quaternion;

	//CGameObject::Begin();
}

void CBoundingBox::SetBufferInfo(void ** ppMappedResources, int & nInstance, shared_ptr<CCamera> pCamera){
	//형변환
	VS_VB_BOUNDINGBOX_INSTANCE* pnInstances = (VS_VB_BOUNDINGBOX_INSTANCE*)ppMappedResources[0];

	//XMStoreFloat4(&m_xmf4RotationQuaternion, XMQuaternionRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f));
	//XMStoreFloat4(&m_xmf4RotationQuaternion, XMQuaternionIdentity());
	//정보 주입
	pnInstances[nInstance].m_xmf3Pos = m_xmf3Position;
	pnInstances[nInstance].m_xmf3Extend = m_xmf3Extents;
	pnInstances[nInstance].m_xmf4Quaternion = m_xmf4Quaternion;
}

void CBoundingBox::SetBoundingBoxInfo(BoundingBox& aabb){
	m_xmf3Position = aabb.Center;
	m_xmf3Extents = aabb.Extents;
	XMStoreFloat4(&m_xmf4Quaternion, XMQuaternionIdentity());
}
void CBoundingBox::SetBoundingBoxInfo(BoundingOrientedBox& obb) {
	m_xmf3Position = obb.Center;
	m_xmf3Extents = obb.Extents;
	m_xmf4Quaternion = obb.Orientation;
}

CBoundingBox::CBoundingBox() :CGameObject("boundingbox"){
	XMStoreFloat4(&m_xmf4Quaternion, XMQuaternionIdentity());
}

CBoundingBox::~CBoundingBox()
{
}
