#pragma once
#include "UseFBXMesh.h"
#include "StaticBuffer.h"

struct KeyFrame {
	FbxLongLong		mFrameNum;
	FbxAMatrix		mGlobalTransform;

	~KeyFrame() { mGlobalTransform.SetIdentity(); }
};

//조인트 개수만큼 
//0 GlobalBindposeInverse
//1 ~ m_vpKeyFrame .size만큼 
//frame의 num과 global transform이 있어야함
struct Joint{
	int m_ParentIndex{ 0 };
	int m_MyIndex{ 0 };
	std::string m_Name;

	FbxAMatrix m_mtxGlobalBindposeInverse;
	FbxNode* m_pNode{ nullptr };

	//프레임의 개수만큼 정보가 있어야 한다.
	vector<KeyFrame*> m_vpKeyFrame;//시작 프레임부터 끝까지

	Joint() :
		m_pNode(nullptr)
	{
		m_mtxGlobalBindposeInverse.SetIdentity();
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
	//각 정점의 blendinfo는 vertex buffer로
	XMFLOAT3 mPosition;
	std::vector<BlendingIndexWeightPair> mBlendingInfo;//4개의 weightpair

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

	//---------------------------mesh----------------------------------
	//virtual void RenderExcute(UINT nnInstance);
	//begin func
	void ProcessGetCtrlPoint(FbxNode* pNode);
	void ProcessGetJointData(FbxNode* pNode);
	void ProcessGetAnimationData(FbxNode* pNode);
	//begin func
	void TransferkeyVertexInfo();
	//---------------------------mesh----------------------------------
private:
	//skelleton
	vector<Joint> m_Skeleton;
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
	XMMATRIX* m_pAnimationBindingData;
	int m_nJoint{ 0 };
	int m_nFrameCnt{ 0 };
	ID3D11RasterizerState* m_pPreRasterizerState{ nullptr };
	int m_nFrame{ 0 };
	int m_nTime{ 0 };
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