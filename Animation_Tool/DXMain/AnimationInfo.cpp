#include "stdafx.h"
#include "AnimationInfo.h"

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
	CAnimationInfo* pAnimationInfo = reinterpret_cast<CAnimationInfo*>(clientData);
	pAnimationInfo->GetCurFrame() = 0;
}

void TW_CALL DeleteOBBButtonCallback(void * clientData) {
	
	CBoundingBox* pBoundingBox = reinterpret_cast<CBoundingBox*>(clientData);
	char deleteGroupName[64];
	sprintf(deleteGroupName, "%s%d", "OBB", pBoundingBox->GetMyIndex());

	pBoundingBox->SetToolActive(false);

	//TWBARMGR->DeleteVar("Active Joint", deleteGroupName);
}

void TW_CALL CreateSelectOBBCallback(void * clientData) {
	CAnimationInfo* pAnimationInfo = reinterpret_cast<CAnimationInfo*>(clientData);
	
	char* barName = "Active Joint";
	int TempOBBCnt{ 0 };
	//for(int j = 0; j<pAnimationInfo->GetAnimationInfos().size(); ++j){
		for (int i = 0; i < pAnimationInfo->GetJoints().size(); ++i) {
			if (pAnimationInfo->GetTempOBB()[TempOBBCnt++].GetActive()) {
				CBoundingBox* pBoundingBox = new CBoundingBox();
				pBoundingBox->Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(2.f, 2.f, 2.f, 1.f));
				//pBoundingBox->SetMyMeshIndex(j);
				pBoundingBox->SetMyJointIndex(i);
				pBoundingBox->SetMyIndex(pAnimationInfo->GetActiveOBB().size());

				pAnimationInfo->GetActiveOBB().push_back(pBoundingBox);

				//add obb position bar
				char GroupName[64];
				//sprintf(positionGroupName, "%s%d", "OBB", pBoundingBox->GetMyIndex());
				sprintf(GroupName, "%s%d", pAnimationInfo->GetJoints()[i].GetJointName().c_str() , pBoundingBox->GetMyIndex());
				char positionMenuName[64];
				sprintf(positionMenuName, "%s %s", GroupName, "Position");
				TWBARMGR->AddSeparator(barName, GroupName, nullptr);
				TWBARMGR->AddPositionBar(barName, GroupName, positionMenuName, pBoundingBox,
					-100.f, 100.f, 0.1f);
				//add obb scale bar
				char scaleMenuName[64];
				sprintf(scaleMenuName, "%s %s", GroupName, "Scale");
				TWBARMGR->AddSeparator(barName, GroupName, nullptr);
				TWBARMGR->AddScaleBar(barName, GroupName, scaleMenuName, pBoundingBox,
					0.1f, 100.f, 0.1f);

				//add min max bar
				TWBARMGR->AddSeparator(barName, GroupName, nullptr);

				char minmaxMenuName[64];
				sprintf(minmaxMenuName, "%s%s", GroupName, " Min");
				TWBARMGR->AddMinMaxBarRW(barName, GroupName, minmaxMenuName, &pBoundingBox->GetMin(),
					0.1f, pAnimationInfo->GetFrameCnt(), 0.1f);
				sprintf(minmaxMenuName, "%s%s", GroupName, " Max");
				TWBARMGR->AddMinMaxBarRW(barName, GroupName, minmaxMenuName, &pBoundingBox->GetMax(),
					0.1f, pAnimationInfo->GetFrameCnt(), 0.1f);

				//add obb delete button
				char deleteMenuName[64];
				sprintf(deleteMenuName, "%s %s", GroupName, "Delete");
				TWBARMGR->AddSeparator(barName, GroupName, nullptr);
				TWBARMGR->AddButtonCB(barName, GroupName, deleteMenuName, DeleteOBBButtonCallback, pBoundingBox);
			}
		}
	//}

}
bool CAnimationInfo::Begin(UINT AniamationIndex){
	m_AnimationIndex = AniamationIndex;
	m_FrameCnt = static_cast<int>(FBXIMPORTER->GetAnimStackData().GetAnimationLength());
	m_AnimationName = FBXIMPORTER->GetAnimStackData().GetAnimationName();
	int nJoint{ 0 };

		nJoint = FBXIMPORTER->GetAnimStackData().GetJointDatas().size();

		for (int j = 0; j < nJoint; ++j) {
			XMMATRIX FrameTransform;
			m_vJoints.push_back(FBXIMPORTER->GetAnimStackData().GetJointDatas()[j]);
			CBoundingBox boundingBox;
			boundingBox.Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(2.f, 2.f, 2.f, 1.f));
			boundingBox.SetActive(false);
			m_vTempBoundingBox.push_back(boundingBox);
		}
	


