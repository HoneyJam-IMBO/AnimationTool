#pragma once
#include "Object.h"
#include "FbxBlendWeightPair.h"
#include "FbxJointData.h"

class CFbxAnimStackData : public CObject {
public:
	void SetpAnimStack(FbxAnimStack* pStack) { m_pAnimStack = pStack; }
	void SetBoneCnt(UINT cnt) { m_BoneCnt = cnt; }
	void SetAnimationName(string name) { m_sName = name; }
	void SetAnimationLength(FbxLongLong length) { m_tAnimLength = length; }
	void SetTimeMode(FbxTime::EMode& timeMode) { m_TimeMode = timeMode; }
	void SetTimeStart(FbxTime& time) { m_tStart = time; }
	void SetTimeEnd(FbxTime& time) { m_tEnd = time; }

	FbxAnimStack* GetpAnimStack() { return m_pAnimStack; }
	FbxTime::EMode& GetTimeMode() { return m_TimeMode; }
	FbxLongLong GetTimeStart() { return m_tStart.GetFrameCount(FbxTime::eFrames30); }
	FbxLongLong GetTimeEnd() { return m_tEnd.GetFrameCount(FbxTime::eFrames30); }
	string GetAnimationName() { return m_sName; }
	FbxLongLong GetAnimationLength() { return m_tAnimLength; }

private:
	UINT m_BoneCnt{ 0 };
	string m_sName;
	FbxAnimStack* m_pAnimStack;

	FbxTime::EMode m_TimeMode;
	FbxTime m_tStart;
	FbxTime m_tEnd;
	FbxLongLong m_tAnimLength;
public:
	CFbxAnimStackData() : CObject("fbxskinneddata") {}
	~CFbxAnimStackData() {}
};