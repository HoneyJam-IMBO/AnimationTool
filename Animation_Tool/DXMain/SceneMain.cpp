#include "stdafx.h"
#include "SceneMain.h"

void TW_CALL LoadFileCallback(void * clientData) {
	LoadFileStruct* pLFS = reinterpret_cast<LoadFileStruct*>(clientData);
//	string s = pLFS->Filename;
//	string path{ "../inputdata/" };

	pLFS->m_pScene->CreateControllObject(pLFS->Filename);
	const char* barName{ "LoadFile" };
	TWBARMGR->DeleteBar(barName);
	//string s = pLFS;
}
void TW_CALL LoadFileButtonCallback(void * clientData) {
	CSceneMain* pScene = reinterpret_cast<CSceneMain*>(clientData);
	pScene->CreateLoadFileUI();
}
void TW_CALL AddInfoCallback(void * clientData) {
	LoadFileStruct* pLFS = reinterpret_cast<LoadFileStruct*>(clientData);
//	string s = pLFS->Filename;
//	string path{ "../inputdata/" };

	pLFS->m_pScene->AddFBXAnimationInfo(pLFS->Filename);
	
	const char* barName{ "AddInfo" };
	TWBARMGR->DeleteBar(barName);
	//string s = pLFS;
}
void TW_CALL AddInfoButtonCallback(void * clientData) {
	CSceneMain* pScene = reinterpret_cast<CSceneMain*>(clientData);
	pScene->CreateAddInfoUI();
}
void TW_CALL ClearAllButtonCallback(void * clientData) {
	CSceneMain* pScene = reinterpret_cast<CSceneMain*>(clientData);
	pScene->ClearAllFBXObject();
}
void TW_CALL WriteNowButtonCallback(void * clientData) {
	CSceneMain* pScene = reinterpret_cast<CSceneMain*>(clientData);
	EXPORTER->Begin(L"../outputdata/text.txt");
	EXPORTER->ExportFbxObject(pScene->m_pFBXObject);
	EXPORTER->End();
}
bool CSceneMain::Begin() {

	//main button ui추가 
	//추가될 버튼
	//fbx mesh 변경 버튼 -> 기존 객체 mesh animation 정보 전부 삭제
	//fbx animation 정보 추가 -> 기존 객체 mesh의 animater에 정보 추가
	//
	const char* barName{ "MainControll" };
	TWBARMGR->AddBar(barName);
	//set param
	TWBARMGR->SetBarSize(barName, 500, 200);
	TWBARMGR->SetBarPosition(barName, 250, 550);
	TWBARMGR->SetBarColor(barName, 255, 0, 255);
	TWBARMGR->SetBarContained(barName, true);
	TWBARMGR->SetBarMovable(barName, false);
	TWBARMGR->SetBarResizable(barName, false);
	//set param
	TWBARMGR->AddButtonCB(barName, "load", "load file button", LoadFileButtonCallback, this);
	TWBARMGR->AddButtonCB(barName, "add", "add info button", AddInfoButtonCallback, this);
	TWBARMGR->AddButtonCB(barName, "clear", "clear all button", ClearAllButtonCallback, this);
	TWBARMGR->AddButtonCB(barName, "write", "write now button", WriteNowButtonCallback, this);

	//m_pPlayer = new CPlayer;
	//m_pPlayer->Begin();
	//----------------------------------camera-------------------------------------
	m_pCamera = m_pFrameWork->GetCamera();
	//----------------------------------camera-------------------------------------

	//--------------------------------------space-------------------------------------
	//space
	m_pSpaceContainer = new CSpaceContainer();
	m_pSpaceContainer->Begin(static_cast<int>(SPACE_SIZE), static_cast<int>(SPACE_LEVEL));
	////terrain
	//m_pTerrainContainer = new CTerrainContainer();
	//m_pTerrainContainer->Begin(L"../../Assets/HeightMap.raw", 257, 257, 0.5, m_pSpaceContainer);
	//--------------------------------------space-------------------------------------

	//--------------------------------객체 제작------------------------
	int nMaxObjects = 1;

	//m_pRotationTestObject = new CTestCube();
	//m_pRotationTestObject->Begin();
	////m_pRotationTestObject->SetTerrainContainer(m_pTerrainContainer);
	//m_pRotationTestObject->SetPosition(XMLoadFloat3(&XMFLOAT3(0, 0, 0)));
	//m_pSpaceContainer->AddObject(m_pRotationTestObject);

	//fbx mesh
	//CTestObject* pObject = nullptr;
	
	//CBunny* pBunny = new CBunny();
	//pBunny->Begin();
	////pBunny->SetTerrainContainer(m_pTerrainContainer);
	//pBunny->SetPosition(XMLoadFloat3(&XMFLOAT3(SPACE_SIZE / 2.f, 0, SPACE_SIZE / 2.f)));
	//m_pSpaceContainer->AddObject(pBunny);
	//fbx mesh

	//int space_size = static_cast<int>(SPACE_SIZE);
	//CPointLight* pPointLight = nullptr;
	//int nPointLight = 5;
	////int nPointLight = 0;
	//float fStartX = 0;
	//float fStartZ = 0;
	//for (int i = 0; i < nPointLight; ++i) {
	//		pPointLight = new CPointLight;
	//		pPointLight->Begin(POINT_LIGHT{ 8.5f, XMFLOAT3(rand() % 5, rand() % 5, rand() % 5) });
	//		//pPointLight->SetTerrainContainer(m_pTerrainContainer);
	//
	//		pPointLight->SetPosition(XMVectorSet(rand() % space_size, 100, rand() % space_size, 0.f));
	//		m_pSpaceContainer->AddObject(pPointLight);
	//}
	//
	//fStartX += 500;
	//fStartZ += 500;
	//CCapsuleLight* pCapsuleLight = nullptr;
	//int nCapsuleLight = 5;
	//
	//for (int i = 0; i < nCapsuleLight; ++i) {
	//	pCapsuleLight = new CCapsuleLight;
	//	pCapsuleLight->Begin(CAPSULE_LIGHT{ 10.f, XMFLOAT3(rand() % 5, rand() % 5, rand() % 5), 5.f});
	//	//pCapsuleLight->SetTerrainContainer(m_pTerrainContainer);
	//
	//	pCapsuleLight->SetPosition(XMVectorSet(rand() % space_size, rand() % 50 + 100, rand() % space_size, 0.f));
	//	m_pSpaceContainer->AddObject(pCapsuleLight);
	//}
	//
	//fStartX += 500;
	//fStartZ += 500;
	//
	//int nSpotLight = 5;
	//CSpotLight* pSpotLight = nullptr;
	//for (int i = 0; i < nSpotLight; ++i) {
	//		pSpotLight = new CSpotLight;
	//		pSpotLight->Begin(SPOT_LIGHT{ 10.f, XMFLOAT3(rand() % 100, rand() % 100, rand() % 100), 5.f, 5.f });
	//		//pSpotLight->SetTerrainContainer(m_pTerrainContainer);
	//
	//		pSpotLight->SetPosition(XMVectorSet(rand() % space_size, rand()%50+100, rand() % space_size, 0.f));
	//		m_pSpaceContainer->AddObject(pSpotLight);
	//}

	//--------------------------전역 객체 제작-------------------------
	//skybox
	m_pSkyBox = new CSkyBox();
	m_pSkyBox->Begin();
	m_pSkyBox->SetCamera(m_pCamera);
	//skybox

	//directional light
	m_pDirectionalLight = new CDirectionalLight;
	m_pDirectionalLight->Begin(DIRECTIONAL_AMBIENT_LIGHT{
		XMFLOAT4(1.0f, -1.0f, 1.0f, 0.0f),XMFLOAT4(0.0f, 0.1f, 0.1f, 0.0f) , XMFLOAT4(1.5f, 1.5f, 1.5f, 1.f),//dir
		XMFLOAT4(0.1f, 0.1f, 0.1f, 1.f), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.f), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.f), XMFLOAT4(5.1f, 5.1f, 5.1f, 1.f)//ambient
	});
	m_pDirectionalLight->SetPosition(XMVectorSet(SPACE_SIZE / 2.f, SPACE_SIZE, SPACE_SIZE / 2.f, 0.f));
	m_pDirectionalLight->Rotate(30.f, 0.f, 0.f);
	/*
	XMFLOAT4 m_DirToLight;
	XMFLOAT4 m_DirLightColor;
	XMFLOAT4 m_DirLightPower;

	XMFLOAT4 m_AmbientDown;
	XMFLOAT4 m_AmbientRange;
	XMFLOAT4 m_AmbientUp;
	XMFLOAT4 m_AmbientColor;
	*/
	//m_pDirectionalLight->SetRenderContainer(m_RenderContainerSeller);
	//directional light
	//--------------------------전역 객체 제작-------------------------
	//--------------------------------객체 제작-------------------------

	//create layer
	CreateRenderLayers();
	//create layer

	return true;
}

