#include "stdafx.h"
#include "SceneMain.h"

static XMFLOAT4 xmf4TestRotation;

bool CSceneMain::Begin() {

	//m_pPlayer = new CPlayer;
	//m_pPlayer->Begin();
	//----------------------------------camera-------------------------------------
	m_pCamera = m_pFrameWork->GetCamera();
	//----------------------------------camera-------------------------------------

	//--------------------------------------space-------------------------------------
	//space
	m_pSpaceContainer = new CSpaceContainer();
	m_pSpaceContainer->Begin(static_cast<int>(SPACE_SIZE), static_cast<int>(SPACE_LEVEL));
	//terrain
	m_pTerrainContainer = new CTerrainContainer();
	m_pTerrainContainer->Begin(L"../../Assets/HeightMap.raw", 257, 257, 0.5, m_pSpaceContainer);
	//--------------------------------------space-------------------------------------

	//--------------------------------객체 제작------------------------
	int nMaxObjects = 1;
	////fbx mesh
	//CTestObject* pObject = nullptr;
	//pObject = new CTestObject();
	//pObject->Begin();
	//pObject->SetTerrainContainer(m_pTerrainContainer);
	//pObject->SetPosition(XMLoadFloat3(&XMFLOAT3(SPACE_SIZE / 2.f, 0, SPACE_SIZE / 2.f)));
	//
	//m_pSpaceContainer->AddObject(pObject);
	m_pRotationTestObject = new CTestCube();
	m_pRotationTestObject->Begin();
	m_pRotationTestObject->SetTerrainContainer(m_pTerrainContainer);
	m_pRotationTestObject->SetPosition(XMLoadFloat3(&XMFLOAT3(0, 0, 0)));
	m_pSpaceContainer->AddObject(m_pRotationTestObject);

	CBunny* pBunny = new CBunny();
	pBunny->Begin();
	pBunny->SetTerrainContainer(m_pTerrainContainer);
	pBunny->SetPosition(XMLoadFloat3(&XMFLOAT3(SPACE_SIZE / 2.f, 0, SPACE_SIZE / 2.f)));
	m_pSpaceContainer->AddObject(pBunny);
	//fbx mesh
	
	int space_size = static_cast<int>(SPACE_SIZE);
	CPointLight* pPointLight = nullptr;
	int nPointLight = 5;
	//int nPointLight = 0;
	float fStartX = 0;
	float fStartZ = 0;
	for (int i = 0; i < nPointLight; ++i) {
			pPointLight = new CPointLight;
			pPointLight->Begin(POINT_LIGHT{ 8.5f, XMFLOAT3(rand() % 5, rand() % 5, rand() % 5) });
			pPointLight->SetTerrainContainer(m_pTerrainContainer);

			pPointLight->SetPosition(XMVectorSet(rand() % space_size, 100, rand() % space_size, 0.f));
			m_pSpaceContainer->AddObject(pPointLight);
	}
	
	fStartX += 500;
	fStartZ += 500;
	CCapsuleLight* pCapsuleLight = nullptr;
	int nCapsuleLight = 5;
	
	for (int i = 0; i < nCapsuleLight; ++i) {
		pCapsuleLight = new CCapsuleLight;
		pCapsuleLight->Begin(CAPSULE_LIGHT{ 10.f, XMFLOAT3(rand() % 5, rand() % 5, rand() % 5), 5.f});
		pCapsuleLight->SetTerrainContainer(m_pTerrainContainer);

		pCapsuleLight->SetPosition(XMVectorSet(rand() % space_size, rand() % 50 + 100, rand() % space_size, 0.f));
		m_pSpaceContainer->AddObject(pCapsuleLight);
	}
	
	fStartX += 500;
	fStartZ += 500;
	
	int nSpotLight = 5;
	CSpotLight* pSpotLight = nullptr;
	for (int i = 0; i < nSpotLight; ++i) {
			pSpotLight = new CSpotLight;
			pSpotLight->Begin(SPOT_LIGHT{ 10.f, XMFLOAT3(rand() % 100, rand() % 100, rand() % 100), 5.f, 5.f });
			pSpotLight->SetTerrainContainer(m_pTerrainContainer);

			pSpotLight->SetPosition(XMVectorSet(rand() % space_size, rand()%50+100, rand() % space_size, 0.f));
			m_pSpaceContainer->AddObject(pSpotLight);
	}

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
	m_pDirectionalLight->SetPosition(XMVectorSet(SPACE_SIZE / 2.f, SPACE_SIZE / 2.f, SPACE_SIZE / 2.f, 0.f));
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

	TWBARMGR->AddColorBar("TweakBarTest", "rotation", "testCo", &xmf4DirectionalLightColor);
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
	//DEBUGER->AddText(100, 100, 100, YT_Color(255, 0, 0), L"TEST");

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
			TWBARMGR->DeleteBar("PickingBar");
			TWBARMGR->AddRotationBar("PickingBar", "rotation", "testRo", m_pPickingObject);
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
	
	RCSELLER->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
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

CSceneMain::CSceneMain(CDirectXFramework* pFrameWork) : CScene("Main") {
	m_pFrameWork = pFrameWork;
	m_pd3dDevice = m_pFrameWork->GetDevice();
	m_pd3dDeviceContext = m_pFrameWork->GetDeviceContext();
	m_hWnd = m_pFrameWork->GethWnd();
}
CSceneMain::~CSceneMain() {

}


