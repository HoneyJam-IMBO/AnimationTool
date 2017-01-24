#pragma once
#include "DXObject.h"
#include "AnimationData.h"
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
	int& GetFrameCnt() { return m_pAnimationData->GetAnimationLength(); }
	bool& GetbAnimation() { return m_bAnimation; }
	float& GetAnimationSpd() { return m_fAnimationSpd; }
	UINT GetAnimationIndex() { return m_AnimationIndex; }
	void SetAnimationIndex(UINT index) { m_AnimationIndex = index; }

	//utill 
	void Reset();

	//joint data 수정 함수
	//void ChangeJointData(vector<string>& vJointName);

	XMMATRIX GetCurFrameMtx(UINT JointIndex, UINT meshIndex = 0) { return m_pAnimationData->GetKeyFrames(JointIndex)[m_CurFrame].GetKeyFrameTransformMtx(); };
	//map<UINT, vector<CFbxJointData>>& GetAnimationInfos() { return m_mMeshIndexJoints; }
	//ui proc
	void SelectAnimationProc();
	void DeleteActiveJointProc();

	void CreateSelectOBBUI();
	void CreateActiveOBBUI();
	void CreateAnimationInfoUI();

	vector<CBoundingBox>& GetTempOBB() { return m_vTempBoundingBox; }
	list<CBoundingBox*>& GetActiveOBB() { return m_lActiveBoundingBox; }

	shared_ptr<CAnimater> GetAnimater() { return m_pAnimater; }
private:
	shared_ptr<CAnimater> m_pAnimater{ nullptr };
	//animation 
	CAnimationData* m_pAnimationData{ nullptr };
	UINT m_AnimationIndex{ 0 };

	//buffer
	CStaticBuffer* m_pAnimBuffer{ nullptr };

	//map<UINT, vector<CFbxJointData>> m_mMeshIndexJoints;
	vector<CBoundingBox> m_vTempBoundingBox;
	list<CBoundingBox*> m_lActiveBoundingBox;

	float m_CurFrame{ 0 };
	bool m_bAnimation{ true };
	float m_fAnimationSpd{ 1.0f };
	
	//map<int, int> m_mChangeIndex;
public:
	CAnimationInfo(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	~CAnimationInfo();

};