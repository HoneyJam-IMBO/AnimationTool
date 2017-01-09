#include "stdafx.h"
#include "Bunny.h"
void TW_CALL AnimationOnOffButtonCallback(void * clientData) {
	bool* pBool = reinterpret_cast<bool*>(clientData);
	if (*pBool) {
		*pBool = false;
	}
	else {
		*pBool = true;
	}
}
void TW_CALL AnimationResetButtonCallback(void * clientData) {
	CFBXAnimationMesh* pMesh = reinterpret_cast<CFBXAnimationMesh*>(clientData);
	pMesh->GetCurFrame() = 0;
}
void TW_CALL AnimationSelectCallback(void * clientData) {
	CBunny* pBunny = reinterpret_cast<CBunny*>(clientData);
	pBunny->SelectAnimationProc();
}
void TW_CALL DeleteOBBButtonCallback(void * clientData) {
	CFbxJointData* pJoint = reinterpret_cast<CFbxJointData*>(clientData);
	TWBARMGR->DeleteVar("Joint", pJoint->GetJointName().c_str());
}

void TW_CALL MakeSelectOBBCallback(void * clientData) {
	CFBXAnimationMesh* pMesh = reinterpret_cast<CFBXAnimationMesh*>(clientData);
	
	for (int i = 0; i < pMesh->GetOBBCnt(); ++i) {
		if (pMesh->GetOBBObject(i).GetActive()) {
			//add obb position bar
			char positionMenuName[64];
			sprintf(positionMenuName, "%s%s", pMesh->GetvJoint()[i].GetJointName().c_str(), " Position");
			TWBARMGR->AddSeparator("Joint", positionMenuName, nullptr);
			TWBARMGR->AddPositionBar("Joint", positionMenuName, positionMenuName, &pMesh->GetOBBObject(i),
				-100.f, 100.f, 0.1f);
			//add obb scale bar
			char scaleMenuName[64];
			sprintf(scaleMenuName, "%s%s", pMesh->GetvJoint()[i].GetJointName().c_str(), " Scale");
			TWBARMGR->AddSeparator("Joint", scaleMenuName, nullptr);
			TWBARMGR->AddScaleBar("Joint", scaleMenuName, scaleMenuName, &pMesh->GetOBBObject(i),
				0.1f, 100.f, 0.1f);

			//add min max bar
			char minmaxGroupName[64];
			sprintf(minmaxGroupName, "%s%s", pMesh->GetvJoint()[i].GetJointName().c_str(), " ActiveTime");
			TWBARMGR->AddSeparator("Joint", minmaxGroupName, nullptr);

			char minmaxMenuName[64];
			sprintf(minmaxMenuName, "%s%s", minmaxGroupName, " Min");
			TWBARMGR->AddMinMaxBarRW("Joint", minmaxGroupName, minmaxMenuName, &pMesh->GetOBBObject(i).GetMin(),
				0.1f, pMesh->GetFrameCnt(), 0.1f);
			sprintf(minmaxMenuName, "%s%s", minmaxGroupName, " Max");
			TWBARMGR->AddMinMaxBarRW("Joint", minmaxGroupName, minmaxMenuName, &pMesh->GetOBBObject(i).GetMax(),
				0.1f, pMesh->GetFrameCnt(), 0.1f);

			//add obb delete button
			char deleteMenuName[64];
			sprintf(deleteMenuName, "%s%s", pMesh->GetvJoint()[i].GetJointName().c_str(), " Delete");
			TWBARMGR->AddSeparator("Joint", deleteMenuName, nullptr);
			TWBARMGR->AddButtonCB("Joint", deleteMenuName, deleteMenuName, DeleteOBBButtonCallback, &pMesh->GetvJoint()[i]);
		}

	}
	
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
	for (int i = 0; i < m_pRenderContainer->GetvMesh().size(); ++i) {
		m_pRenderContainer->GetMesh(i).get()->Update(fTimeElapsed);
	}

}
void CBunny::SetPosition(XMVECTOR pos) {
	XMFLOAT3 xmfPos;
	XMStoreFloat3(&xmfPos, pos);

	m_xmf4x4World._41 = xmfPos.x;
	m_xmf4x4World._42 = xmfPos.y;
	m_xmf4x4World._43 = xmfPos.z;

	if (m_pTerrainContainer) m_xmf4x4World._42 = GetTerrainHeight();

}

void CBunny::PickingProc(){
	//1. 모든 mesh의 목록 
	CGameObject::PickingProc();

	//animation ui
	CFBXAnimationMesh* pMesh = dynamic_cast<CFBXAnimationMesh* >(m_pRenderContainer->GetMesh(0).get());
	TWBARMGR->AddButtonCB("PickingBar", "Animation Select", "Animation 1", AnimationSelectCallback, this);
	
}

void CBunny::SelectAnimationProc(){
	//TWBARMGR->DeleteBar("Animation1");

	CFBXAnimationMesh* pMesh = dynamic_cast<CFBXAnimationMesh* >(m_pRenderContainer->GetMesh(0).get());
	TWBARMGR->AddMinMaxBarRW("Animation1", "Animation Info", "Animation Spd", &pMesh->GetAnimationSpd(), 1.0f, 1000.f, 0.1f);
	TWBARMGR->AddMinMaxBarRW("Animation1", "Animation Info", "Animation Frame", &pMesh->GetCurFrame(), 0.f, pMesh->GetFrameCnt() - 1, 1.0f);
	TWBARMGR->AddButtonCB("Animation1", "Animation Info", "Animation On/Off", AnimationOnOffButtonCallback, &pMesh->GetbAnimation());
	TWBARMGR->AddButtonCB("Animation1", "Animation Info", "Animation Reset", AnimationResetButtonCallback, pMesh);

	//모든 메쉬의 좌표계 ui
	for (int i = 0; i < m_pRenderContainer->GetvMesh().size(); ++i) {
		char groupName[64];
		sprintf(groupName, "%s%d %s", "Mesh", i, "info");

		TWBARMGR->AddButtonCB("Animation1", groupName, "Make Select OBB", MakeSelectOBBCallback, pMesh);
		TWBARMGR->AddSeparator("Animation1", groupName, nullptr);
		for (int j = 0; j < pMesh->GetvJoint().size(); ++j) {
			//add active bool bar
			char menuName[64];
			sprintf(menuName, "%s", pMesh->GetvJoint()[j].GetJointName().c_str());
			
			TWBARMGR->AddBoolBar("Animation1", groupName, menuName, &pMesh->GetOBBObject(j).GetActive());
			//TWBARMGR->AddButtonCB("Animation1", groupName, menuName, JointSelectButtonCallback, &pMesh->GetOBBObject(j));
			//TWBARMGR->AddBoundingBoxActiveBar("Animation1", groupName, menuName, &pMesh->GetvJoint()[j]);

			//TWBARMGR->AddBoundingBoxActiveBar("PickingBar", groupName, boolMenuName, NULL);
			
		}
	}
}

CBunny::CBunny() : CGameObject("testobject", tag::TAG_DYNAMIC_OBJECT) {

}
CBunny::~CBunny() {

}