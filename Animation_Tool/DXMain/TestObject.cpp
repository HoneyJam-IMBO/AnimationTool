#include "stdafx.h"
#include "TestObject.h"

bool CTestObject::Begin() {
	//object_id set
	m_objectID = object_id::OBJECT_FBX_ELF;
	//m_objectID = object_id::OBJECT_TEST;

	Rotate(-90.f, 0.f, 0.f);
	//���ο� ��ü�� ���ܳ��� Begin���� Component���� set���ش�. 
	//CComponent* pComponent = new CRotateComponent;
	//pComponent->Begin();
	//return SetComponent(pComponent);

	CGameObject::Begin();
	return true;
}
bool CTestObject::End() {

	return CGameObject::End();
}
void CTestObject::SetPosition(XMVECTOR pos) {
	XMFLOAT3 xmfPos;
	XMStoreFloat3(&xmfPos, pos);

	m_xmf4x4World._41 = xmfPos.x;
	m_xmf4x4World._42 = xmfPos.y;
	m_xmf4x4World._43 = xmfPos.z;

	if( m_pTerrainContainer ) m_xmf4x4World._42 = GetTerrainHeight();
	
}

void CTestObject::PickingProc() {
	//1. ��� mesh�� ��� 
	CGameObject::PickingProc();

	for (int i = 0; i < m_pRenderContainer->GetvMesh().size(); ++i) {
		char groupName[64];
		sprintf(groupName, "%s%d", "Mesh", i);
		CFBXAnimationMesh* pMesh = dynamic_cast<CFBXAnimationMesh* >(m_pRenderContainer->GetMesh(i).get());
		for (int j = 0; j < pMesh->GetvJoint().size(); ++j) {
			//add obb position bar
			char positionMenuName[64];
			sprintf(positionMenuName, "%s%s", pMesh->GetvJoint()[j].GetJointName().c_str(), " Position");
			TWBARMGR->AddPositionBar("PickingBar", pMesh->GetvJoint()[j].GetJointName().c_str(), positionMenuName, &pMesh->GetOBBObject(j),
				-100.f, 100.f, 0.1f);
			//add obb scale bar
			char scaleMenuName[64];
			sprintf(scaleMenuName, "%s%s", pMesh->GetvJoint()[j].GetJointName().c_str(), " Scale");
			TWBARMGR->AddScaleBar("PickingBar", pMesh->GetvJoint()[j].GetJointName().c_str(), scaleMenuName, &pMesh->GetOBBObject(j),
				0.1f, 100.f, 0.1f);
		}
	}
}
CTestObject::CTestObject() : CGameObject("testobject", tag::TAG_DYNAMIC_OBJECT) {

}
CTestObject::~CTestObject() {

}