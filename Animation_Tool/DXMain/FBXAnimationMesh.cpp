#include "stdafx.h"
#include "FBXAnimationMesh.h"

bool CFBXAnimationMesh::Begin(UINT index){
	//mesh의 정점 인덱스, 노말 데이터 추출
	CUseFBXMesh::Begin(index);
	m_MeshIndex = index;
	////ctrl point data얻어옴 다시..
	//ProcessGetCtrlPoint(FBXIMPORTER->GetScene()->GetRootNode());
	////joint data를 스케치함
	//ProcessGetJointData(FBXIMPORTER->GetScene()->GetRootNode());
	////animation 정보를 본격적으로 가져옴
	//ProcessGetAnimationData(FBXIMPORTER->GetScene()->GetRootNode());
	////weight, index값 설정
	//TransferkeyVertexInfo();

	//create bone index buffer data
	XMFLOAT4* pxmf4BoneIndex = new XMFLOAT4[m_nVertices];
	XMFLOAT3* pxmf3Weight = new XMFLOAT3[m_nVertices];

	//모든 mesh에 대해서
	int nVertexIndex{ 0 };
	for (UINT j = 0; j < FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexCnt(); ++j) {

		pxmf4BoneIndex[nVertexIndex].x = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetIndex());
		pxmf4BoneIndex[nVertexIndex].y = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetIndex());
		pxmf4BoneIndex[nVertexIndex].z = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetIndex());
		pxmf4BoneIndex[nVertexIndex].w = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[3].GetIndex());

		pxmf3Weight[nVertexIndex].x = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetWeight());
		pxmf3Weight[nVertexIndex].y = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetWeight());
		pxmf3Weight[nVertexIndex++].z = static_cast<float>(FBXIMPORTER->GetMeshDatas()[m_MeshIndex].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetWeight());
	}
	//create weight buffer data

	//create buffer
	m_pd3dWeightBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, pxmf3Weight, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	ID3D11Buffer* pd3dBoneIndexBuffer = CreateBuffer(sizeof(XMFLOAT4), m_nVertices, pxmf4BoneIndex, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	//create buffer
	
	//-----------------------------------버퍼 조립-----------------------------------
	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dWeightBuffer , pd3dBoneIndexBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT4) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	//-----------------------------------버퍼 조립-----------------------------------
	m_pd3dBoneIndexBuffer = CreateBuffer(sizeof(XMFLOAT4), m_nVertices, pxmf4BoneIndex, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	ExchangeVertexBuffer(m_nVertexBuffers - 1, m_pd3dBoneIndexBuffer, sizeof(XMFLOAT4), 0);
	//m_pAnimationInfo = new CAnimationInfo(m_pd3dDevice, m_pd3dDeviceContext);
	//m_pAnimationInfo->Begin(0);

	//선택한 obb 표시를 위한 obb 나중에 필요 없음
	//for (int j = 0; j < m_pAnimationInfo->GetJoints().size(); ++j) {
	//	CBoundingBox boundingBox;
	//	boundingBox.Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(5.f, 5.f, 5.f, 1.f));
	//	m_vOBB.push_back(boundingBox);
	//}
	return true;
}

bool CFBXAnimationMesh::End(){
	//m_pAnimationInfo->End();
	//delete m_pAnimationInfo;

	return CUseFBXMesh::End();
}

void CFBXAnimationMesh::SetShaderState(){

	//set texture
	for (auto pTexture : m_vMeshTexture) {
		pTexture->SetShaderState();
	}
	//m_pAnimationInfo->SetShaderState();

	CMesh::SetShaderState();
}

void CFBXAnimationMesh::CleanShaderState(){
	//m_pAnimationInfo->CleanShaderState();
	CMesh::CleanShaderState();
}



void CFBXAnimationMesh::Update(float fTimeElapsed){
	//m_pAnimationInfo->Update(fTimeElapsed);
	
	//for (int j = 0; j < m_pAnimationInfo->GetJoints().size(); ++j) {
	//	if (m_vOBB[j].GetActive()) {
	//		DEBUGER->RegistCoordinateSys(m_pAnimationInfo->GetCurFrameMtx(j));
	//		//BoundingOrientedBox originObb = m_vOBB[j].GetOBB();
	//		//originObb.Transform(originObb, m_pAnimationInfo->GetCurFrameMtx(j));
	//		//DEBUGER->RegistOBB(originObb);
	//	}
	//}
}
