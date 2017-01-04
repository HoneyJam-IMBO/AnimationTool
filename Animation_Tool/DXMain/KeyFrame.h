#pragma once
#include "Object.h"

class CKeyFrame : public CObject {
public:
	void SetKeyFrameTransformMtx(FbxAMatrix& mtx) { m_FbxATransformMtx = mtx; }
	FbxAMatrix& GetKeyFrameTransformMtx() { return m_FbxATransformMtx; }
	void SetFrameTime(double time) { m_dFrameTime = time; }
	double GetFrameTime() { return m_dFrameTime; }

private:
	FbxAMatrix m_FbxATransformMtx;
	double m_dFrameTime;

public:
	CKeyFrame() : CObject("keyframe") {};
	CKeyFrame(double curFrameTime, FbxAMatrix mtx) : CObject("keyframe"),
		m_dFrameTime(curFrameTime), m_FbxATransformMtx(mtx) {};
	~CKeyFrame() {};
};