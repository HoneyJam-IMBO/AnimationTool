#include "stdafx.h"
#include "FileBasedMesh.h"

static bool gVerbose = true;

bool CFileBasedMesh::Begin(UINT index) {
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_MeshIndex = index;

	CreateVertexBuffer();
	CreateIndexBuffer();
	//create vertex buffer
	return true;
}

bool CFileBasedMesh::End() {
	if(m_pNormals) delete[] m_pNormals;
	if(m_pUVs) delete[] m_pUVs;
	//animation
	if(m_pxmf4BoneIndex) delete[] m_pxmf4BoneIndex;
	if(m_pxmf3Weight) delete[] m_pxmf3Weight;

	return CMesh::End();
}

bool CFileBasedMesh::CreateVertexBuffer() {
//	m_nMeshCnt = FBXIMPORTER->GetMeshCnt();

	//1. 전체 정점을 구한다.
	m_nVertices = FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexCnt();
	
	m_pVertices = new XMFLOAT3[m_nVertices];
	m_pNormals = new XMFLOAT3[m_nVertices];
	m_pUVs = new XMFLOAT2[m_nVertices];
	//저장할 정점 인덱스
	int nVertex{ 0 };
	
	//mesh의 모든 정점에 대해서
	for (UINT j = 0; j < FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexCnt(); ++j) {
		//정점 정보 얻어온다.
		m_pVertices[nVertex] = FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetPosition();
		m_pNormals[nVertex] = FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetNormal();
		m_pUVs[nVertex++] = FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetUV();
	}
	//create space mesh aabb
	BoundingBox boundingBox;
	BoundingBox::CreateFromPoints(boundingBox, (size_t)m_nVertices, m_pVertices, (size_t)sizeof(XMFLOAT3));
	m_AABB.SetBoundingBoxInfo(boundingBox);

	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT2), m_nVertices, m_pUVs, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	//-----------------------------------버퍼 조립-----------------------------------
	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dUVBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	//-----------------------------------버퍼 조립-----------------------------------

	m_pxmf4BoneIndex = new XMFLOAT4[m_nVertices];
	m_pxmf3Weight = new XMFLOAT3[m_nVertices];

	//모든 mesh에 대해서
	int nVertexIndex{ 0 };
	for (UINT j = 0; j < FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexCnt(); ++j) {

		m_pxmf4BoneIndex[nVertexIndex].x = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetIndex());
		m_pxmf4BoneIndex[nVertexIndex].y = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetIndex());
		m_pxmf4BoneIndex[nVertexIndex].z = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetIndex());
		m_pxmf4BoneIndex[nVertexIndex].w = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[3].GetIndex());

		m_pxmf3Weight[nVertexIndex].x = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetWeight());
		m_pxmf3Weight[nVertexIndex].y = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetWeight());
		m_pxmf3Weight[nVertexIndex++].z = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetWeight());
	}
	//create weight buffer data

	//create buffer
	m_pd3dWeightBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pxmf3Weight, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dBoneIndexBuffer = CreateBuffer(sizeof(XMFLOAT4), m_nVertices, m_pxmf4BoneIndex, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	//create buffer

	//-----------------------------------버퍼 조립-----------------------------------
	ID3D11Buffer *pd3dAnimationBuffers[2] = { m_pd3dWeightBuffer , m_pd3dBoneIndexBuffer };
	UINT pnAnimationBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT4) };
	UINT pnAnimationBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dAnimationBuffers, pnAnimationBufferStrides, pnAnimationBufferOffsets);
	//-----------------------------------버퍼 조립-----------------------------------

	if (m_ppd3dVertexBuffers)	return true;

	return false;
}
bool CFileBasedMesh::CreateIndexBuffer() {
	//중요중요 offset을 잘 설정 해야한다. 
	//처음 mesh가 인덱스로 되어있다면 모든 mesh가 index로 되어있다고 가정한다.
	if (!FBXIMPORTER->GetMeshDatas()[0].GetByControlPoint()) {
		//1. 모든 인덱스 정보 얻어옴
		
		m_nIndices = FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetIndexCnt();
		
		//2. 할당
		m_pnIndices = new UINT[m_nIndices];

		//3. offset을 더해가며 index데이터 완성
		//저장할 인덱스 
//		int nIndex{ 0 };
		int offset{ 0 };
		for (UINT j = 0; j < m_nIndices; ++j) {
			//mesh의 index가 0이 아닌 경우 offset을 더해간다.
			//offset을 더한 index를 저장한다. offest은 0번mesh는 0/ 1번 mesh는 0번 mesh의 정점 수만큼 offset을 가진다.
			m_pnIndices[j] = FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetIndexs()[j] + offset;
		}

		m_pd3dIndexBuffer = CreateBuffer(sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	}


	return true;
}

CFileBasedMesh::CFileBasedMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CFileBasedMesh::~CFileBasedMesh(){

}