#include "stdafx.h"
#include "RotateComponent.h"


bool CRotateComponent::Begin() {
	m_fRotate = static_cast<float>(rand() % 180);

	return TRUE;
};

bool CRotateComponent::End() {

	//자신의 소유자
	m_pOwner = NULL;

	return CComponent::End();
};
XMMATRIX CRotateComponent::Animate(XMMATRIX xmmtxWorld, float fTimeElapsed) {

	float fRadian = XMConvertToRadians( m_fRotate * fTimeElapsed);

	return XMMatrixMultiply(XMMatrixRotationY(fRadian), xmmtxWorld);
}

CRotateComponent::CRotateComponent() : CAnimateComponent() {
	//컴포넌트 id
	m_ID = ROTATE;

	//family랑 owner는 AnimateComponent가 초기화함
}
CRotateComponent::~CRotateComponent() {

}