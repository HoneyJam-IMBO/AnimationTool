#include "stdafx.h""
#include "Animater.h"

bool CAnimater::Begin(){
	m_vpAnimationInfos.clear();

	m_pMainBoundingBox = new CBoundingBox();
	m_pMainBoundingBox->Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(5.f, 5.f, 5.f, 1.f));
	return true;
}

bool CAnimater::End(){
	for (auto data : m_vpAnimationInfos) {
		delete data;
	}
	m_vpAnimationInfos.clear();

	return true;
}

void CAnimater::SetShaderState(){
	if (m_vpAnimationInfos.empty()) {

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
		return;
	}
	m_vpAnimationInfos[m_CurAnimationIndex]->Update(fTimeElapsed);
}

void CAnimater::AddAnimationInfo(CAnimationInfo * pAnimationInfo){

	if (m_vpAnimationInfos.empty() || //empty�� �ƴ϶��
		pAnimationInfo->GetJoints().size() == GetAnimaterJointCnt()) {
		m_vpAnimationInfos.push_back(pAnimationInfo);
		return;
	}
	
	if (GetAnimaterJointCnt() < pAnimationInfo->GetJoints().size()) {
		//������ joint�� ���� ���� ���� joint���� �� ���ٸ� 
		//������ AnimationInfo�鿡�� JointTree ������ ��û
		for (auto data : m_vpAnimationInfos) {
			data->ChangeJointData(pAnimationInfo);
		}
	}
	else {
		//������ joinnt���� ���� ���� joint�� ���� ���ٸ�
		//���� ���� AnimationInfo���� JointTree ������ ��û
		pAnimationInfo->ChangeJointData(m_vpAnimationInfos[0]);
	}
	
	m_vpAnimationInfos.push_back(pAnimationInfo);
}

void CAnimater::DeleteAnimationInfo(UINT AnimationIndex){
	vector<CAnimationInfo*>::iterator iter;
	for (iter = m_vpAnimationInfos.begin(); iter != m_vpAnimationInfos.end(); ++iter) {
		if ((*iter)->GetAnimationIndex() == m_CurAnimationIndex) break;
	}
	if (iter != m_vpAnimationInfos.end()) m_vpAnimationInfos.erase(iter);
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