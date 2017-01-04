#pragma once

#include "Component.h"
#include "Camera.h"
#include "Object.h"

class CTerrainContainer;

//rendercontainer 더미 클래스
class CRenderContainer;
//class CRenderContainerSeller;

typedef std::pair<component_id, CComponent*> pairComponent;
typedef std::map<component_id, CComponent*> mapComponent;


struct VS_VB_INSTANCE {
	XMMATRIX m_xmmtxWorld;
};


class CGameObject : public CObject {
public:
	//----------------------------object-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------object-----------------------------

	virtual void SetPosition(XMVECTOR pos);

	XMVECTOR GetRight();
	XMVECTOR GetUp();
	XMVECTOR GetLook();
	void SetRight(XMVECTOR xmvRight);
	void SetUp(XMVECTOR xmvUp);
	void SetLook(XMVECTOR xmvLook);

	XMVECTOR GetPosition();
	object_id GetObjectID() { return m_objectID; }

	virtual XMMATRIX GetWorldMtx();
	void SetWorldMtx(XMMATRIX mtxWorld);
	void SetRotation(XMMATRIX mtxRotation);
	//test animate func
	virtual void Move(XMVECTOR xmvDir, float fDistance);
	virtual void Rotate(XMMATRIX xmMtx);
	virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	//----------------------------component------------------------
	//자신의 component 실행 + 
	virtual void Animate(float fTimeElapsed);

	//Component Set
	bool SetComponent(CComponent* pComponenet);

	//Get
	CComponent* GetComponenet(const component_id& componenetID);
	const CComponent* GetComponenet(const component_id& componenetID)const;

	//Clear
	void ClearComponents();
	//----------------------------component------------------------

	//instance buffer controll
	virtual void SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);
	
	//render container controll
	virtual void RegistToContainer();
	CRenderContainer* GetRenderContainer() { return m_pRenderContainer; }
	//virtual void RegistToLayer(mapLayer & mLayer);
//	void SetRenderContainer(CRenderContainerSeller* pSeller);

	//space
	void SetSpaceIndex(int index) { m_spaceIndex = index; }
	int GetSpaceIndex() { return m_spaceIndex;	}

	//terrain 
	float GetTerrainHeight();
	void SetTerrainContainer(CTerrainContainer* pTerrainContainer) { m_pTerrainContainer = pTerrainContainer; };

	//aabb
	virtual bool IsVisible(shared_ptr<CCamera> pCamera);//계층구조의 녀석들은 다시 만들어줄 필요가 있음
	BoundingBox GetBoundingBox() { return m_BoundingBox; }

	XMVECTOR GetRotationQuaternion() { return XMLoadFloat4(&m_xmf4RotationQuaternion); }
	void SetRotationQuaternion(XMVECTOR xmv) { XMStoreFloat4(&m_xmf4RotationQuaternion, xmv); }

	XMVECTOR GetScale() { return XMLoadFloat4(&m_xmf4Scale); }
	void SetScale(XMVECTOR xmv) { XMStoreFloat4(&m_xmf4Scale, xmv); }

	//ray picking 
	bool CheckPickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distance);
protected:
	
	BoundingBox m_OriBoundingBox;
	BoundingBox m_BoundingBox;

	bool m_bIsVisible{ true }; 
	bool m_bActive{ true };

	//terrain container
	CTerrainContainer* m_pTerrainContainer{ nullptr };

	//world
	XMFLOAT4X4 m_xmf4x4World;
	XMFLOAT4 m_xmf4RotationQuaternion;
	XMFLOAT4 m_xmf4Scale{ 1.f, 1.f, 1.f, 1.f };
	mapComponent m_mapComponents;

	CRenderContainer* m_pRenderContainer{ nullptr };
	//object_id
	object_id m_objectID{ object_id::OBJECT_END };

	//space
	int m_spaceIndex{ 0 };
public:
	CGameObject(string name, tag t = tag::TAG_DEFAULT);
	virtual ~CGameObject();
};