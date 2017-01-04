#include "stdafx.h"
#include "TwBarManager.h"


bool CTwBarManager::Begin(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pd3dDeviceContext, const char* def) {
	m_pd3dDevice = pd3dDevice;
	m_pd3dDeviceContext = pd3dDeviceContext;
	
	//ui
	// Initialize AntTweakBar
	if (!TwInit(TW_DIRECT3D11, m_pd3dDevice)) {
		DEBUGER->DebugMessageBox("AntTweakBar initialization", "failed");
		return false;
	}

	TwDefine(def); // Message added to the help bar.
	return true;
}

bool CTwBarManager::End() {
	//ui
	TwTerminate();

	m_mTwBar.clear();
	return true;
}


bool CTwBarManager::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	//ui
	// Send event message to AntTweakBar
	if (TwEventWin(hWnd, nMessageID, wParam, lParam))
		return true; // Event has been handled by AntTweakBar

	return false;
}

void CTwBarManager::Render(){
	//ui
	// Draw tweak bars
	TwDraw();
}

void CTwBarManager::AddBar(const char* barName){
	if (m_mTwBar.end() != m_mTwBar.find(barName)) return;//있으면 return

	TwBar *bar = TwNewBar(barName);
	m_mTwBar.insert(pairTwBar(barName, bar));
}

void CTwBarManager::SetParam(const char * barName, const char * paramName, TwParamValueType type, UINT inValueCnt, const void * inValue){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);

	TwSetParam(m_mTwBar[barName], barName, paramName, type, inValueCnt, inValue);
}

void CTwBarManager::AddVarCB(const char * barName, const char * manuName, TwType type, TwSetVarCallback setCallback, TwGetVarCallback getCallback, void * clientData, const char * def){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);

	TwAddVarCB(m_mTwBar[barName], manuName, type, setCallback, getCallback, clientData, def);
}

void CTwBarManager::AddVarRW(const char * barName, const char * manuName, TwType type, void * var, const char * def){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);

	TwAddVarRW(m_mTwBar[barName], manuName, type, var, def);
}

void CTwBarManager::AddDirBar(const char * barName, const char* groupName, const char * menuName, void * var){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "opened=true axisz=-z showval=false group=%s", groupName);

	TwAddVarRW(m_mTwBar[barName], menuName, TW_TYPE_DIR3F, var, buff);
}

void CTwBarManager::AddRotationBar(const char * barName, const char* groupName, const char * menuName, void * var){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "opened=true axisz=-z group=%s", groupName);

	TwAddVarRW(m_mTwBar[barName], menuName, TW_TYPE_QUAT4F, var, buff);
}

void CTwBarManager::AddColorBar(const char * barName, const char* groupName, const char * menuName, void * var){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "colormode=hls group=%s", groupName);

	TwAddVarRW(m_mTwBar[barName], menuName, TW_TYPE_COLOR4F, var, buff);
}

void CTwBarManager::AddBoolBar(const char * barName, const char* groupName, const char * menuName, void * var){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "group=%s", groupName);

	TwAddVarRW(m_mTwBar[barName], menuName, TW_TYPE_BOOLCPP, var, buff);
}

void CTwBarManager::AddDirBar(const char * barName, const char * groupName, const char * menuName, CGameObject* pObj){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "opened=true axisz=-z showval=false group=%s", groupName);

	AddVarCB(barName, menuName, TW_TYPE_DIR3F, SetQuaternionToTwBar, GetQuaternionToTwBar, pObj, buff);
}

void CTwBarManager::AddRotationBar(const char * barName, const char * groupName, const char * menuName, CGameObject* pObj){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) AddBar(barName);
	char buff[256];
	sprintf(buff, "opened=true axisz=-z group=%s", groupName);

	AddVarCB(barName, menuName, TW_TYPE_QUAT4F, SetQuaternionToTwBar, GetQuaternionToTwBar, pObj, buff);
}


void CTwBarManager::DeleteBar(const char * barName){
	if (m_mTwBar.end() == m_mTwBar.find(barName)) return;//없으면 return;

	TwDeleteBar(m_mTwBar[barName]);
	m_mTwBar.erase(barName);
}

void TW_CALL SetQuaternionToTwBar(const void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	pObj->SetRotationQuaternion(XMLoadFloat4(static_cast<const XMFLOAT4 *>(value)));

	pObj->SetWorldMtx(XMMatrixAffineTransformation(pObj->GetScale(), XMQuaternionIdentity(), pObj->GetRotationQuaternion(), pObj->GetPosition()));
}

void TW_CALL GetQuaternionToTwBar(void * value, void * clientData) {
	if (nullptr == clientData) return;
	CGameObject* pObj = reinterpret_cast<CGameObject*>(clientData);
	XMFLOAT4 xmf4;
	XMStoreFloat4(&xmf4, pObj->GetRotationQuaternion());
	
	*static_cast<XMFLOAT4 *>(value) = xmf4;
}
