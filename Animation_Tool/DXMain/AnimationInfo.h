#pragma once
#include "DXObject.h"
#include "FbxJointData.h"
#include "KeyFrame.h"
#include "StaticBuffer.h"
#include "BoundingBox.h"

class CAnimationInfo :public DXObject {
public:
	bool Begin(UINT AniamationIndex);
	virtual bool End();
	virtual void SetShaderState(UINT MeshIndex = 0);
	virtual void CleanShaderState();

	virtual void Update(float fTimeElapsed);

	float& GetCurFrame() { return m_CurFrame; }
	int& GetFrameCnt() { return m_FrameCnt; }
	bool& GetbAnimation() { return m_bAnimation; }
	float& GetAnimationSpd() { return m_fAnimationSpd; }
	string GetAnimationName() { return m_AnimationName; }
	UINT GetAnimationIndex() { return m_AnimationIndex; }

	XMMATRIX GetCurFrameMtx(UINT JointIndex, UINT meshIndex = 0) { return m_mMeshIndexJoints[meshIndex][JointIndex].GetKeyFrames()[m_CurFrame].GetKeyFrameTransformMtx(); };
	vector<CFbxJointData>& GetJoints(UINT meshIndex = 0) { return m_mMeshIndexJoints[meshIndex]; }

	void SelectAnimationProc();
private:
	//animation 
	FbxLongLong m_AnimationLength;
	std::string m_AnimationName;
	UINT m_AnimationIndex{ 0 };

	//buffer
	CStaticBuffer* m_pAnimBuffer{ nullptr };

	map<UINT, vector<CFbxJointData>> m_mMeshIndexJoints;
	vector<CBoundingBox> m_vBoundingBox;

	float m_CurFrame{ 0 };
	int m_FrameCnt{ 0 };
	bool m_bAnimation{ true };
	float m_fAnimationSpd{ 1.0f };
	
public:
	CAnimationInfo(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	~CAnimationInfo();

};