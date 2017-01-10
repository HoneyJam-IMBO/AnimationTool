#pragma once
#include "UseFBXMesh.h"
#include "StaticBuffer.h"
#include "FbxJointData.h"
//test
#include "AnimationInfo.h"

class CFBXAnimationMesh : public CUseFBXMesh{
public:
	//----------------------------dxobject-----------------------------
	bool Begin(UINT index = 0);
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();
	//----------------------------dxobject-----------------------------

	//---------------------------mesh----------------------------------
	virtual void Update(float fTimeElapsed);
	//virtual void RenderExcute(UINT nnInstance);
	//---------------------------mesh----------------------------------

	//CAnimationInfo* GetAnimationInfo() { return m_pAnimationInfo; }
private:
	//CAnimationInfo* m_pAnimationInfo{ nullptr };
	UINT m_MeshIndex{ 0 };

	//vertex buffer
	ID3D11Buffer* m_pd3dBoneIndexBuffer{ nullptr };
	ID3D11Buffer* m_pd3dWeightBuffer{ nullptr };
public:
	CFBXAnimationMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CUseFBXMesh(pd3dDevice, pd3dDeviceContext) {};
	virtual ~CFBXAnimationMesh() {};
};