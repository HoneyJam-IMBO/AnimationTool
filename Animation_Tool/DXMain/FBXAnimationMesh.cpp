#include "stdafx.h"
#include "FBXAnimationMesh.h"

XMMATRIX ConvertFbxMtxToXMMATRIX(FbxAMatrix FbxMtx) {
	return XMMATRIX{ 
		static_cast<float>(FbxMtx.Get(0,0)), static_cast<float>(FbxMtx.Get(0,1)), static_cast<float>(FbxMtx.Get(0,2)), static_cast<float>(FbxMtx.Get(0,3)),
		static_cast<float>(FbxMtx.Get(1,0)), static_cast<float>(FbxMtx.Get(1,1)), static_cast<float>(FbxMtx.Get(1,2)), static_cast<float>(FbxMtx.Get(1,3)),
		static_cast<float>(FbxMtx.Get(2,0)), static_cast<float>(FbxMtx.Get(2,1)), static_cast<float>(FbxMtx.Get(2,2)), static_cast<float>(FbxMtx.Get(2,3)),
		static_cast<float>(FbxMtx.Get(3,0)), static_cast<float>(FbxMtx.Get(3,1)), static_cast<float>(FbxMtx.Get(3,2)), static_cast<float>(FbxMtx.Get(3,3))
	};
}
#define ANIM_INDEX 0
bool CFBXAnimationMesh::Begin(UINT index){
	//test
	D3D11_RASTERIZER_DESC descRasterizer;
	ZeroMemory(&descRasterizer, sizeof(D3D11_RASTERIZER_DESC));
	//descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.FillMode = D3D11_FILL_WIREFRAME;
	descRasterizer.CullMode = D3D11_CULL_NONE;
	m_pd3dDevice->CreateRasterizerState(&descRasterizer, &m_pPreRasterizerState);
	//tset
	//mesh의 정점 인덱스, 노말 데이터 추출
	CUseFBXMesh::Begin(index);
	
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
	for (UINT j = 0; j < FBXIMPORTER->GetMeshDatas()[index].GetVertexCnt(); ++j) {

		pxmf4BoneIndex[nVertexIndex].x = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetIndex());
		pxmf4BoneIndex[nVertexIndex].y = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetIndex());
		pxmf4BoneIndex[nVertexIndex].z = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetIndex());
		pxmf4BoneIndex[nVertexIndex].w = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[3].GetIndex());

		pxmf3Weight[nVertexIndex].x = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetWeight());
		pxmf3Weight[nVertexIndex].y = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetWeight());
		pxmf3Weight[nVertexIndex++].z = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetWeight());
	}
	//create weight buffer data

	//create buffer
	m_pd3dBoneIndexBuffer = CreateBuffer(sizeof(XMFLOAT4), m_nVertices, pxmf4BoneIndex, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dWeightBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, pxmf3Weight, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	//create buffer
	
	//-----------------------------------버퍼 조립-----------------------------------
	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dBoneIndexBuffer, m_pd3dWeightBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT4), sizeof(XMFLOAT3) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	//-----------------------------------버퍼 조립-----------------------------------

	//bone data set
//	int jointIndex = 0;

	m_nJoint = static_cast<int>(FBXIMPORTER->GetAnimationDatas()[index].GetJointCnt());
	m_nFrameCnt = static_cast<int>(FBXIMPORTER->GetAnimStackData().GetAnimationLength());

	m_ppAnimationData = new XMMATRIX*[m_nFrameCnt];
	for (int iframe = 0; iframe < m_nFrameCnt; ++iframe) {
		m_ppAnimationData[iframe] = new XMMATRIX[m_nJoint];
	}
	m_pAnimationBindingData = new XMMATRIX[m_nJoint];

	for (int i = 0; i < m_nFrameCnt; ++i) {
		for (int j = 0; j < m_nJoint; ++j) {
			FbxAMatrix BoneTransform;
			if (FBXIMPORTER->GetAnimationDatas()[index].GetJointDatas()[j].GetKeyFrames().empty())
				BoneTransform.SetIdentity();
			else BoneTransform = FBXIMPORTER->GetAnimationDatas()[index].GetJointDatas()[j].GetKeyFrames()[i].GetKeyFrameTransformMtx();

			BoneTransform *= FBXIMPORTER->GetAnimationDatas()[index].GetJointDatas()[j].GetOffsetMtx();

			m_ppAnimationData[i][j] = XMMatrixTranspose(ConvertFbxMtxToXMMATRIX(BoneTransform));//그럼 최종 행렬 = A*B*C*D
		}
	}
	for (int j = 0; j < m_nJoint; ++j) {
		m_pAnimationBindingData[j] = m_ppAnimationData[m_nFrame][j];
	}
	m_pAnimBuffer = new CStaticBuffer(m_pd3dDevice, m_pd3dDeviceContext);
	m_pAnimBuffer->Begin(1, 6128, m_pAnimationBindingData, 10, BIND_VS);

	//delete[] pAnimationData;
	//bone data set

	return true;
}

