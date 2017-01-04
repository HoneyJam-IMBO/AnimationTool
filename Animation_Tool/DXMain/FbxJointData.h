#pragma once
#include "Object.h"
#include "KeyFrame.h"

class CFbxJointData : public CObject {
public:
	void SetJointName(string name) { m_sName = name; }
	void SetParentIndex(UINT index) { m_ParentIndex = index; }
	void SetMyIndex(UINT index) { m_MyIndex = index; }
	void SetOffsetMtx(FbxAMatrix mtx) { m_FbxAOffsetMtx = mtx; }
	
	string GetJointName() { return m_sName; }
	UINT GetParentIndex() { return m_ParentIndex; }
	UINT GetMyIndex() { return m_MyIndex; }
	FbxAMatrix GetOffsetMtx() { return m_FbxAOffsetMtx; }

	vector<CKeyFrame>& GetKeyFrames() { return m_vKeyFrame; }
	bool operator==(CFbxJointData& data) {
		return m_sName == data.m_sName;
	}
	bool operator!=(CFbxJointData& data) {
		return m_sName != data.m_sName;
	}
private:
	vector<CKeyFrame> m_vKeyFrame;
	string m_sName;
	UINT m_MyIndex{ 0 };
	UINT m_ParentIndex{ 0 };
	FbxAMatrix m_FbxAOffsetMtx;

public:
	CFbxJointData() :CObject("fbxjointdata") {};
	~CFbxJointData() {}
};