//	for (int MeshIndex = 0; MeshIndex < FBXIMPORTER->GetAnimationDatas().size(); ++MeshIndex) {
//		nJoint = static_cast<int>(FBXIMPORTER->GetAnimationDatas()[MeshIndex].GetJointCnt());
//
//		for (int j = 0; j < nJoint; ++j) {
//			XMMATRIX FrameTransform;
//			m_mMeshIndexJoints[MeshIndex].push_back(FBXIMPORTER->GetAnimationDatas()[MeshIndex].GetJointDatas()[j]);
//			CBoundingBox boundingBox;
//			boundingBox.Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(2.f, 2.f, 2.f, 1.f));
//			boundingBox.SetActive(false);
//			m_vTempBoundingBox.push_back(boundingBox);
//		}
//	}

	m_pAnimBuffer = new CStaticBuffer(m_pd3dDevice, m_pd3dDeviceContext);
	m_pAnimBuffer->Begin(256, sizeof(XMMATRIX), nullptr, 10, BIND_VS);
	return true;
}

bool CAnimationInfo::End(){
	m_vJoints.clear();

	//int MeshNum = m_mMeshIndexJoints.size();
	//for (int i = 0; i<MeshNum; ++i) {
	//	m_mMeshIndexJoints[i].clear();
	//}
	//m_mMeshIndexJoints.clear();

	return true;

}