bool CSceneMain::End() {
	//m_pPlayer->End();
	//seller
	//m_RenderContainerSeller->End();

	if (m_pObjectLayer) {
		m_pObjectLayer->End();
		delete m_pObjectLayer;
	}
	//layers
	if (m_pLightLayer) {
		m_pLightLayer->End();
		delete m_pLightLayer;
	}
	
	//space
	if (m_pSpaceContainer) {
		m_pSpaceContainer->End();
		delete m_pSpaceContainer;
	}
	//terrain
	if (m_pTerrainContainer) {
		m_pTerrainContainer->End();
		delete m_pTerrainContainer;
	}

	//global object
	if (m_pSkyBox) {
		m_pSkyBox->End();
		delete m_pSkyBox;
	}
	if (m_pDirectionalLight) {
		m_pDirectionalLight->End();
		delete m_pDirectionalLight;
	}
	//카메라는 Framework에 존재하는 것이기에 End()작업을 진행하지 않는다.
	return true;
}

void CSceneMain::Animate(float fTimeElapsed) {
	//drag & drop 처리문
	const char* test{ nullptr };
	test = INPUTMGR->GetDropFileName();
	if (test) {
		if (nullptr == m_pFBXObject) {//만약 없다면 
			CreateControllObject("../../Assets/Model/fbx/1-2/Die_85.fbx");
		}
		else {
			AddFBXAnimationInfo("../../Assets/Model/fbx/1-2/Die_85.fbx");
			m_pFBXObject->PickingProc();
		}
	}
	//drag & drop 처리문
	//-----------------------------------space------------------------------
	m_pSpaceContainer->Animate(fTimeElapsed);
	//-----------------------------------space------------------------------

	//--------------------------전역 객체 animate / regist-------------------------
	m_pDirectionalLight->SetColor(xmf4DirectionalLightColor.x, xmf4DirectionalLightColor.y, xmf4DirectionalLightColor.z);
	
	//skybox camera 동기화
	m_pSkyBox->Animate(fTimeElapsed);
	//sky box 등록
	m_pSkyBox->RegistToContainer();
	//directional light 등록
	m_pDirectionalLight->RegistToContainer();
	//--------------------------전역 객체 animate / regist-------------------------
}

