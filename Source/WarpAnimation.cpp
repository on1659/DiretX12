#include "stdafx.h"
#include "WarpAnimation.h"



CWarpAnimation::CWarpAnimation(const std::string& animfileName, float scale)
{
	m_BoneKeyframeList.clear();
	m_Scale = scale;
	std::string dataComment;	//#
	std::string dataKeyword;	//:
	std::string nonDataString;	//
	UINT listStackNum = 0;

	m_StartKeyframe = 0;
	m_EndKeyframe = 0;

	std::ifstream m_AnimFile;
	m_AnimFile.open(animfileName);

	m_AnimFile >> dataComment;
	m_AnimFile >> dataKeyword >> m_KeyframeLength;
	m_AnimFile >> dataKeyword >> m_BoneCount;
	m_AnimFile >> dataKeyword >> m_WeightVertexCount;
	m_BoneKeyframeList.reserve(m_BoneCount);

	BoneKeyframedata tempAnimData;
	tempAnimData.mKeyframeData.reserve(m_KeyframeLength);
	Keyframedata tempKeyframeDB;

	std::vector<FbxAMatrix> gfMtx;

	for (auto i = 0; i < m_BoneCount; i++)
	{
		m_AnimFile >> dataComment >> tempAnimData.mBoneName;
		m_AnimFile >> dataComment
			>> tempAnimData.mGlobalBindposeInverse.mData[0].mData[0] >> tempAnimData.mGlobalBindposeInverse.mData[0].mData[1] >> tempAnimData.mGlobalBindposeInverse.mData[0].mData[2] >> tempAnimData.mGlobalBindposeInverse.mData[0].mData[3]
			>> tempAnimData.mGlobalBindposeInverse.mData[1].mData[0] >> tempAnimData.mGlobalBindposeInverse.mData[1].mData[1] >> tempAnimData.mGlobalBindposeInverse.mData[1].mData[2] >> tempAnimData.mGlobalBindposeInverse.mData[1].mData[3]
			>> tempAnimData.mGlobalBindposeInverse.mData[2].mData[0] >> tempAnimData.mGlobalBindposeInverse.mData[2].mData[1] >> tempAnimData.mGlobalBindposeInverse.mData[2].mData[2] >> tempAnimData.mGlobalBindposeInverse.mData[2].mData[3]
			>> tempAnimData.mGlobalBindposeInverse.mData[3].mData[0] >> tempAnimData.mGlobalBindposeInverse.mData[3].mData[1] >> tempAnimData.mGlobalBindposeInverse.mData[3].mData[2] >> tempAnimData.mGlobalBindposeInverse.mData[3].mData[3];


		tempAnimData.mGlobalBindposeInverse.SetT(
			FbxVector4(tempAnimData.mGlobalBindposeInverse.GetT().mData[0] * m_Scale,
				tempAnimData.mGlobalBindposeInverse.GetT().mData[1] * m_Scale,
				tempAnimData.mGlobalBindposeInverse.GetT().mData[2] * m_Scale));
		//>> tempAnimData.mGlobalBindposeInverse._11 >> tempAnimData.mGlobalBindposeInverse._12 >> tempAnimData.mGlobalBindposeInverse._13 >> tempAnimData.mGlobalBindposeInverse._14
		//>> tempAnimData.mGlobalBindposeInverse._21 >> tempAnimData.mGlobalBindposeInverse._22 >> tempAnimData.mGlobalBindposeInverse._23 >> tempAnimData.mGlobalBindposeInverse._24
		//>> tempAnimData.mGlobalBindposeInverse._31 >> tempAnimData.mGlobalBindposeInverse._32 >> tempAnimData.mGlobalBindposeInverse._33 >> tempAnimData.mGlobalBindposeInverse._34
		//>> tempAnimData.mGlobalBindposeInverse._41 >> tempAnimData.mGlobalBindposeInverse._42 >> tempAnimData.mGlobalBindposeInverse._43 >> tempAnimData.mGlobalBindposeInverse._44;

		//>> tempAnimData.mGlobalBindposeInverse._11 >> tempAnimData.mGlobalBindposeInverse._21 >> tempAnimData.mGlobalBindposeInverse._31 >> tempAnimData.mGlobalBindposeInverse._41
		//>> tempAnimData.mGlobalBindposeInverse._12 >> tempAnimData.mGlobalBindposeInverse._22 >> tempAnimData.mGlobalBindposeInverse._32 >> tempAnimData.mGlobalBindposeInverse._42
		//>> tempAnimData.mGlobalBindposeInverse._13 >> tempAnimData.mGlobalBindposeInverse._23 >> tempAnimData.mGlobalBindposeInverse._33 >> tempAnimData.mGlobalBindposeInverse._43
		//>> tempAnimData.mGlobalBindposeInverse._14 >> tempAnimData.mGlobalBindposeInverse._24 >> tempAnimData.mGlobalBindposeInverse._34 >> tempAnimData.mGlobalBindposeInverse._44;

		gfMtx.push_back(tempAnimData.mGlobalBindposeInverse);

		tempAnimData.mListIndex = listStackNum++;

		for (auto j = 0; j < m_KeyframeLength; j++)
		{
			m_AnimFile >> dataKeyword >> tempKeyframeDB.mKeyframeIndex
				>> tempKeyframeDB.mGlobalTransform.mData[0].mData[0] >> tempKeyframeDB.mGlobalTransform.mData[0].mData[1] >> tempKeyframeDB.mGlobalTransform.mData[0].mData[2] >> tempKeyframeDB.mGlobalTransform.mData[0].mData[3]
				>> tempKeyframeDB.mGlobalTransform.mData[1].mData[0] >> tempKeyframeDB.mGlobalTransform.mData[1].mData[1] >> tempKeyframeDB.mGlobalTransform.mData[1].mData[2] >> tempKeyframeDB.mGlobalTransform.mData[1].mData[3]
				>> tempKeyframeDB.mGlobalTransform.mData[2].mData[0] >> tempKeyframeDB.mGlobalTransform.mData[2].mData[1] >> tempKeyframeDB.mGlobalTransform.mData[2].mData[2] >> tempKeyframeDB.mGlobalTransform.mData[2].mData[3]
				>> tempKeyframeDB.mGlobalTransform.mData[3].mData[0] >> tempKeyframeDB.mGlobalTransform.mData[3].mData[1] >> tempKeyframeDB.mGlobalTransform.mData[3].mData[2] >> tempKeyframeDB.mGlobalTransform.mData[3].mData[3];

			//>> tempKeyframeDB.mGlobalTransform._11 >> tempKeyframeDB.mGlobalTransform._12 >> tempKeyframeDB.mGlobalTransform._13 >> tempKeyframeDB.mGlobalTransform._14
			//>> tempKeyframeDB.mGlobalTransform._21 >> tempKeyframeDB.mGlobalTransform._22 >> tempKeyframeDB.mGlobalTransform._23 >> tempKeyframeDB.mGlobalTransform._24
			//>> tempKeyframeDB.mGlobalTransform._31 >> tempKeyframeDB.mGlobalTransform._32 >> tempKeyframeDB.mGlobalTransform._33 >> tempKeyframeDB.mGlobalTransform._34
			//>> tempKeyframeDB.mGlobalTransform._41 >> tempKeyframeDB.mGlobalTransform._42 >> tempKeyframeDB.mGlobalTransform._43 >> tempKeyframeDB.mGlobalTransform._44;

			//>> tempKeyframeDB.mGlobalTransform._11 >> tempKeyframeDB.mGlobalTransform._21 >> tempKeyframeDB.mGlobalTransform._31 >> tempKeyframeDB.mGlobalTransform._41
			//>> tempKeyframeDB.mGlobalTransform._12 >> tempKeyframeDB.mGlobalTransform._22 >> tempKeyframeDB.mGlobalTransform._32 >> tempKeyframeDB.mGlobalTransform._42
			//>> tempKeyframeDB.mGlobalTransform._13 >> tempKeyframeDB.mGlobalTransform._23 >> tempKeyframeDB.mGlobalTransform._33 >> tempKeyframeDB.mGlobalTransform._43
			//>> tempKeyframeDB.mGlobalTransform._14 >> tempKeyframeDB.mGlobalTransform._24 >> tempKeyframeDB.mGlobalTransform._34 >> tempKeyframeDB.mGlobalTransform._44;


			//tempKeyframeDB.mGlobalTransform._41 = tempKeyframeDB.mGlobalTransform._41 * m_Scale;
			//tempKeyframeDB.mGlobalTransform._42 = tempKeyframeDB.mGlobalTransform._42 * m_Scale;
			//tempKeyframeDB.mGlobalTransform._43 = tempKeyframeDB.mGlobalTransform._43 * m_Scale;

			//tempKeyframeDB.mGlobalTransform._14 = tempKeyframeDB.mGlobalTransform._14 * m_Scale;
			//tempKeyframeDB.mGlobalTransform._24 = tempKeyframeDB.mGlobalTransform._24 * m_Scale;
			//tempKeyframeDB.mGlobalTransform._34 = tempKeyframeDB.mGlobalTransform._34 * m_Scale;

			tempKeyframeDB.mGlobalTransform.SetT(
				FbxVector4(tempKeyframeDB.mGlobalTransform.GetT().mData[0] * m_Scale,
					tempKeyframeDB.mGlobalTransform.GetT().mData[1] * m_Scale,
					tempKeyframeDB.mGlobalTransform.GetT().mData[2] * m_Scale));

			tempAnimData.mKeyframeData.push_back(tempKeyframeDB);
		}

		//for (auto &b : m_BoneKeyframeList)
		//{
		//	if (b.mBoneName == tempAnimData.mBoneName)
		//	{
		//		b.mGlobalBindposeInverse = tempAnimData.mGlobalBindposeInverse;
		//		b.mKeyframeData = tempAnimData.mKeyframeData;
		//	}
		//}

		m_BoneKeyframeList.push_back(tempAnimData);
		//m_BoneKeyframeList[i].mGlobalBindposeInverse = tempAnimData.mGlobalBindposeInverse;
		//m_BoneKeyframeList[i].mKeyframeData = tempAnimData.mKeyframeData;

		//GetBoneKeyframeData(tempAnimData.mBoneName).mGlobalBindposeInverse = tempAnimData.mGlobalBindposeInverse;
		//GetBoneKeyframeData(tempAnimData.mBoneName).mKeyframeData = tempAnimData.mKeyframeData;
		//m_BoneKeyframeList.push_back(tempAnimData);
		tempAnimData.mKeyframeData.clear();
	}
	m_StartKeyframe = m_BoneKeyframeList.begin()->mKeyframeData.begin()->mKeyframeIndex;
	m_EndKeyframe = m_BoneKeyframeList.begin()->mKeyframeData.back().mKeyframeIndex;


	//°¡ÁßÄ¡
	m_AnimFile >> dataComment;

	std::string nonstring; // /

	for (auto i = 0; i < m_WeightVertexCount; ++i)
	{
		UINT boneVertexIndex;
		std::string tempBone[4];
		XMFLOAT4	tempweights;

		m_AnimFile >> boneVertexIndex
			>> tempBone[0] >> tempBone[1] >> tempBone[2] >> tempBone[3] >> nonstring
			>> tempweights.x >> tempweights.y >> tempweights.z >> tempweights.w;

		//Warp3dBlendingInfo tempBlendingInfo(tempBone[0], tempBone[1], tempBone[2], tempBone[3], tempweights);

		m_blendingInfoList[boneVertexIndex].mInfo
			.push_back(std::make_pair(tempBone[0], tempweights.x));

		m_blendingInfoList[boneVertexIndex].mInfo
			.push_back(std::make_pair(tempBone[1], tempweights.y));

		m_blendingInfoList[boneVertexIndex].mInfo
			.push_back(std::make_pair(tempBone[2], tempweights.z));

		m_blendingInfoList[boneVertexIndex].mInfo
			.push_back(std::make_pair(tempBone[3], tempweights.w));
	}
	m_AnimFile.close();


}

CWarpAnimation::CWarpAnimation(CWarpAnimation* anim)
{
	m_KeyframeLength = anim->GetKeyframeLegth();
	m_Scale = anim->GetScale();
	m_BoneCount = anim->GetBoneCount();
	m_WeightVertexCount = anim->GetWeightVertexCount();
	m_BoneKeyframeList = anim->GetBoneKeyframeList();
	m_blendingInfoList = anim->GetBlendingInfoList();

	m_StartKeyframe = anim->GetStartKeyframe();
	m_EndKeyframe = anim->GetEndKeyframe();
}

CWarpAnimation::~CWarpAnimation()
{
}
