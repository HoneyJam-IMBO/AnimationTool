#include "stdafx.h""
#include "Animater.h"

bool CAnimater::Begin(){
	m_vpAnimationInfos.clear();

	return true;
}

bool CAnimater::End(){
	for (auto data : m_vpAnimationInfos) {
		delete data;
	}
	m_vpAnimationInfos.clear();

	return true;
}

void CAnimater::SetShaderState(UINT MeshIndex){
	if (m_vpAnimationInfos.empty() || m_vpAnimationInfos[m_CurAnimationIndex]->GetAnimationInfos().size() < MeshIndex) {

		return;
	}
	m_vpAnimationInfos[m_CurAnimationIndex]->SetShaderState(MeshIndex);
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
	m_vpAnimationInfos.push_back(pAnimationInfo);
}

void CAnimater::DeleteAnimationInfo(UINT AnimationIndex){
	vector<CAnimationInfo*>::iterator iter;
	for (iter = m_vpAnimationInfos.begin(); iter != m_vpAnimationInfos.end(); ++iter) {
		if ((*iter)->GetAnimationIndex() == m_CurAnimationIndex) break;
	}
	if (iter != m_vpAnimationInfos.end()) m_vpAnimationInfos.erase(iter);
}

CAnimater::CAnimater(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject("animater", pd3dDevice, pd3dDeviceContext) {
}

CAnimater::~CAnimater(){

}