void CSceneMain::CreateRenderLayers() {
	
	//set texture
	//craete object layer
	m_pObjectLayer = new CObjectLayer(m_pd3dDevice, m_pd3dDeviceContext);
	m_pObjectLayer->Begin();
	//craete object layer

	//craete light layer
	m_pLightLayer = new CLightLayer(m_pd3dDevice, m_pd3dDeviceContext);
	m_pLightLayer->Begin();
	//craete light layer
}

void CSceneMain::ObjectRender() {

	m_pSpaceContainer->PrepareRender(m_pCamera);

	m_pObjectLayer->Render(m_pCamera);

	//debuge
	if(INPUTMGR->GetDebugMode())
		DEBUGER->DebugRender(m_pCamera);

}
void CSceneMain::LightRender() {
	//render light!
	m_pLightLayer->Render(m_pCamera);
}

void CSceneMain::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:

		POINT p = INPUTMGR->GetMousePoint();
		m_pPickingObject = PickObjectPointedByCursor(p.x, p.y);
		if (m_pPickingObject) {
			m_pPickingObject->PickingProc();
		}
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		

	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}
void CSceneMain::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	
	//RCSELLER->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
	case WM_KEYUP:
		
		switch (wParam)
		{
		case VK_F1:
		case VK_F2:
		case VK_F3:
			break;
		case VK_P:

		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CSceneMain::ProcessInput(float fTimeElapsed) {
	if (INPUTMGR->KeyDown(VK_P)) {
		INPUTMGR->SetDebugMode(static_cast<bool>((INPUTMGR->GetDebugMode() + 1) % 2));
	}
	m_pCamera->ProcessInput(fTimeElapsed);
}


CGameObject* CSceneMain::PickObjectPointedByCursor(int xClient, int yClient)
{
	if (!m_pCamera) return(NULL);

	//Get screen pos -> Camera pos
	XMFLOAT4X4 xmf4x4Projection;
	XMStoreFloat4x4(&xmf4x4Projection, m_pCamera->GetProjectionMtx());
	D3D11_VIEWPORT d3dViewport = m_pCamera->GetViewport();

	//음 이건 화면을 찍은 점의 카메라 좌표계의 녀석이고
	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / d3dViewport.Width) - 1) / xmf4x4Projection._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) / xmf4x4Projection._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition;
	xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmMtxViewInverse;
	xmMtxViewInverse = XMMatrixInverse(nullptr, m_pCamera->GetViewMtx());
	//picking pos에 camera inverse를 곱했으니 이건 picking pos의 world pos!
	xmvPickPosition = XMVector3TransformCoord(xmvPickPosition, xmMtxViewInverse);
	XMVECTOR xmvRayDir = xmvPickPosition - m_pCamera->GetPosition();
	
	CGameObject* pNearestObject = NULL;
	float fHitDistance = FLT_MAX;
	float fNearDistance = FLT_MAX;
	pNearestObject = m_pSpaceContainer->PickObject(m_pCamera->GetPosition(), XMVector3Normalize(xmvRayDir), fHitDistance);
	fNearDistance = fHitDistance;
	if (m_pDirectionalLight->CheckPickObject(m_pCamera->GetPosition(), XMVector3Normalize(xmvRayDir), fHitDistance)) {
		if (fNearDistance > fHitDistance) {
			pNearestObject = m_pDirectionalLight;
		}
	}
	return(pNearestObject);
}

