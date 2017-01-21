#include "stdafx.h""
#include "Animater.h"

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


bool CAnimater::Begin(){
	m_vpAnimationInfos.clear();

	m_pMainBoundingBox = new CBoundingBox();
	m_pMainBoundingBox->Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(5.f, 5.f, 5.f, 1.f));

	m_pAnimBuffer = new CStaticBuffer(m_pd3dDevice, m_pd3dDeviceContext);
	m_pAnimBuffer->Begin(256, sizeof(XMMATRIX), nullptr, 10, BIND_VS);

	return true;
}

bool CAnimater::End(){
	for (auto data : m_vpAnimationInfos) {
		delete data;
	}
	m_vpAnimationInfos.clear();

	if (m_pMainBoundingBox) {
		m_pMainBoundingBox->End();
		delete m_pMainBoundingBox;
	}
	return true;
}

void CAnimater::SetShaderState(){
	if (m_vpAnimationInfos.empty()) {
		m_pAnimBuffer->SetShaderState();
		return;
	}
	m_vpAnimationInfos[m_CurAnimationIndex]->SetShaderState();
}

void CAnimater::CleanShaderState(){
	if (m_vpAnimationInfos.empty()) {

		return;
	}
	m_vpAnimationInfos[m_CurAnimationIndex]->CleanShaderState();
}

void CAnimater::Update(float fTimeElapsed){
	if (m_vpAnimationInfos.empty()) {
		void* pData = m_pAnimBuffer->Map();
		XMMATRIX* pAnimationData = static_cast<XMMATRIX*>(pData);

		for (int j = 0; j < 256; ++j) {
			pAnimationData[j] = XMMatrixIdentity();
		}
		m_pAnimBuffer->Unmap();
		return;
	}
	m_vpAnimationInfos[m_CurAnimationIndex]->Update(fTimeElapsed);
}

void CAnimater::AddAnimationInfo(CAnimationInfo * pAnimationInfo){
	//1. 넣고
	m_vpAnimationInfos.push_back(pAnimationInfo);
	
//	//2. 모든 anim info를 가지고. 전체적인 Joint tree를 제작한다.
//	CreateJointTree();
//
//	//3. 모든 anim info에게 새로운 joint tree를 가지고 자신의 joint를 재정렬 할 것을 요청
//	ChangeAllAnimationInfoJointData();
}

//void CAnimater::CreateJointTree() {
//	//모든? x 처음과 끝
//	m_vJointName.clear();
//	int Lastindex = m_vpAnimationInfos.size() - 1;
//	vector<CFbxJointData>::iterator iter;
//	//기준은 항상 처음 메쉬
//	for (auto FJoint : m_vpAnimationInfos[0]->GetJoints()) {
//		m_vJointName.push_back(FJoint.GetJointName());
//	}
//	for (auto LJoint : m_vpAnimationInfos[Lastindex]->GetJoints()) {
//		iter = find_if(m_vpAnimationInfos[0]->GetJoints().begin(), m_vpAnimationInfos[0]->GetJoints().end(), [&LJoint](CFbxJointData& my) {
//			return (my.GetJointName() == LJoint.GetJointName());
//		});
//		if (iter == m_vpAnimationInfos[0]->GetJoints().end()) {
//			//없으면
//			m_vJointName.push_back(LJoint.GetJointName());
//		}
//	}
//}
//
//void CAnimater::ChangeAllAnimationInfoJointData(){
//	for (auto pAnimationInfo : m_vpAnimationInfos) {
//		pAnimationInfo->ChangeJointData(m_vJointName);
//	}
//}

void CAnimater::DeleteAnimationInfo(UINT AnimationIndex){
	vector<CAnimationInfo*>::iterator iter;
	for (iter = m_vpAnimationInfos.begin(); iter != m_vpAnimationInfos.end(); ++iter) {
		if ((*iter)->GetAnimationIndex() == AnimationIndex) break;
	}
	if (iter != m_vpAnimationInfos.end()) {
		delete (*iter);
		m_vpAnimationInfos.erase(iter);
		m_CurAnimationIndex = 0;
		UINT index{ 0 };
		//animation info 재정렬
		for (auto pAnimationInfo : m_vpAnimationInfos) {
			pAnimationInfo->SetAnimationIndex(index++);
		}
		//animation info 재정렬
		CreateAnimationUI();
	}
}

void CAnimater::SetCurAnimationIndex(UINT AnimationIndex) { 

	m_CurAnimationIndex = AnimationIndex; 
	ResetAnimationInfos();
}

void CAnimater::ResetAnimationInfos(){
	for (auto pAnimationInfo : m_vpAnimationInfos) {
		pAnimationInfo->Reset();
	}
}

void CAnimater::CreateAnimationUI(){
	//animation ui
	//이러면 pMesh에 한해서 이렇게 한거잖아? 여러 메쉬면? 모든 애니메이션 목록은!
	//모든 joint 목록은!!! 

	char* barName = "AnimaterInfo";
	TWBARMGR->DeleteBar(barName);
	
	TWBARMGR->AddBar(barName);
	//set param
	TWBARMGR->SetBarSize(barName, 200, 200);
	TWBARMGR->SetBarPosition(barName, 0, 200);
	TWBARMGR->SetBarColor(barName, 200, 200, 0);
	TWBARMGR->SetBarContained(barName, true);
	TWBARMGR->SetBarMovable(barName, false);
	TWBARMGR->SetBarResizable(barName, false);
	//set param


	TWBARMGR->AddPositionBar(barName, "MainAABB", "MainAABB Position", GetMainAABB(), 0.f, SPACE_SIZE - 1.0f, 1.0f);
	TWBARMGR->AddScaleBar(barName, "MainAABB", "MainAABB Scale", GetMainAABB(), 0.1f, 100.f, 0.1f);

	//for(모든 애니메이션에 대해서){
	//TWBARMGR->AddButtonCB("PickingBar", "Animation Select", "Animation name", AnimationSelectCallback, 애니메이션 정보);
	//}
	for (size_t i = 0; i < GetAnimationCnt(); ++i) {
		//string manuNameStr = m_pAnimater->GetAnimationInfo(i)->GetAnimationName();
		char menuName[64];
		sprintf(menuName, "%s %d", "Animation", i);
		TWBARMGR->AddButtonCB(barName, "Animation Select", menuName, AnimationSelectCallback, GetAnimationInfo(i));
	}
}

UINT CAnimater::GetAnimaterJointCnt(){
	UINT JointCnt{ 0 };

	if (false == m_vpAnimationInfos.empty()) {
		JointCnt = m_vpAnimationInfos[0]->GetJoints().size();
	}

	return JointCnt;
}

CAnimater::CAnimater(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject("animater", pd3dDevice, pd3dDeviceContext) {
}

CAnimater::~CAnimater(){

}