bool CFBXAnimationMesh::End(){

	for (int i = 0; i < m_nFrameCnt; ++i) {
		delete[] m_ppAnimationData[i];
	}
	delete[] m_ppAnimationData;
	delete[] m_pAnimationBindingData;
	return CUseFBXMesh::End();
}

void CFBXAnimationMesh::SetShaderState(){

	//set texture
	for (auto pTexture : m_vMeshTexture) {
		pTexture->SetShaderState();
	}

	//update animation data
	void* pData = m_pAnimBuffer->Map();
	XMMATRIX* pAnimationData = static_cast<XMMATRIX*>(pData);
	
	if (m_nTime++ > 5) {
		m_nTime = 0;
		m_nFrame++;
		m_nFrame = (m_nFrame) % m_nFrameCnt;
	}
	int jointIndex = 0;
	
	for (int j = 0; j < m_nJoint; ++j) {
		pAnimationData[jointIndex++] = m_ppAnimationData[m_nFrame][j];
	}
	
	m_pAnimBuffer->Unmap();
	m_pAnimBuffer->SetShaderState();

	CMesh::SetShaderState();
}



void CFBXAnimationMesh::ProcessGetCtrlPoint(FbxNode * pNode){
	FbxMesh* pMesh = pNode->GetMesh();
	if (pMesh) {
		//int nIndices = pMesh->GetPolygonVertexCount();
		//UINT* pnIndices = new UINT[nIndices];
		//pnIndices = (UINT*)pMesh->GetPolygonVertices();

		//int nIndices = pMesh->GetPolygonVertexCount();
		//m_pnIndices = new UINT[nIndices];
		//m_pnIndices = (UINT*)pMesh->GetPolygonVertices();
		//get vertex
		m_nControllPoint = pMesh->GetControlPointsCount();
		CtrlPoint* pCtrlPoint = nullptr;

		//모든 정점 수 만큼
		for (int j = 0; j < m_nControllPoint; ++j) {
			pCtrlPoint = new CtrlPoint;
			pCtrlPoint->mPosition.x = static_cast<float>(pMesh->GetControlPointAt(j).mData[0]);
			pCtrlPoint->mPosition.y = static_cast<float>(pMesh->GetControlPointAt(j).mData[1]);
			pCtrlPoint->mPosition.z = static_cast<float>(pMesh->GetControlPointAt(j).mData[2]);
			m_mControlPoints[j] = pCtrlPoint;
		}//end for
	}//end if lMesh

	 //recursively traverse each node in the scene
	int i, lCount = pNode->GetChildCount();
	for (i = 0; i < lCount; i++) {
		ProcessGetCtrlPoint(pNode->GetChild(i));
	}

//	if (!pNode) return;
//
//	FbxMesh* pMesh = pNode->GetMesh();
//	if (pMesh) {
//		//get index
//		int nIndices = pMesh->GetPolygonVertexCount();
//		UINT* pnIndices = new UINT[nIndices];
//		pnIndices = (UINT*)pMesh->GetPolygonVertices();
//		int n = pnIndices[0];
//		//get vertex
//		m_nControllPoint = pMesh->GetControlPointsCount();
//		CtrlPoint* pCtrlPoint = nullptr;
//		 
//		//모든 정점 수 만큼
//		for (unsigned int j = 0; j < m_nControllPoint; ++j) {
//			pCtrlPoint  = new CtrlPoint;
//			pCtrlPoint ->mPosition.x = static_cast<float>(pMesh->GetControlPointAt(j).mData[0]);
//			pCtrlPoint ->mPosition.y = static_cast<float>(pMesh->GetControlPointAt(j).mData[1]);
//			pCtrlPoint ->mPosition.z = static_cast<float>(pMesh->GetControlPointAt(j).mData[2]);
//			m_mControlPoints[j] = pCtrlPoint;
//		}//end for
//	}//end if lMesh
//
//	for (int i = 0; i < pNode->GetChildCount(); i++){
//		ProcessGetCtrlPoint(pNode->GetChild(i));
//	}
}

