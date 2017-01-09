#pragma once
#include "UseFBXMesh.h"
#include "StaticBuffer.h"
#include "FbxJointData.h"

struct KeyFrame {
	FbxLongLong		mFrameNum;
	FbxAMatrix		mGlobalTransform;

	~KeyFrame() { mGlobalTransform.SetIdentity(); }
};

//����Ʈ ������ŭ 
//0 GlobalBindposeInverse
//1 ~ m_vpKeyFrame .size��ŭ 
//frame�� num�� global transform�� �־����
struct Joint{
	int m_ParentIndex{ 0 };
	int m_MyIndex{ 0 };
	std::string m_Name;

	XMMATRIX m_mtxGlobalBindposeInverse;
	FbxNode* m_pNode{ nullptr };

	//�������� ������ŭ ������ �־�� �Ѵ�.
	vector<KeyFrame*> m_vpKeyFrame;//���� �����Ӻ��� ������

	Joint() :
		m_pNode(nullptr)
	{
		m_mtxGlobalBindposeInverse = XMMatrixIdentity();
	}

	~Joint(){

	}
}; 
struct BlendingIndexWeightPair{
	unsigned int mBlendingIndex;
	float mBlendingWeight;

	BlendingIndexWeightPair() :
		mBlendingIndex(0),
		mBlendingWeight(0)
	{}
};
struct CtrlPoint
{
	//�� ������ blendinfo�� vertex buffer��
	XMFLOAT3 mPosition;
	std::vector<BlendingIndexWeightPair> mBlendingInfo;//4���� weightpair

	CtrlPoint()
	{
	}
};
//typedef map<int, Joint> mapJoint;
//typedef pair<int, Joint> pairJoint;

class CFBXAnimationMesh : public CUseFBXMesh{
public:
	//----------------------------dxobject-----------------------------
	bool Begin(UINT index = 0);
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState() { m_pAnimBuffer->CleanShaderState(); CMesh::CleanShaderState(); }
	//----------------------------dxobject-----------------------------

	vector<CFbxJointData>& GetvJoint() { return m_Skeleton; }

	//---------------------------mesh----------------------------------
	virtual void Update(float fTimeElapsed);
	//virtual void RenderExcute(UINT nnInstance);
	//begin func
	void ProcessGetCtrlPoint(FbxNode* pNode);
	void ProcessGetJointData(FbxNode* pNode);
	void ProcessGetAnimationData(FbxNode* pNode);
	//begin func
	void TransferkeyVertexInfo();
	//---------------------------mesh----------------------------------

	float& GetCurFrame() { return m_nFrame; }
	int& GetFrameCnt() { return m_nFrameCnt; }
	bool& GetbAnimation() { return m_bAnimation; }
	float& GetAnimationSpd() { return m_fAnimationSpd; }
private:
	//skelleton
	vector<CFbxJointData> m_Skeleton;
	bool m_bHasAnimation{ true };
	//ctrl point
	unordered_map<unsigned int, CtrlPoint*> m_mControlPoints;
	int m_nControllPoint{ 0 };
	//animation 
	FbxLongLong mAnimationLength;
	std::string mAnimationName;
	//buffer
	CStaticBuffer* m_pAnimBuffer{ nullptr };

	//vertex buffer
	ID3D11Buffer* m_pd3dBoneIndexBuffer{ nullptr };
	ID3D11Buffer* m_pd3dWeightBuffer{ nullptr };

	//test
	XMFLOAT4* m_pxmf4BoneIndex;
	XMFLOAT3* m_pxmf3Weight;
	XMMATRIX** m_ppAnimationData;
	XMMATRIX* m_pAnimationJointOffsetMtx;
	int m_nJoint{ 0 };
	int m_nFrameCnt{ 0 };
	ID3D11RasterizerState* m_pPreRasterizerState{ nullptr };
	float m_nFrame{ 0 };
	int m_nTime{ 0 };
	bool m_bAnimation{ true };
	float m_fAnimationSpd{ 1.0f };
	//test

	//helper func
	void ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int myIndex, int inParentIndex);
	void ProcessJointsAndAnimations(FbxNode* pNode);
	unsigned int FindJointIndexUsingName(const std::string& inJointName);
	FbxAMatrix GetGeometryTransformation(FbxNode* inNode);
public:
	CFBXAnimationMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CUseFBXMesh(pd3dDevice, pd3dDeviceContext) {};
	virtual ~CFBXAnimationMesh() {};
};