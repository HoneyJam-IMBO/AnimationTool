#pragma once
#include "Object.h"
#include "KeyFrame.h"
#include "BoundingBox.h"

struct ActiveTime {
	UINT m_nMin;
	UINT m_nMax;

	bool operator==(ActiveTime& other){
		if (other.m_nMin == m_nMin && other.m_nMax == m_nMax) return true;
		return false;
	}

	ActiveTime(UINT nMin, UINT nMax) : m_nMin(nMin), m_nMax(nMax) {}

};
class CFbxJointData : public CGameObject {
public:
	void SetJointName(string name) { m_sName = name; }
	void SetParentIndex(UINT index) { m_ParentIndex = index; }
	void SetMyIndex(UINT index) { m_MyIndex = index; }
	void SetOffsetMtx(XMMATRIX mtx) { m_xmmtxOffset = mtx; }
	
	string GetJointName() { return m_sName; }
	UINT GetParentIndex() { return m_ParentIndex; }
	UINT GetMyIndex() { return m_MyIndex; }
	XMMATRIX GetOffsetMtx() { return m_xmmtxOffset; }

	vector<CKeyFrame>& GetKeyFrames() { return m_vKeyFrame; }
//	list<ActiveTime>& GetActiveTimes() { return m_lActiveTime; }
//	void ProcOn(UINT nMin, UINT nMax) { m_lActiveTime.push_back(ActiveTime(nMin, nMax)); }
//	void ProcOff(UINT nMin) {
//		UINT nMax = m_FrameCnt;
//		if (nMin > nMax) return;
//
//		bool remove = false;
//
//		list<ActiveTime>::iterator iter;
//		for (iter = m_lActiveTime.begin(); iter != m_lActiveTime.end(); ++iter) {
//			if (nMin > (*iter).m_nMin && nMin <= (*iter).m_nMax) {
//				(*iter).m_nMax = nMin - 1;//그 active time의 max값을 지금 시간으로 바꾸고 리턴
//				return;
//			}
//			else if (nMin == (*iter).m_nMin) {//지금 시간이 min값과 같은게 있다면
//				break;
//			}
//		}
//
//		if (iter != m_lActiveTime.end()) {
//			m_lActiveTime.erase(iter);
//		}
//	}
//	void ProcOn(UINT nMin) {
//		UINT nMax = m_FrameCnt;
//		if (nMin > nMax) return;
//
//		list<ActiveTime>::iterator iter;
//		for (iter = m_lActiveTime.begin(); iter != m_lActiveTime.end(); ++iter) {
//			if (nMin < (*iter).m_nMin) {
//				nMax = (*iter).m_nMin;
//				break;
//			}
//		}
//		m_lActiveTime.insert(iter, ActiveTime(nMin, nMax));
//	}
//	bool IsActiveTime(UINT nTime) {
//		for (auto data : m_lActiveTime) 
//			if (nTime >= data.m_nMin && nTime <= data.m_nMax) return true;
//
//		return false;
//	}

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
	XMMATRIX m_xmmtxOffset;
public:
	CFbxJointData() :CGameObject("fbxjointdata") {
	};
	~CFbxJointData() {}
};
