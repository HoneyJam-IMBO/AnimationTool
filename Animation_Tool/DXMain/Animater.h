#pragma once
#include "DXObject.h"
#include "AnimationInfo.h"

class CAnimater : public DXObject {
public:
	bool Begin();
	virtual bool End();
	virtual void SetShaderState(UINT MeshIndex = 0);
	virtual void CleanShaderState();

	void Update(float fTimeElapsed);

	void AddAnimationInfo(CAnimationInfo* pAnimationInfo);
	void DeleteAnimationInfo(UINT AnimationIndex);

	void SetCurAnimationIndex(UINT AnimationIndex) { m_CurAnimationIndex = AnimationIndex; }
	UINT GetCurAnimationIndex() { return m_CurAnimationIndex; }

	CAnimationInfo* GetCurAnimationInfo() { return m_vpAnimationInfos[m_CurAnimationIndex]; }
	CAnimationInfo* GetAnimationInfo(UINT AnimationIndex = 0) { return m_vpAnimationInfos[AnimationIndex]; }

	size_t GetAnimationCnt() { return m_vpAnimationInfos.size(); }
private:
	vector<CAnimationInfo*> m_vpAnimationInfos;
	UINT m_CurAnimationIndex{ 0 };

public:
	CAnimater(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	~CAnimater();

};