void CSceneMain::CreateControllObject(string path){
//resource 제작	
	m_MeshCnt = RESOURCEMGR->CreateMultiMesh(path, "Test");
	//m_MeshCnt = RESOURCEMGR->CreateMultiMesh("../outputata/text.txt", "Test");
	RCSELLER->GetRenderContainer(object_id::OBJECT_FBX_ELF)->ClearMesh();
	char pName[20];
	for (int i = 0; i < m_MeshCnt; ++i) {
		sprintf(pName, "%s%d", "Test", i);
		RCSELLER->GetRenderContainer(object_id::OBJECT_FBX_ELF)->AddMesh(RESOURCEMGR->GetMesh(pName));
	}
	RCSELLER->GetRenderContainer(object_id::OBJECT_FBX_ELF)->SetAnimater(RESOURCEMGR->GetAnimater("Test"));
//resource 제작	


//객체 제작
	m_pFBXObject = new CTestObject();
	m_pFBXObject->Begin();
	//pObject->SetTerrainContainer(m_pTerrainContainer);
	m_pFBXObject->SetPosition(XMLoadFloat3(&XMFLOAT3(SPACE_SIZE / 2.f, 0, SPACE_SIZE / 2.f)));
	m_pSpaceContainer->AddObject(m_pFBXObject);
//객체 제작
	//ui pop up!
	m_pFBXObject->PickingProc();
}

void CSceneMain::CreateLoadFileUI(){

	const char* barName{ "LoadFile" };
	TWBARMGR->AddBar(barName);

	vector<wstring> vFile;
	DIRECTORYFINDER->GetFiles(vFile, L"../inputdata", true, true, L".fbx");
	DIRECTORYFINDER->GetFiles(vFile, L"../inputdata", true, true, L".FBX");
	DIRECTORYFINDER->GetFiles(vFile, L"../inputdata", true, true, L".gjm");
	DIRECTORYFINDER->GetFiles(vFile, L"../inputdata", true, true, L".GJM");
	DIRECTORYFINDER->GetFiles(vFile, L"../outputdata", true, true, L".gjm");
	DIRECTORYFINDER->GetFiles(vFile, L"../outputdata", true, true, L".GJM");
	//test
	DIRECTORYFINDER->GetFiles(vFile, L"../outputdata", true, true, L".txt");

	//const char* groupName = "File";
	char menuName[64];
	int cnt{ 0 };
	m_LoadFileStruct.resize(vFile.size());
	for (auto data : vFile) {
		//file directory store;
		data = DIRECTORYFINDER->ReplaceString(data, L"\\", L"/");
		string filsDirectory{ "" };
		filsDirectory.assign(data.cbegin(), data.cend());
		m_LoadFileStruct[cnt] = LoadFileStruct{ this, filsDirectory };

		//menu name = file name
		string menuNameString{ "" };
		menuNameString.assign(data.cbegin(), data.cend());
		sprintf(menuName, "%s", menuNameString.c_str());

		//group name = directory name
		data = DIRECTORYFINDER->ReplaceString(data, L"/", L"\\");
		LPWSTR str = (LPWSTR)data.c_str();
		PathRemoveFileSpec(str);
	
		wstring wGroupName{ str };
		string groupName;
		groupName.assign(wGroupName.cbegin(), wGroupName.cend());
		TWBARMGR->AddButtonCB(barName, groupName.c_str() , menuName, LoadFileCallback, &m_LoadFileStruct[cnt]);
		cnt++;
	}
	//make texture/ set texture
	//pObj->GetRenderContainer()->GetMesh(pObj->GetSelectMeshIndex());
}

