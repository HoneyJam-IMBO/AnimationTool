#include "stdafx.h"
#include "AnimationInfo.h"

bool CAnimationInfo::Begin(UINT AniamationIndex){
	m_AnimationIndex = AniamationIndex;
	m_FrameCnt = static_cast<int>(FBXIMPORTER->GetAnimStackData().GetAnimationLength());
	m_AnimationName = FBXIMPORTER->GetAnimStackData().GetAnimationName();
	int nJoint{ 0 };

	for (int MeshIndex = 0; MeshIndex < FBXIMPORTER->GetAnimationDatas().size(); ++MeshIndex) {
		nJoint = static_cast<int>(FBXIMPORTER->GetAnimationDatas()[MeshIndex].GetJointCnt());

		for (int j = 0; j < nJoint; ++j) {
			XMMATRIX FrameTransform;
			m_mMeshIndexJoints[0].push_back(FBXIMPORTER->GetAnimationDatas()[MeshIndex].GetJointDatas()[j]);
		}
	}

	m_pAnimBuffer = new CStaticBuffer(m_pd3dDevice, m_pd3dDeviceContext);
	m_pAnimBuffer->Begin(1, 6128, nullptr, 10, BIND_VS);
	return true;
}

bool CAnimationInfo::End(){
	int MeshNum = m_mMeshIndexJoints.size();
	for (int i = 0; i<MeshNum; ++i) {
		m_mMeshIndexJoints[i].clear();
	}
	m_mMeshIndexJoints.clear();

	return true;

}

void CAnimationInfo::SetShaderState(UINT MeshIndex){
	void* pData = m_pAnimBuffer->Map();
	XMMATRIX* pAnimationData = static_cast<XMMATRIX*>(pData);

	for (int j = 0; j < m_mMeshIndexJoints[MeshIndex].size(); ++j) {
		pAnimationData[j] = XMMatrixTranspose(m_mMeshIndexJoints[MeshIndex][j].GetOffsetMtx() * m_mMeshIndexJoints[MeshIndex][j].GetKeyFrames()[m_CurFrame].GetKeyFrameTransformMtx());
		//		if (m_vOBB[j].GetActive()) {
		//	
		//			DEBUGER->RegistCoordinateSys(m_ppAnimationData[static_cast<int>(m_nFrame)][j]);
		//			BoundingOrientedBox originObb = m_vOBB[j].GetOBB();
		//			originObb.Transform(originObb, m_ppAnimationData[static_cast<int>(m_nFrame)][j]);
		//			DEBUGER->RegistOBB(originObb);
		//		}
	}

	m_pAnimBuffer->Unmap();
	m_pAnimBuffer->SetShaderState();
}

void CAnimationInfo::CleanShaderState(){
	m_pAnimBuffer->CleanShaderState();
}

void CAnimationInfo::Update(float fTimeElapsed){
	//update animation data
	if (m_bAnimation) {
		m_CurFrame = m_CurFrame + (fTimeElapsed*m_fAnimationSpd);
	}
	if (m_CurFrame > m_FrameCnt) {
		m_CurFrame = 0.f;
	}
}

void CAnimationInfo::SelectAnimationProc(){
//	const char* barName = m_AnimationName.c_str();
//	TWBARMGR->AddMinMaxBarRW(barName, "Animation Info", "Animation Spd", &m_fAnimationSpd, 1.0f, 1000.f, 0.1f);
//	TWBARMGR->AddMinMaxBarRW(barName, "Animation Info", "Animation Frame", &m_CurFrame, 0.f, m_FrameCnt - 1, 1.0f);
//	TWBARMGR->AddButtonCB(barName, "Animation Info", "Animation On/Off", AnimationOnOffButtonCallback, &m_bAnimation);
//	TWBARMGR->AddButtonCB(barName, "Animation Info", "Animation Reset", AnimationResetButtonCallback, &m_CurFrame);
//
//	//모든 메쉬의 좌표계 ui
//	//for (int i = 0; i < m_pRenderContainer->GetvMesh().size(); ++i) {
//		char groupName[64];
//		sprintf(groupName, "%s%d %s", "Mesh", m_AnimationIndex, "info");
//
//		TWBARMGR->AddButtonCB(barName, groupName, "Make Select OBB", MakeSelectOBBCallback, m_pMesh);
//		TWBARMGR->AddSeparator(barName, groupName, nullptr);
//		for (int j = 0; j < m_vJoints.size(); ++j) {
//			//add active bool bar
//			char menuName[64];
//			sprintf(menuName, "%s", m_vJoints[j].GetJointName().c_str());
//
//			TWBARMGR->AddBoolBar(barName, groupName, menuName, &m_pMesh->GetOBBObject(j).GetActive());
//			//TWBARMGR->AddButtonCB("Animation1", groupName, menuName, JointSelectButtonCallback, &pMesh->GetOBBObject(j));
//			//TWBARMGR->AddBoundingBoxActiveBar("Animation1", groupName, menuName, &pMesh->GetvJoint()[j]);
//
//			//TWBARMGR->AddBoundingBoxActiveBar("PickingBar", groupName, boolMenuName, NULL);
//
//		}
//	//}
}

CAnimationInfo::CAnimationInfo(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject("animationinfo", pd3dDevice, pd3dDeviceContext){

}

CAnimationInfo::~CAnimationInfo()
{
}
