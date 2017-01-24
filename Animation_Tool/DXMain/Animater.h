#pragma once
#include "DXObject.h"
#include "AnimationInfo.h"
#include "SkeletonData.h"
#include "BoundingBox.h"

class CAnimater : public DXObject {
public:
	bool Begin();
	virtual bool End();
	virtual void SetShaderState();
	virtual void CleanShaderState();

	void Update(float fTimeElapsed);

	void AddAnimationInfo(CAnimationInfo* pAnimationInfo);
	//void CreateJointTree();
	//void ChangeAllAnimationInfoJointData();
	void DeleteAnimationInfo(UINT AnimationIndex);

	void SetCurAnimationIndex(UINT AnimationIndex);
	UINT GetCurAnimationIndex() { return m_CurAnimationIndex; }

	CAnimationInfo* GetCurAnimationInfo() { return m_vpAnimationInfos[m_CurAnimationIndex]; }
	CAnimationInfo* GetAnimationInfo(UINT AnimationIndex = 0) { return m_vpAnimationInfos[AnimationIndex]; }
	CSkeletonData* GetSkeletonData() { return m_pSkeletonData; }

	size_t GetAnimationCnt() { return m_vpAnimationInfos.size(); }

	CBoundingBox* GetMainAABB() { return m_pMainBoundingBox;}
	
	//util
	void ResetAnimationInfos();
	void CreateAnimationUI();

	//helper func
	UINT GetAnimaterJointCnt();
private:
	vector<CAnimationInfo*> m_vpAnimationInfos;
	CSkeletonData* m_pSkeletonData{ nullptr };
	UINT m_CurAnimationIndex{ 0 };

	CBoundingBox* m_pMainBoundingBox{ nullptr };
//	//joint tree Á¤º¸
//	vector<string> m_vJointName;

	//buffer
	CStaticBuffer* m_pAnimBuffer{ nullptr };

public:
	CAnimater(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	~CAnimater();

};