void CFBXAnimationMesh::ProcessGetJointData(FbxNode * pNode){
	//root노드는 보지도 않음 바로 다음 노드로 넘어간다.
	for (int childIndex = 0; childIndex < pNode->GetChildCount(); ++childIndex){
		FbxNode* currNode = pNode->GetChild(childIndex);

		// 본(Bone)의 최상위의 인덱스는 -1 단계별로 뻗어갈 때보다 +1
		ProcessSkeletonHierarchyRecursively(currNode, 0, -1);
	}
	if (m_Skeleton.empty()){
		m_bHasAnimation = false;
	}
}

void CFBXAnimationMesh::ProcessGetAnimationData(FbxNode * pNode){
	if (pNode->GetNodeAttribute())
	{
		switch (pNode->GetNodeAttribute()->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			if (m_bHasAnimation)
			{
				ProcessJointsAndAnimations(pNode);
			}
			break;
		}
	}

	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		ProcessGetAnimationData(pNode->GetChild(i));
	}
}

void CFBXAnimationMesh::ProcessSkeletonHierarchyRecursively(FbxNode * inNode, int myIndex, int inParentIndex){
	if (inNode->GetNodeAttribute()
		&& inNode->GetNodeAttribute()->GetAttributeType()
		&& inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint currJoint;
		currJoint.m_ParentIndex = inParentIndex;
		currJoint.m_Name = inNode->GetName();
		currJoint.m_MyIndex = myIndex;
		m_Skeleton.push_back(currJoint);
	}
	for (int i = 0; i < inNode->GetChildCount(); i++)
	{
		ProcessSkeletonHierarchyRecursively(inNode->GetChild(i), static_cast<int>(m_Skeleton.size()), myIndex);
	}
}

