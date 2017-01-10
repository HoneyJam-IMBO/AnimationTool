#include "stdafx.h"
#include "Bunny.h"

void TW_CALL AnimationSelectCallback(void * clientData) {
	CAnimationInfo* pAnimInfo = reinterpret_cast<CAnimationInfo*>(clientData);
	pAnimInfo->SelectAnimationProc();
}
void TW_CALL JointSelectButtonCallback(void * clientData) {
	CBoundingBox* pOBB = reinterpret_cast<CBoundingBox*>(clientData);
	if(pOBB->GetActive())
		pOBB->SetActive(false);
	else
		pOBB->SetActive(true);
	
}
bool CBunny::Begin() {
	//object_id set
	m_objectID = object_id::OBJECT_FBX_BUNNY;
	//m_objectID = object_id::OBJECT_TEST;

	CGameObject::Begin();
//	Rotate(-90.f, 0.f, 0.f);
	//새로운 객체가 생겨나면 Begin에서 Component들을 set해준다. 
	//CComponent* pComponent = new CRotateComponent;
	//pComponent->Begin();
	//return SetComponent(pComponent);


	return true;
}
bool CBunny::End() {

	return CGameObject::End();
}
void CBunny::Animate(float fTimeElapsed){
	if (m_pAnimater)m_pAnimater->Update(fTimeElapsed);
	//for (int i = 0; i < m_pRenderContainer->GetvMesh().size(); ++i) {
	//	m_pRenderContainer->GetMesh(i).get()->Update(fTimeElapsed);
	//}

}
void CBunny::SetPosition(XMVECTOR pos) {
	XMFLOAT3 xmfPos;
	XMStoreFloat3(&xmfPos, pos);

	m_xmf4x4World._41 = xmfPos.x;
	m_xmf4x4World._42 = xmfPos.y;
	m_xmf4x4World._43 = xmfPos.z;

	if (m_pTerrainContainer) m_xmf4x4World._42 = GetTerrainHeight();

}

void CBunny::PickingProc() {
	//1. 모든 mesh의 목록 
	CGameObject::PickingProc();

	//animation ui
	//이러면 pMesh에 한해서 이렇게 한거잖아? 여러 메쉬면? 모든 애니메이션 목록은!
	//모든 joint 목록은!!! 

	CFBXAnimationMesh* pMesh = dynamic_cast<CFBXAnimationMesh*>(m_pRenderContainer->GetMesh(0).get());

	//for(모든 애니메이션에 대해서){
		//TWBARMGR->AddButtonCB("PickingBar", "Animation Select", "Animation name", AnimationSelectCallback, 애니메이션 정보);
	//}
	for (size_t i = 0; i < m_pAnimater->GetAnimationCnt(); ++i) {
		string manuNameStr = m_pAnimater->GetAnimationInfo(i)->GetAnimationName();
		const char* menuName = manuNameStr.c_str();
		TWBARMGR->AddButtonCB("PickingBar", "Animation Select", menuName, AnimationSelectCallback, m_pAnimater->GetAnimationInfo(i));
	}
	
}

void CBunny::SelectAnimationProc(){
	////TWBARMGR->DeleteBar("Animation1");
	//
	//CFBXAnimationMesh* pMesh = dynamic_cast<CFBXAnimationMesh* >(m_pRenderContainer->GetMesh(0).get());
	//int ActiveAnimationIndex = 0;
	////m_pAnimater->GetActiveAnimationIndex();
	////m_pAnimater->GetActiveAnimationName();
	//string barNameStr = m_pAnimater->GetCurAnimationInfo()->GetAnimationName();
	//const char* barName = barNameStr.c_str();
	//TWBARMGR->AddMinMaxBarRW(barName, "Animation Info", "Animation Spd", &m_pAnimater->GetCurAnimationInfo()->GetAnimationSpd(), 1.0f, 1000.f, 0.1f);
	//TWBARMGR->AddMinMaxBarRW(barName, "Animation Info", "Animation Frame", &m_pAnimater->GetCurAnimationInfo()->GetCurFrame(), 0.f, m_pAnimater->GetCurAnimationInfo()->GetFrameCnt() - 1, 1.0f);
	//TWBARMGR->AddButtonCB(barName, "Animation Info", "Animation On/Off", AnimationOnOffButtonCallback, &m_pAnimater->GetCurAnimationInfo()->GetbAnimation());
	//TWBARMGR->AddButtonCB(barName, "Animation Info", "Animation Reset", AnimationResetButtonCallback, pMesh);
	//
	////모든 메쉬의 좌표계 ui
	//for (int i = 0; i < m_pRenderContainer->GetvMesh().size(); ++i) {
	//	char groupName[64];
	//	sprintf(groupName, "%s%d %s", "Mesh", i, "info");
	//
	//	TWBARMGR->AddButtonCB(barName, groupName, "Make Select OBB", CreateSelectOBBCallback, pMesh);
	//	TWBARMGR->AddSeparator(barName, groupName, nullptr);
	//	for (int j = 0; j <  m_pAnimater->GetCurAnimationInfo()->GetJoints().size(); ++j) {
	//		//add active bool bar
	//		//모든 메시의 ActiveAnimationIndex
	//		char menuName[64];
	//		sprintf(menuName, "%s", m_pAnimater->GetCurAnimationInfo()->GetJoints()[j].GetJointName().c_str());
	//		
	//		TWBARMGR->AddBoolBar(barName, groupName, menuName, &pMesh->GetOBBObject(j).GetActive());
	//		//TWBARMGR->AddButtonCB("Animation1", groupName, menuName, JointSelectButtonCallback, &pMesh->GetOBBObject(j));
	//		//TWBARMGR->AddBoundingBoxActiveBar("Animation1", groupName, menuName, &pMesh->GetvJoint()[j]);
	//
	//		//TWBARMGR->AddBoundingBoxActiveBar("PickingBar", groupName, boolMenuName, NULL);
	//		
	//	}
	//}
}

CBunny::CBunny() : CGameObject("testobject", tag::TAG_DYNAMIC_OBJECT) {

}
CBunny::~CBunny() {

}