void CSceneMain::AddFBXAnimationInfo(string path){
	if (nullptr == m_pFBXObject) {
		//wstring ws{ L"" };
		//ws.assign(path.cbegin(), path.cend());
		CreateControllObject(path);
		return;
	}

	//fbx animation info 추가!
	string name{ "Test" };
	FBXIMPORTER->Begin(path);
	if (FBXIMPORTER->GetHasAnimation()) {
		CAnimationInfo* pAnimationInfo = CAnimationInfo::CreateAnimationInfoFromFBXFile(m_pd3dDevice, m_pd3dDeviceContext, RESOURCEMGR->GetAnimater(name));
	}
	FBXIMPORTER->End();
	//fbx animation info 추가!

	m_pFBXObject->PickingProc();
}

void CSceneMain::CreateAddInfoUI(){
	const char* barName{ "AddInfo" };
	TWBARMGR->AddBar(barName);

	m_LoadFileStruct.clear();

	vector<wstring> vFile;
	DIRECTORYFINDER->GetFiles(vFile, L"../inputdata", true, true, L".fbx");
	DIRECTORYFINDER->GetFiles(vFile, L"../inputdata", true, true, L".FBX");

	const char* groupName = "File";
	char menuName[64];
	int cnt{ 0 };
	m_LoadFileStruct.resize(vFile.size());
	for (auto data : vFile) {
		//file directory store;
		data = DIRECTORYFINDER->ReplaceString(data, L"\\", L"/");
		string filsDirectory{ "" };
		filsDirectory.assign(data.cbegin(), data.cend());
		m_LoadFileStruct[cnt] = LoadFileStruct{ this, filsDirectory };

		//menu name = file name
		string menuNameString{ "" };
		menuNameString.assign(data.cbegin(), data.cend());
		sprintf(menuName, "%s", menuNameString.c_str());

		//group name = directory name
		data = DIRECTORYFINDER->ReplaceString(data, L"/", L"\\");
		LPWSTR str = (LPWSTR)data.c_str();
		PathRemoveFileSpec(str);

		wstring wGroupName{ str };
		string groupName;
		groupName.assign(wGroupName.cbegin(), wGroupName.cend());
		TWBARMGR->AddButtonCB(barName, groupName.c_str(), menuName, AddInfoCallback, &m_LoadFileStruct[cnt]);
		cnt++;
	}
}

void CSceneMain::ClearAllFBXObject(){
	if (m_pFBXObject) {
		m_pSpaceContainer->RevomeObject(m_pFBXObject);
		m_pFBXObject->GetRenderContainer()->ClearMesh();
		m_pFBXObject->GetRenderContainer()->ClearAnimater();
		m_pFBXObject->End();
		delete m_pFBXObject;
		m_pFBXObject = nullptr;

		//pObject->SetTerrainContainer(m_pTerrainContainer);
		char MeshName[64];
		const char* name = "Test";
		for (int i = 0; i < m_MeshCnt; ++i) {
			sprintf(MeshName, "%s%d", name, i);
			RESOURCEMGR->ReleaseMesh(MeshName);
		}
		RESOURCEMGR->ReleaseAnimater(name);
		m_MeshCnt = 0;

	}
	ClearAllFBXUI();
}

void CSceneMain::ClearAllFBXUI(){
	TWBARMGR->DeleteBar("AnimaterInfo");
	TWBARMGR->DeleteBar("AddInfo");
	TWBARMGR->DeleteBar("LoadFile");

	TWBARMGR->DeleteBar("AnimationInfo");
	TWBARMGR->DeleteBar("ActiveJoint");

	TWBARMGR->DeleteBar("MeshInfo");
	TWBARMGR->DeleteBar("PickingBar");
	TWBARMGR->DeleteBar("LoadTextureFile");
}

CSceneMain::CSceneMain(CDirectXFramework* pFrameWork) : CScene("Main") {
	m_pFrameWork = pFrameWork;
	m_pd3dDevice = m_pFrameWork->GetDevice();
	m_pd3dDeviceContext = m_pFrameWork->GetDeviceContext();
	m_hWnd = m_pFrameWork->GethWnd();
}
CSceneMain::~CSceneMain() {

}


