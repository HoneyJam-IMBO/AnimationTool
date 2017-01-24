#include "stdafx.h"
#include "FbxAnimStackData.h"

bool CFbxAnimStackData::Begin(){
	m_AnimationData.Begin();
	m_SkeletonData.Begin();

	return true;
}

bool CFbxAnimStackData::End(){
	m_AnimationData.End();
	m_SkeletonData.End();

	m_pAnimStack->Destroy();
	m_pAnimStack = nullptr;

	m_tStart = 0;
	m_tEnd = 0;

	return true;
}
