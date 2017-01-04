#pragma once

#include "SingleTon.h"
#include "DirectXFramework.h"

#include "DirectionalLightMesh.h"
#include "PointLightMesh.h"
#include "SpotLightMesh.h"
#include "CapsuleLightMesh.h"
#include "TestMesh.h"
#include "PlaneMesh.h"
#include "TestDeferredMesh.h"
#include "SkyBoxMesh.h"//skybox
#include "SpaceMesh.h"//space
#include "TerrainMesh.h"//terrain
#include "AABBMesh.h"//aabb
#include "DebugTextureMesh.h"//debug texture mesh
#include "CoordinateSysMesh.h"//CoordinateSystem Mesh
#include "UseFBXMesh.h"//fbx mesh
#include "FBXAnimationMesh.h"//animation mesh
#include "MultiMesh.h"//multiMesh

#include "TestObject.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CapsuleLight.h"
#include "SkyBox.h"//skybox
#include "Space.h"//space
#include "Terrain.h"//terrain
#include "DebugTexture.h"//debug texture

//buffer
#include "InstanceBuffer.h"
#include "ConstantBuffer.h"
//global buffer
#include "GlobalBuffer.h"
#include "GlobalObject.h"
#include "GlobalTerrain.h"


#include "Material.h"

#include "Texture.h"
#include "Sampler.h"
#include "RenderShader.h"

class CResourceManager : public CSingleTonBase<CResourceManager> {

public:
	bool Begin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool End();

	//begin func
	void CreateTexture();
	void CreateRenderShader();
	void CreateMesh();
	void CreateMultiMesh(string path);
	void CreateBuffer();
	void CreateGlobalBuffer();
	void CreateMaterial();

	//end func
	void ReleaseTexture();
	void ReleaseRenderShader();
	void ReleaseMesh();
	void ReleaseBuffer();
	void ReleaseGlobalBuffer();
	void ReleaseMaterial();

	shared_ptr<CTexture> GetTexture(string name) { return m_mTexture[name]; }
	shared_ptr<CRenderShader> GetRenderShader(string name) { return m_mRenderShader[name]; }
	shared_ptr<CMesh> GetMesh(string name) { return m_mMesh[name]; }
	shared_ptr<CBuffer> GetBuffer(string name) { return m_mBuffer[name]; }
	shared_ptr<CGlobalBuffer> GetGlobalBuffer(string name) { return m_mGlobalBuffer[name]; }
	shared_ptr<CMaterial> GetMaterial(string name) { return m_mMaterial[name]; }

private:
	ID3D11Device* m_pd3dDevice{ nullptr };
	ID3D11DeviceContext* m_pd3dDeviceContext{ nullptr };

	map<string, shared_ptr<CTexture>> m_mTexture;
	using pairTexture = pair<string, shared_ptr<CTexture>>;
	map<string, shared_ptr<CRenderShader>> m_mRenderShader;
	using pairShader = pair<string, shared_ptr<CRenderShader>>;
	map<string, shared_ptr<CMesh>> m_mMesh;
	using pairMesh = pair<string, shared_ptr<CMesh>>;
	map<string, shared_ptr<CBuffer>> m_mBuffer;
	using pairBuffer = pair<string, shared_ptr<CBuffer>>;
	map<string, shared_ptr<CGlobalBuffer>> m_mGlobalBuffer;
	using pairGlobalBuffer = pair<string, shared_ptr<CGlobalBuffer>>;
	map<string, shared_ptr<CMaterial>> m_mMaterial;
	using pairMaterial = pair<string, shared_ptr<CMaterial>>;
	map<string, shared_ptr<CSampler>> m_mSampler;
	using pairSampler = pair<string, shared_ptr<CSampler>>;

public:
	CResourceManager();
	virtual ~CResourceManager();


};