void CFBXAnimationMesh::ProcessJointsAndAnimations(FbxNode * pNode){
	FbxMesh* currMesh = pNode->GetMesh();
	unsigned int numOfDeformers = currMesh->GetDeformerCount();
	// This geometry transform is something I cannot understand
	// I think it is from MotionBuilder
	// If you are using Maya for your models, 99% this is just an
	// identity matrix
	// But I am taking it into account anyways......
	FbxAMatrix geometryTransform = GetGeometryTransformation(pNode);

	// A deformer is a FBX thing, which contains some clusters
	// A cluster contains a link, which is basically a joint
	// Normally, there is only one deformer in a mesh
	for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		// There are many types of deformers in Maya,
		// We are using only skins, so we see if this is a skin
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!currSkin)
		{
			continue;
		}

		unsigned int numOfClusters = currSkin->GetClusterCount();
		for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
			std::string currJointName = currCluster->GetLink()->GetName();
			unsigned int currJointIndex = FindJointIndexUsingName(currJointName);
			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			currCluster->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
			currCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix *geometryTransform;

			// Update the information in mSkeleton 
			m_Skeleton[currJointIndex].m_mtxGlobalBindposeInverse = globalBindposeInverseMatrix;
			m_Skeleton[currJointIndex].m_pNode = currCluster->GetLink();

			// Associate each joint with the control points it affects
			unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < numOfIndices; ++i)
			{
				BlendingIndexWeightPair currBlendingIndexWeightPair;
				currBlendingIndexWeightPair.mBlendingIndex = currJointIndex;
				currBlendingIndexWeightPair.mBlendingWeight = static_cast<float>(currCluster->GetControlPointWeights()[i]);
				m_mControlPoints[currCluster->GetControlPointIndices()[i]]->mBlendingInfo.push_back(currBlendingIndexWeightPair);
			}

			// Get animation information
			// Now only supports one take
			FbxAnimStack* currAnimStack = FBXIMPORTER->GetScene()->GetSrcObject<FbxAnimStack>(0);
			FbxString animStackName = currAnimStack->GetName();
			mAnimationName = animStackName.Buffer();
			FbxTakeInfo* takeInfo = FBXIMPORTER->GetScene()->GetTakeInfo(animStackName);
			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
			mAnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;
			

			for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i <= end.GetFrameCount(FbxTime::eFrames24); ++i)
			{
				FbxTime currTime;
				currTime.SetFrame(i, FbxTime::eFrames24);
				KeyFrame* pKeyFrame = new KeyFrame();
				pKeyFrame->mFrameNum = i;
				FbxAMatrix currentTransformOffset = pNode->EvaluateGlobalTransform(currTime) * geometryTransform;
				pKeyFrame->mGlobalTransform = currentTransformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform(currTime);
				m_Skeleton[currJointIndex].m_vpKeyFrame.push_back(pKeyFrame);
			}
		}
	}

	// Some of the control points only have less than 4 joints
	// affecting them.
	// For a normal renderer, there are usually 4 joints
	// I am adding more dummy joints if there isn't enough
	BlendingIndexWeightPair currBlendingIndexWeightPair;
	currBlendingIndexWeightPair.mBlendingIndex = 0;
	currBlendingIndexWeightPair.mBlendingWeight = 0;
	for (auto itr = m_mControlPoints.begin(); itr != m_mControlPoints.end(); ++itr)
	{
		for (size_t i = itr->second->mBlendingInfo.size(); i <= 4; ++i)
		{
			itr->second->mBlendingInfo.push_back(currBlendingIndexWeightPair);
		}
	}
}
void CFBXAnimationMesh::TransferkeyVertexInfo()
{
	//int influenceInfoSize = m_pSkinnedIterator->GetControlPoint().size();

	//if (influenceInfoSize == pMesh->GetControlPointsCount())
	{

		//const FbxVector4* pControlPoints = pMesh->GetControlPoints();
		FbxVector4 currVertex;
		for (auto& Data : m_mControlPoints)
		{
			int boneIndices[4] = { 0, 0, 0, 0 };
			float boneWeights[4] = { 0.0, 0.0, 0.0, 0.0 };
			CtrlPoint* pCtrlPoint = Data.second;
			if (pCtrlPoint->mBlendingInfo.size() > 4)
			{
				//내림차순 
				sort(pCtrlPoint->mBlendingInfo.begin(), pCtrlPoint->mBlendingInfo.end(), [](const BlendingIndexWeightPair& lhs, const BlendingIndexWeightPair& rhs)
				{
					return lhs.mBlendingWeight > rhs.mBlendingWeight;
				});

				//nomalize
				float sumWeight = 0.0f, interpolateWeight = 0.0f;
				for (int i = 0; i < 4; ++i)
				{
					sumWeight += pCtrlPoint->mBlendingInfo[i].mBlendingWeight;
				}
				
				interpolateWeight = 1.0f - sumWeight;
				//nomalize

				//3.
				auto removeIter = pCtrlPoint->mBlendingInfo.begin() + 4;
				pCtrlPoint->mBlendingInfo.erase(removeIter, pCtrlPoint->mBlendingInfo.end());
				//4.
				pCtrlPoint->mBlendingInfo[0].mBlendingWeight += interpolateWeight;
			}

			int influenceCount = 0;
			for (auto influenceInfo : pCtrlPoint->mBlendingInfo)
			{

				pCtrlPoint->mBlendingInfo[influenceCount].mBlendingIndex = influenceInfo.mBlendingIndex;
				pCtrlPoint->mBlendingInfo[influenceCount++].mBlendingWeight = influenceInfo.mBlendingWeight;

			}

		}
	}


}

unsigned int CFBXAnimationMesh::FindJointIndexUsingName(const std::string& inJointName)
{
	for (unsigned int i = 0; i < m_Skeleton.size(); ++i)
	{
		if (m_Skeleton[i].m_Name == inJointName)
		{
			return i;
		}
	}

	throw std::exception("Skeleton information in FBX file is corrupted.");
}

FbxAMatrix CFBXAnimationMesh::GetGeometryTransformation(FbxNode* pNode)
{
	if (!pNode)
	{
		throw std::exception("Null for mesh geometry");
	}

	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}