void CAnimationInfo::SetShaderState(){
	void* pData = m_pAnimBuffer->Map();
	XMMATRIX* pAnimationData = static_cast<XMMATRIX*>(pData);

	for (int j = 0; j < m_vJoints.size(); ++j) {

		//pAnimationData[j] = XMMatrixIdentity();
		if (m_vJoints[j].GetKeyFrames().empty()) {
			continue;
		}
		pAnimationData[j] = XMMatrixTranspose(m_vJoints[j].GetOffsetMtx() * m_vJoints[j].GetKeyFrames()[m_CurFrame].GetKeyFrameTransformMtx());
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
	vector<CBoundingBox*> vDeleteBoundingBox;
	int DeleteBoxCnt{ 0 };
	//debug
	for (int tempOBBCnt = 0; tempOBBCnt < m_vTempBoundingBox.size(); ++tempOBBCnt) {
		if (m_vTempBoundingBox[tempOBBCnt].GetActive()) {
			if(false == m_vJoints[tempOBBCnt].GetKeyFrames().empty())
				DEBUGER->RegistCoordinateSys(m_vJoints[tempOBBCnt].GetKeyFrames()[m_CurFrame].GetKeyFrameTransformMtx());
		}
	}
	//debug
	for (auto data : m_lActiveBoundingBox) {
		float fMin = data->GetMin();
		float fMax = data->GetMax();
		if (fMin <= m_CurFrame && m_CurFrame <= fMax) data->SetActive(true);
		else data->SetActive(false);

		if (data->GetToolActive()) {//ui 삭제 로직 
			
			if (data->GetActive()) {
				BoundingOrientedBox originObb = data->GetOBB();
				if (false == m_vJoints[data->GetMyJointIndex()].GetKeyFrames().empty()) {
					originObb.Transform(originObb, m_vJoints[data->GetMyJointIndex()].GetKeyFrames()[m_CurFrame].GetKeyFrameTransformMtx());
					DEBUGER->RegistOBB(originObb);
				}
			}
		}
		else {
			vDeleteBoundingBox.push_back(data);
		}
	}
	if (false == vDeleteBoundingBox.empty()) {
		for (auto data : vDeleteBoundingBox) {
			m_lActiveBoundingBox.remove(data);
		}
		DeleteActiveJointProc();
	}
	//update animation data
	if (m_bAnimation) {
		m_CurFrame = m_CurFrame + (fTimeElapsed*m_fAnimationSpd);
	}
	if (m_CurFrame > m_FrameCnt) {
		m_CurFrame = 0.f;
	}
}

void CAnimationInfo::SelectAnimationProc(){
	const char* barName = m_AnimationName.c_str();
	TWBARMGR->AddMinMaxBarRW(barName, "Animation Info", "Animation Spd", &m_fAnimationSpd, 1.0f, 1000.f, 0.1f);
	TWBARMGR->AddMinMaxBarRW(barName, "Animation Info", "Animation Frame", &m_CurFrame, 0.f, m_FrameCnt - 1, 1.0f);
	TWBARMGR->AddButtonCB(barName, "Animation Info", "Animation On/Off", AnimationOnOffButtonCallback, &m_bAnimation);
	TWBARMGR->AddButtonCB(barName, "Animation Info", "Animation Reset", AnimationResetButtonCallback, this);

	//create active obb button
	TWBARMGR->AddButtonCB(barName, "Button", "Create Select OBB", CreateSelectOBBCallback, this);

	//모든 메쉬의 좌표계 ui
	int JointIndex{ 0 };
	//for (int i = 0; i < m_mMeshIndexJoints.size(); ++i) {//모든 메쉬의
		char groupName[64];
		sprintf(groupName, "%s%d %s", "Animation", m_AnimationIndex, "info");

		TWBARMGR->AddSeparator(barName, groupName, nullptr);
		for (int j = 0; j < m_vJoints.size(); ++j) {//모든 joint
			//add active bool bar
			char menuName[64];
			sprintf(menuName, "%s", m_vJoints[j].GetJointName().c_str());

			TWBARMGR->AddBoolBar(barName, groupName, menuName, &m_vTempBoundingBox[JointIndex++].GetActive());
			//TWBARMGR->AddButtonCB("Animation1", groupName, menuName, JointSelectButtonCallback, &pMesh->GetOBBObject(j));
			//TWBARMGR->AddBoundingBoxActiveBar("Animation1", groupName, menuName, &pMesh->GetvJoint()[j]);

			//TWBARMGR->AddBoundingBoxActiveBar("PickingBar", groupName, boolMenuName, NULL);

		}
	//}
}

void CAnimationInfo::DeleteActiveJointProc(){
	//기존 유아이 다 지움
	TWBARMGR->DeleteBar("Active Joint");

	
	CAnimationInfo* pAnimationInfo = this;

	char* barName = "Active Joint";
	int TempOBBCnt{ 0 };
	for (auto data : m_lActiveBoundingBox) {
		CBoundingBox* pBoundingBox = data;
		pBoundingBox->SetMyIndex(TempOBBCnt++);
		//add obb position bar
		char positionGroupName[64];
		sprintf(positionGroupName, "%s%d", "OBB", pBoundingBox->GetMyIndex());
		char positionMenuName[64];
		sprintf(positionMenuName, "%s %s", positionGroupName, "Position");
		TWBARMGR->AddSeparator(barName, positionGroupName, nullptr);
		TWBARMGR->AddPositionBar(barName, positionGroupName, positionMenuName, pBoundingBox,
			-100.f, 100.f, 0.1f);
		//add obb scale bar
		char scaleGroupName[64];
		sprintf(scaleGroupName, "%s%d", "OBB", pBoundingBox->GetMyIndex());
		char scaleMenuName[64];
		sprintf(scaleMenuName, "%s %s", scaleGroupName, "Scale");
		TWBARMGR->AddSeparator(barName, scaleGroupName, nullptr);
		TWBARMGR->AddScaleBar(barName, scaleGroupName, scaleMenuName, pBoundingBox,
			0.1f, 100.f, 0.1f);

		//add min max bar
		char minmaxGroupName[64];
		sprintf(minmaxGroupName, "%s%d", "OBB", pBoundingBox->GetMyIndex());
		TWBARMGR->AddSeparator(barName, minmaxGroupName, nullptr);

		char minmaxMenuName[64];
		sprintf(minmaxMenuName, "%s%s", minmaxGroupName, " Min");
		TWBARMGR->AddMinMaxBarRW(barName, minmaxGroupName, minmaxMenuName, &pBoundingBox->GetMin(),
			0.1f, pAnimationInfo->GetFrameCnt(), 0.1f);
		sprintf(minmaxMenuName, "%s%s", minmaxGroupName, " Max");
		TWBARMGR->AddMinMaxBarRW(barName, minmaxGroupName, minmaxMenuName, &pBoundingBox->GetMax(),
			0.1f, pAnimationInfo->GetFrameCnt(), 0.1f);

		//add obb delete button
		char deleteGroupName[64];
		sprintf(deleteGroupName, "%s%d", "OBB", pBoundingBox->GetMyIndex());
		char deleteMenuName[64];
		sprintf(deleteMenuName, "%s %s", deleteGroupName, "Delete");
		TWBARMGR->AddSeparator(barName, deleteGroupName, nullptr);
		TWBARMGR->AddButtonCB(barName, deleteGroupName, deleteMenuName, DeleteOBBButtonCallback, pBoundingBox);
	}
}

CAnimationInfo::CAnimationInfo(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject("animationinfo", pd3dDevice, pd3dDeviceContext){

}

CAnimationInfo::~CAnimationInfo()
{
}
