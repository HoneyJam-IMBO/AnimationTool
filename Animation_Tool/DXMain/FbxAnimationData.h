#pragma once
#include "Object.h"
#include "FbxBlendWeightPair.h"
#include "FbxJointData.h"

class CFbxAnimationData : public CObject {
public:
	void SetGeometryTransform(FbxAMatrix mtx) { m_FbxAGeomeryTransformMtx = mtx; }
	FbxAMatrix& GetGeometryTransform() { return m_FbxAGeomeryTransformMtx; }

	UINT GetJointCnt() { return static_cast<UINT>(m_JointDatas.size()); }
	vector<CFbxJointData>& GetJointDatas() { return m_JointDatas; }


private:
	vector<CFbxJointData> m_JointDatas;

	FbxAMatrix m_FbxAGeomeryTransformMtx;
public:
	CFbxAnimationData() : CObject("fbxskinneddata") {}
	~CFbxAnimationData() {}
};