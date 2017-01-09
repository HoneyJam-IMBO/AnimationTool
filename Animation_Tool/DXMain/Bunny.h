#pragma once
#include "GameObject.h"
#include "RotateComponent.h"

struct CTestStruct {
	CFBXAnimationMesh* pMesh;
	CFbxJointData* pJoint;
};
class CBunny : public CGameObject {

public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------dxobject-----------------------------

	virtual void SetPosition(XMVECTOR pos);
	virtual void PickingProc();
private:
	bool test;

public:
	CBunny();
	~CBunny();
};
