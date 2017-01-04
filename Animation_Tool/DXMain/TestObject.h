#pragma once
#include "GameObject.h"
#include "RotateComponent.h"

class CTestObject : public CGameObject {

public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------dxobject-----------------------------

	virtual void SetPosition(XMVECTOR pos);
private:


public:
	CTestObject();
	~CTestObject();
};
