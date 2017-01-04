#include "stdafx.h"
#include "Mesh.h"

bool CMesh::Begin() {

	//create default aabb
	BoundingBox::CreateFromPoints(m_BoundingBox, XMVectorSet(+FLT_MAX, +FLT_MAX, +FLT_MAX, 0.f), XMVectorSet(-FLT_MAX, -FLT_MAX, -FLT_MAX, 0.f));
	
	if (false == CreateVertexBuffer()) return false;

	if (false == CreateIndexBuffer()) return false;
	
	return true;
}
bool CMesh::End() {
	//----------------------vertex buffer release-------------------
	if (m_ppd3dVertexBuffers) {
		for (int i = 0; i < m_nVertexBuffers; ++i) {
			if(m_ppd3dVertexBuffers[i]) m_ppd3dVertexBuffers[i]->Release();
		}
	}
	//----------------------vertex buffer release-------------------
	//----------------------index buffe rrelease------------------
	if (m_pd3dIndexBuffer)			m_pd3dIndexBuffer->Release();
	//----------------------index buffe rrelease------------------

	return true;
}
void CMesh::SetShaderState() {
	for (auto pTexture : m_vMeshTexture) {
		pTexture->SetShaderState();
	}
	//메쉬
	m_pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	// 꼭지점 버퍼를 연결합니다.
	m_pd3dDeviceContext->IASetVertexBuffers(m_nStartSlot, m_nVertexBuffers, m_ppd3dVertexBuffers, m_pnVertexStrides, m_pnVertexOffsets);

	// 인덱스 버퍼를 연결합니다.
	m_pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, m_d3dIndexFormat, m_nIndexOffset);
}

void CMesh::CleanShaderState(){
	for (auto pTexture : m_vMeshTexture) {
		pTexture->CleanShaderState();
	}
}

void CMesh::RenderExcute(UINT nInstance) {

	if (m_pd3dIndexBuffer)
		m_pd3dDeviceContext->DrawIndexedInstanced(m_nIndices, nInstance, m_nStartIndexLocation, m_nBaseVertexLocation, m_nStartInstanceLocation);
	else
		m_pd3dDeviceContext->DrawInstanced(m_nVertices, nInstance, m_nStartVertexLocation, m_nStartInstanceLocation);

}

void CMesh::AssembleToVertexBuffer(int nBuffers, ID3D11Buffer **ppd3dBuffers, UINT *pnBufferStrides, UINT *pnBufferOffsets)
{
	ID3D11Buffer **ppd3dNewVertexBuffers = new ID3D11Buffer*[m_nVertexBuffers + nBuffers];
	UINT *pnNewVertexStrides = new UINT[m_nVertexBuffers + nBuffers];
	UINT *pnNewVertexOffsets = new UINT[m_nVertexBuffers + nBuffers];

	if (m_nVertexBuffers > 0)
	{
		for (int i = 0; i < m_nVertexBuffers; i++)
		{
			ppd3dNewVertexBuffers[i] = m_ppd3dVertexBuffers[i];
			pnNewVertexStrides[i] = m_pnVertexStrides[i];
			pnNewVertexOffsets[i] = m_pnVertexOffsets[i];
		}
		if (m_ppd3dVertexBuffers) delete[] m_ppd3dVertexBuffers;
		if (m_pnVertexStrides) delete[] m_pnVertexStrides;
		if (m_pnVertexOffsets) delete[] m_pnVertexOffsets;
	}

	for (int i = 0; i < nBuffers; i++)
	{
		ppd3dNewVertexBuffers[m_nVertexBuffers + i] = ppd3dBuffers[i];
		pnNewVertexStrides[m_nVertexBuffers + i] = pnBufferStrides[i];
		pnNewVertexOffsets[m_nVertexBuffers + i] = pnBufferOffsets[i];
	}

	m_nVertexBuffers += nBuffers;
	m_ppd3dVertexBuffers = ppd3dNewVertexBuffers;
	m_pnVertexStrides = pnNewVertexStrides;
	m_pnVertexOffsets = pnNewVertexOffsets;
}

