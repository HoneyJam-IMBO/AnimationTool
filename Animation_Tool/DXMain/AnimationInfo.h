#pragma once
#include "DXObject.h"
#include "FbxJointData.h"
#include "KeyFrame.h"
#include "StaticBuffer.h"
#include "BoundingBox.h"

class CAnimater;

class CAnimationInfo :public DXObject {
public:
	bool Begin(shared_ptr<CAnimater> pAnimater);
	virtual bool End();
	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void Update(float fTimeElapsed);

	float& GetCurFrame() { return m_CurFrame; }
	int& GetFrameCnt() { return m_FrameCnt; }
	bool& GetbAnimation() { return m_bAnimation; }
	float& GetAnimationSpd() { return m_fAnimationSpd; }
	string GetAnimationName() { return m_AnimationName; }
	UINT GetAnimationIndex() { return m_AnimationIndex; }

	//utill 
	void Reset();

	//joint data 수정 함수
	void ChangeJointData(CAnimationInfo* pAnimationInfo);

	XMMATRIX GetCurFrameMtx(UINT JointIndex, UINT meshIndex = 0) { return m_vJoints[JointIndex].GetKeyFrames()[m_CurFrame].GetKeyFrameTransformMtx(); };
	vector<CFbxJointData>& GetJoints(UINT meshIndex = 0) { return m_vJoints; }
	//map<UINT, vector<CFbxJointData>>& GetAnimationInfos() { return m_mMeshIndexJoints; }
	//ui proc
	void SelectAnimationProc();
	void DeleteActiveJointProc();

	vector<CBoundingBox>& GetTempOBB() { return m_vTempBoundingBox; }
	list<CBoundingBox*>& GetActiveOBB() { return m_lActiveBoundingBox; }
private:
	shared_ptr<CAnimater> m_pAnimater{ nullptr };
	//animation 
	FbxLongLong m_AnimationLength;
	std::string m_AnimationName;
	UINT m_AnimationIndex{ 0 };

	//buffer
	CStaticBuffer* m_pAnimBuffer{ nullptr };

	//map<UINT, vector<CFbxJointData>> m_mMeshIndexJoints;
	vector<CFbxJointData> m_vJoints;
	vector<CBoundingBox> m_vTempBoundingBox;
	list<CBoundingBox*> m_lActiveBoundingBox;

	float m_CurFrame{ 0 };
	int m_FrameCnt{ 0 };
	bool m_bAnimation{ true };
	float m_fAnimationSpd{ 1.0f };
	
public:
	CAnimationInfo(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	~CAnimationInfo();

};