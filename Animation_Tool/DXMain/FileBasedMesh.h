#pragma once
#include "Mesh.h"

#define START_MESH_INDEX 0

class CFileBasedMesh : public CMesh {

public:
	//----------------------------dxobject-----------------------------
	bool Begin(UINT index);
	virtual bool End();
	//----------------------------dxobject-----------------------------

	//---------------------------mesh----------------------------------
	//begin func
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
//	void ProcessGetVertexBuffer(FbxNode* pNode);
	//begin func
	//---------------------------mesh----------------------------------
	UINT GetMeshIndex() { return m_MeshIndex; }

	XMFLOAT3* GetNormals() { return m_pNormals; }
	XMFLOAT2* GetUVs() { return m_pUVs; }
	//animation data
	XMFLOAT4* GetJointIndices() { return m_pxmf4BoneIndex; }
	XMFLOAT3* GetWeights() { return m_pxmf3Weight; }

protected:
	UINT m_MeshIndex{ 0 };

	//vertex data
	XMFLOAT3* m_pNormals{ nullptr };
	XMFLOAT2* m_pUVs{ nullptr };
	//animation
	XMFLOAT4* m_pxmf4BoneIndex;
	XMFLOAT3* m_pxmf3Weight;
	//vertex data
	//----------------------vertex buffers---------------------------
	ID3D11Buffer* m_pd3dPositionBuffer{ nullptr };
	ID3D11Buffer* m_pd3dNormalBuffer{ nullptr };
	ID3D11Buffer* m_pd3dTBuffer{ nullptr };
	ID3D11Buffer* m_pd3dBBuffer{ nullptr };
	ID3D11Buffer* m_pd3dUVBuffer{ nullptr };
	//animation
	ID3D11Buffer* m_pd3dBoneIndexBuffer{ nullptr };
	ID3D11Buffer* m_pd3dWeightBuffer{ nullptr };
	//----------------------vertex buffers---------------------------

private:
	
	//helper func
	//void GetPositionData(FbxMesh* pMesh);
	//void GetNormalData(FbxMesh* pMesh);
	//void GetUVData(FbxMesh* pMesh);
public:
	CFileBasedMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CFileBasedMesh();
};