#pragma once

#include "GameObject.h"


struct VS_VB_BOUNDINGBOX_INSTANCE {
	XMFLOAT3 m_xmf3Pos;
	XMFLOAT3 m_xmf3Extend;
	XMFLOAT4 m_xmf4Quaternion;
};

class CBoundingBox : public CGameObject{
public:
	bool Begin(XMVECTOR Position, XMVECTOR Extend);
	virtual bool End() { return CGameObject::End(); };

	//debug buffer controll
	virtual void SetDebugBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);
	//buffer controll
	virtual void SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);

	void SetBoundingBoxInfo(BoundingBox& aabb);
	void SetBoundingBoxInfo(BoundingOrientedBox& obb);
	void SetBoundingBoxInfo(CBoundingBox& boundingBox);

	BoundingBox GetAABB();
	BoundingOrientedBox GetOBB();

	virtual bool IsVisible(shared_ptr<CCamera> pCamera) { return true; };//계층구조의 녀석들은 다시 만들어줄 필요가 있음

	//test
	float& GetMin() { return m_fMin; }
	float& GetMax() { return m_fMax; }
	void SetMyMeshIndex(int index) { m_nMyMeshIndex = index; }
	int GetMyMeshIndex() { return m_nMyMeshIndex; }
	void SetMyJointIndex(int index) { m_nMyJointIndex = index; }
	int GetMyJointIndex() { return m_nMyJointIndex; }
	void SetMyIndex(int index) { m_nMyIndex = index; }
	int GetMyIndex() { return m_nMyIndex; }

private:
	//test
	float m_fMin;
	float m_fMax;

	int m_nMyMeshIndex{ 0 };
	int m_nMyJointIndex{ 0 };
	int m_nMyIndex{ 0 };
public:
	CBoundingBox();
	~CBoundingBox();
};
