#include "stdafx.h"
#include "TestObject.h"


void TW_CALL AnimationSelectCallback(void * clientData) {
	CAnimationInfo* pAnimInfo = reinterpret_cast<CAnimationInfo*>(clientData);
	pAnimInfo->SelectAnimationProc();
}
void TW_CALL JointSelectButtonCallback(void * clientData) {
	CBoundingBox* pOBB = reinterpret_cast<CBoundingBox*>(clientData);
	if (pOBB->GetActive())
		pOBB->SetActive(false);
	else
		pOBB->SetActive(true);

}

bool CTestObject::Begin() {
	//object_id set
	m_objectID = object_id::OBJECT_FBX_ELF;
	//m_objectID = object_id::OBJECT_TEST;

	Rotate(-90.f, 0.f, 0.f);
	//���ο� ��ü�� ���ܳ��� Begin���� Component���� set���ش�. 
	//CComponent* pComponent = new CRotateComponent;
	//pComponent->Begin();
	//return SetComponent(pComponent);

	CGameObject::Begin();
	return true;
}
bool CTestObject::End() {

	return CGameObject::End();
}
void CTestObject::SetPosition(XMVECTOR pos) {
	XMFLOAT3 xmfPos;
	XMStoreFloat3(&xmfPos, pos);

	m_xmf4x4World._41 = xmfPos.x;
	m_xmf4x4World._42 = xmfPos.y;
	m_xmf4x4World._43 = xmfPos.z;

	if( m_pTerrainContainer ) m_xmf4x4World._42 = GetTerrainHeight();
	
}
void CTestObject::Animate(float fTimeElapsed) {
	if (m_pAnimater)m_pAnimater->Update(fTimeElapsed);
	//for (int i = 0; i < m_pRenderContainer->GetvMesh().size(); ++i) {
	//	m_pRenderContainer->GetMesh(i).get()->Update(fTimeElapsed);
	//}

}void CTestObject::PickingProc() {
	//1. ��� mesh�� ��� 
	CGameObject::PickingProc();

	//animation ui
	//�̷��� pMesh�� ���ؼ� �̷��� �Ѱ��ݾ�? ���� �޽���? ��� �ִϸ��̼� �����!
	//��� joint �����!!! 
	TWBARMGR->AddPositionBar("PickingBar", "MainAABB", "MainAABB Position", m_pAnimater->GetMainAABB(), 0.f, SPACE_SIZE - 1.0f, 1.0f);
	TWBARMGR->AddScaleBar("PickingBar", "MainAABB", "MainAABB Scale", m_pAnimater->GetMainAABB(), 0.1f, 100.f, 0.1f);
	
	//for(��� �ִϸ��̼ǿ� ���ؼ�){
	//TWBARMGR->AddButtonCB("PickingBar", "Animation Select", "Animation name", AnimationSelectCallback, �ִϸ��̼� ����);
	//}
	for (size_t i = 0; i < m_pAnimater->GetAnimationCnt(); ++i) {
		string manuNameStr = m_pAnimater->GetAnimationInfo(i)->GetAnimationName();
		const char* menuName = manuNameStr.c_str();
		TWBARMGR->AddButtonCB("PickingBar", "Animation Select", menuName, AnimationSelectCallback, m_pAnimater->GetAnimationInfo(i));
	}

}
CTestObject::CTestObject() : CGameObject("testobject", tag::TAG_DYNAMIC_OBJECT) {

}
CTestObject::~CTestObject() {

}