void CMesh::CreateTBFromPoints(XMFLOAT3 * pPositions, XMFLOAT2 * pUVs, XMFLOAT3 & outT, XMFLOAT3 & outB){
	XMVECTOR xmvPos[3];
	XMVECTOR xmvUV[3];
	for (int i = 0; i < 3; ++i) {
		xmvPos[i] = XMLoadFloat3(&pPositions[i]);
		xmvUV[i] = XMLoadFloat2(&pUVs[i]);
	}
	XMVECTOR e0 = xmvPos[1] - xmvPos[0];
	XMVECTOR e1 = xmvPos[2] - xmvPos[0];

	//p0 부터 p1까지의 uv좌표 변화량 
	XMFLOAT2 UV0;
	XMStoreFloat2(&UV0, (xmvUV[1] - xmvUV[0]));
	//p0 부터 p2까지의 uv좌표 변화량
	XMFLOAT2 UV1;
	XMStoreFloat2(&UV1, (xmvUV[2] - xmvUV[0]));

	//u0*v1 - v0*u1
	float inverseValue = 1 / (UV0.x * UV1.y - UV0.y * UV1.x);
	float x = inverseValue * UV1.y;
	float y = inverseValue * -UV0.y;
	float z = inverseValue * -UV1.x;
	float w = inverseValue * UV0.x;

	XMStoreFloat3(&outT, XMVector3Normalize(e0*x + e1*y));
	XMStoreFloat3(&outB, XMVector3Normalize(e0*z + e1*w));
}
void CMesh::AddMeshTexture(shared_ptr<CTexture> pTexture){
	m_vMeshTexture.emplace_back(pTexture);
}
XMVECTOR CMesh::CalculateTriAngleNormal(UINT nIndex0, UINT nIndex1, UINT nIndex2)
{
	XMVECTOR xmvNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.f);
	XMVECTOR xmvP0 = XMLoadFloat3(&m_pVertices[nIndex0]);
	XMVECTOR xmvP1 = XMLoadFloat3(&m_pVertices[nIndex1]);
	XMVECTOR xmvP2 = XMLoadFloat3(&m_pVertices[nIndex2]);
	XMVECTOR xmvEdge1 = xmvP1 - xmvP0;
	XMVECTOR xmvEdge2 = xmvP2 - xmvP0;
	xmvNormal = XMVector3Cross(xmvEdge1, xmvEdge2);

	return XMVector3Normalize(xmvNormal);
}

void CMesh::SetTriAngleListVertexNormal(XMVECTOR *pxmvNormals)
{
	XMVECTOR xmvNormal;
	XMFLOAT3 *pxmf3Positions = NULL;
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		xmvNormal = CalculateTriAngleNormal((i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
		pxmf3Positions = m_pVertices + (i * 3 + 0);
		pxmvNormals[i * 3 + 0] = xmvNormal;
		pxmf3Positions = m_pVertices + (i * 3 + 1);
		pxmvNormals[i * 3 + 1] = xmvNormal;
		pxmf3Positions = m_pVertices + (i * 3 + 2);
		pxmvNormals[i * 3 + 2] = xmvNormal;
	}
}

void CMesh::SetAverageVertexNormal(XMVECTOR* pxmvNormals, int nPrimitives, int nOffset, bool bStrip)
{
	XMVECTOR xmvSumOfNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.f);
	XMFLOAT3 *pxmf3Positions = NULL;
	UINT nIndex0, nIndex1, nIndex2;

	for (unsigned int j = 0; j < m_nVertices; j++)
	{
		xmvSumOfNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.f);
		for (int i = 0; i < nPrimitives; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (m_pnIndices) nIndex0 = m_pnIndices[nIndex0];
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (m_pnIndices) nIndex1 = m_pnIndices[nIndex1];
			nIndex2 = (m_pnIndices) ? m_pnIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)) xmvSumOfNormal += CalculateTriAngleNormal(nIndex0, nIndex1, nIndex2);
		}
		xmvSumOfNormal = XMVector3Normalize(xmvSumOfNormal);
		pxmvNormals[j] = xmvSumOfNormal;
		pxmf3Positions = m_pVertices + j;
	}
}

void CMesh::CalculateVertexNormal(XMVECTOR *pxmvNormals)
{
	switch (m_d3dPrimitiveTopology)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		if (!m_pnIndices)
			SetTriAngleListVertexNormal(pxmvNormals);
		else
			SetAverageVertexNormal(pxmvNormals, (m_nIndices / 3), 3, false);
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		SetAverageVertexNormal(pxmvNormals, (m_pnIndices) ? (m_nIndices - 2) : (m_nVertices - 2), 1, true);
		break;
	default:
		break;
	}
}

CMesh::CMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : DXObject("mesh", pd3dDevice, pd3dDeviceContext) {
	

}
CMesh::~CMesh() {

}