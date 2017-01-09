#pragma once

#include "GameObject.h"


struct VS_VB_BOUNDINGBOX_INSTANCE {
	XMFLOAT3 m_xmf3Pos;
	XMFLOAT3 m_xmf3Extend;
	XMFLOAT4 m_xmf4Quaternion;
};

class CBoundingBox : public CGameObject{
public:
	//debug buffer controll
	virtual void SetDebugBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);
	//buffer controll
	virtual void SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);

	void SetBoundingBoxInfo(BoundingBox& aabb);
	void SetBoundingBoxInfo(BoundingOrientedBox& obb);

	BoundingBox GetAABB();
	BoundingOrientedBox GetOBB();

	virtual bool IsVisible(shared_ptr<CCamera> pCamera) { return true; };//계층구조의 녀석들은 다시 만들어줄 필요가 있음

	float& GetMin() { return m_fMin; }
	float& GetMax() { return m_fMax; }

private:
	float m_fMin;
	float m_fMax;

public:
	CBoundingBox();
	~CBoundingBox();
};
