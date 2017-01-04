#pragma once
//#include "Camera.h"
#include "SingleTon.h"
/*
rotaion 예제 TwAddVarRW(bar, "Rotation", TW_TYPE_QUAT4F, &g_SpongeRotation, "opened=true axisz=-z group=Sponge");
TwAddVarRW(bar, "Camera distance", TW_TYPE_FLOAT, &g_CamDistance, "min=0 max=4 step=0.01 keyincr=PGUP keydecr=PGDOWN");0~4 사이 0.01크기로 증가하는 bar 
TwAddVarRW(bar, "Background", TW_TYPE_COLOR4F, &g_BackgroundColor, "colormode=hls");//color정해주기 이렇게 하면 해당 color데이터를 가지고 색을 정하는 메뉴가 자동으로 나타남
TwAddVarRW(bar, "Animation", TW_TYPE_BOOLCPP, &g_Animate, "group=Sponge key=a"); botton o/x bool type
*/
class CTwBarManager : public CSingleTonBase<CTwBarManager> {
public:
	bool Begin(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, const char* def);
	bool End();

	bool OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void Render();

	void AddBar(const char* barName);
	void SetParam(const char* barName, const char* paramName, TwParamValueType type, UINT inValueCnt, const void* inValue);
	//정해져 있지 않은경우 이걸로 내가 정의한 struct
	// get함수에서 Tw타입에 맞게 변경->ui가 처리->Set함수에서 내 struct로 변경
	//client data는 무엇이든 될 수있고, Callback함수의 value는 ui가 바꾸는 값이다.
	void AddVarCB(const char* barName, const char* menuName, TwType type, TwSetVarCallback setCallback, TwGetVarCallback getCallback, void* clientData, const char* def);
	//자료형이 정해져 있는경우 이걸로
	void AddVarRW(const char* barName, const char* menuName, TwType type, void* var, const char* def);

/*
	만들어 놓은 틀인데 이건 Tw자료형과 같은 녀석이어야함 쿼터니언이나. color등
*/
	//light dir float3에 사용하기 좋은 화살표 bar
	void AddDirBar(const char* barName, const char* groupName, const char* menuName, void* var);
	//rotation object float4에 사용하기 좋은 동그라미 bar
	void AddRotationBar(const char* barName, const char* groupName, const char* menuName, void* var);
	//color에 사용하기 적당한 색상 변경 bar 색상 변경 menu가 자동으로 생김
	void AddColorBar(const char* barName, const char* groupName, const char* menuName, void* var);
	//bool botton o/x에 사용하기 적당한 버튼 ui
	void AddBoolBar(const char* barName, const char* groupName, const char* menuName, void* var);

	//light dir float3에 사용하기 좋은 화살표 bar
	void AddDirBar(const char* barName, const char* groupName, const char* menuName, CGameObject* pObj);
	//rotation object float4에 사용하기 좋은 동그라미 bar
	void AddRotationBar(const char* barName, const char* groupName, const char* menuName, CGameObject* pObj);


	void DeleteBar(const char* barName);
private:
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pd3dDeviceContext;

	map<string, TwBar*> m_mTwBar;
	using pairTwBar = pair<string, TwBar*>;

public:
	CTwBarManager() : CSingleTonBase<CTwBarManager>("twbarmanagersingleton") {}

};

//callback
void TW_CALL SetQuaternionToTwBar(const void *value, void * clientData);
void TW_CALL GetQuaternionToTwBar(void *value, void * clientData);
