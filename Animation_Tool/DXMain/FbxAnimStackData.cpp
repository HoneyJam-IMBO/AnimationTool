#include "stdafx.h"
#include "FbxAnimStackData.h"

bool CFbxAnimStackData::End(){
	m_BoneCnt = 0;
	m_pAnimStack->Destroy();
	m_pAnimStack = nullptr;

	m_tStart = 0;
	m_tEnd = 0;
	m_tAnimLength = 0;

	m_JointDatas.clear();
	return true;
}
