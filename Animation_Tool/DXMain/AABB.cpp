#include "stdafx.h"
#include "AABB.h"

void AABB::SetDebugBufferInfo(void ** ppMappedResources, int & nInstance, shared_ptr<CCamera> pCamera){
	//형변환
	VS_VB_AABB_INSTANCE* pnInstances = (VS_VB_AABB_INSTANCE*)ppMappedResources[0];

	//정보 주입
	pnInstances[nInstance].m_xmf3Pos = m_BoundingBox.Center;
	pnInstances[nInstance].m_xmf3Extend = m_BoundingBox.Extents;

	//CGameObject::Begin();
}

void AABB::SetBufferInfo(void ** ppMappedResources, int & nInstance, shared_ptr<CCamera> pCamera){
	//형변환
	VS_VB_AABB_INSTANCE* pnInstances = (VS_VB_AABB_INSTANCE*)ppMappedResources[0];

	//정보 주입
	pnInstances[nInstance].m_xmf3Pos = m_BoundingBox.Center;
	pnInstances[nInstance].m_xmf3Extend = m_BoundingBox.Extents;
}

void AABB::SetBoundingBoxInfo(CGameObject * pObject){
	m_BoundingBox = pObject->GetBoundingBox();
}

AABB::AABB() :CGameObject("aabb"){

}

AABB::~AABB()
{
}
