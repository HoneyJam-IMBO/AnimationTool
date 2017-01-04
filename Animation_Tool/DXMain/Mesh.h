#pragma once

#include "DXObject.h"
#include "Texture.h"

// 꼭짓점별 데이터를 꼭짓점 셰이더로 보내는 데 사용됩니다.
struct VertexPositionColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};

class CMesh : public DXObject {

public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();
	//----------------------------dxobject-----------------------------

	//---------------------------mesh----------------------------------
	virtual void RenderExcute(UINT nInstance);
	//begin func
	virtual bool CreateVertexBuffer() { return true; }
	virtual bool CreateIndexBuffer() { return true; }
	//begin func

	//aabb 
	BoundingBox GetBoundingBox() { return m_BoundingBox; }

	void CMesh::CalculateVertexNormal(XMVECTOR *pxmvNormals);
	void CMesh::SetTriAngleListVertexNormal(XMVECTOR *pxmvNormals);
	XMVECTOR CMesh::CalculateTriAngleNormal(UINT nIndex0, UINT nIndex1, UINT nIndex2);
	void CMesh::SetAverageVertexNormal(XMVECTOR* pxmvNormals, int nPrimitives, int nOffset, bool bStrip);

	void AssembleToVertexBuffer(int nBuffers, ID3D11Buffer** ppd3dBuffers, UINT* pnBufferStrides, UINT* pnBufferOffsets);
	//tbn을 구하는데 들어오는 점을 가지고 만든다. 실제로 이용하는 것은 점 3개이다.
	void CreateTBFromPoints(XMFLOAT3* pPositions, XMFLOAT2* pUVs, XMFLOAT3& outT, XMFLOAT3& outB);
	//---------------------------mesh----------------------------------

	//setter
	void Setd3dPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY d3dPrimitiveTopology) {m_d3dPrimitiveTopology = d3dPrimitiveTopology;}
	void SetnVertices(UINT nVertices) {	m_nVertices = nVertices;}
	void SetnIndices(UINT nIndices) {	m_nIndices = nIndices;}
	void SetpnIndices(UINT* pnIndices) {m_pnIndices = pnIndices;}
	void SetpVertices(XMFLOAT3* pVertices) {m_pVertices = pVertices;}
	//-----------------------------------render option-------------------------
	void SetnStartIndexLocation(UINT nStartIndexLocation) {	m_nStartIndexLocation = nStartIndexLocation;}
	void SetnStartVertexLocation(UINT nStartVertexLocation) {m_nStartVertexLocation = nStartVertexLocation;}
	void SetnBaseVertexLocation(UINT nBaseVertexLocation) {	m_nBaseVertexLocation = nBaseVertexLocation;}
	void SetnStartInstanceLocation(UINT nStartInstanceLocation) {m_nStartInstanceLocation = nStartInstanceLocation;}
	//-----------------------------------render option-------------------------
	//---------------------------------vertex buffer------------------------
	void SetnStartSlot(UINT nStartSlot) {m_nStartSlot = nStartSlot;}
	void SetnVertexBuffers(int nVertexBuffers) {m_nVertexBuffers = nVertexBuffers;}
	void SetpnVertexOffsets(UINT* pnVertexOffsets) {m_pnVertexOffsets = pnVertexOffsets;}
	void SetpnVertexStrides(UINT* pnVertexStrides) {m_pnVertexStrides = pnVertexStrides;}
	void Setppd3dVertexBuffers(ID3D11Buffer** ppd3dVertexBuffers){	m_ppd3dVertexBuffers = ppd3dVertexBuffers;}
	//---------------------------------vertex buffer------------------------
	//---------------------------------index buffer-------------------------
	void Setd3dIndexFormat(DXGI_FORMAT d3dIndexFormat) {m_d3dIndexFormat = d3dIndexFormat;}
	void Setpd3dIndexBuffer(ID3D11Buffer* pd3dIndexBuffer) {m_pd3dIndexBuffer = pd3dIndexBuffer;}
	void SetnIndexOffset(UINT nIndexOffset) { m_nIndexOffset = nIndexOffset; }
	//---------------------------------index buffer-------------------------

	void AddMeshTexture(shared_ptr<CTexture>);
	//setter
protected:
	//aabb
	BoundingBox m_BoundingBox;
	vector<shared_ptr<CTexture>> m_vMeshTexture;

	//topology
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

	UINT									m_nVertices{ 0 };
	UINT									m_nIndices{ 0 };
	UINT*									m_pnIndices{ nullptr };
	XMFLOAT3*								m_pVertices{ nullptr };

	//-----------------------------------render option-------------------------
	UINT m_nStartIndexLocation{ 0 };
	UINT m_nStartVertexLocation{ 0 };
	UINT m_nBaseVertexLocation{ 0 };
	UINT m_nStartInstanceLocation{ 0 };
	//-----------------------------------render option-------------------------

	//---------------------------------vertex buffer------------------------
	UINT								m_nStartSlot{ 0 };
	int									m_nVertexBuffers{ 0 };
	UINT*								m_pnVertexOffsets{ 0 };
	UINT*								m_pnVertexStrides{ 0 };
	ID3D11Buffer**						m_ppd3dVertexBuffers{ nullptr };
	//---------------------------------vertex buffer------------------------
	
	//---------------------------------index buffer-------------------------
	DXGI_FORMAT							m_d3dIndexFormat{ DXGI_FORMAT_R32_UINT };
	ID3D11Buffer*						m_pd3dIndexBuffer{ nullptr };
	UINT								m_nIndexOffset{ 0 };
	//---------------------------------index buffer-------------------------
public:
	CMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CMesh();
};