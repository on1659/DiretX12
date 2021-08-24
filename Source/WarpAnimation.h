#pragma once
#include "stdafx.h"

struct Keyframedata
{
	unsigned int mKeyframeIndex;
	//XMFLOAT4X4 mGlobalTransform;
	FbxAMatrix mGlobalTransform;
};

struct BoneKeyframedata
{
	std::string		mBoneName;
	UINT			mListIndex;
	unsigned int	mIndex;
	//XMFLOAT4X4		mGlobalBindposeInverse;
	FbxAMatrix		mGlobalBindposeInverse;
	std::vector<Keyframedata> mKeyframeData;

	BoneKeyframedata()
	{
		mBoneName = "";
		mListIndex = 0;
		mIndex = 0;
		mGlobalBindposeInverse.SetIdentity();
		//XMStoreFloat4x4(&mGlobalBindposeInverse, XMMatrixIdentity());
		mKeyframeData.clear();
	}

	//imsi
	Keyframedata GetKeyframe(int keyNum)
	{
		if (keyNum < 0)
		{
			if (keyNum > mKeyframeData.size())
				return mKeyframeData[0];
		}
		return mKeyframeData[keyNum];
	}
};

struct Warp3dBone
{
	unsigned int	mIndex;
	std::string		mName;
};

struct Warp3dBlendingInfo
{
	//FBX_DATATYPE3 	   m_position;	//위치값

	//BlendingIndexWeightPair
	//XMUINT4		mIndices;
	std::string mBone[4];
	XMFLOAT4	mWeights;

	Warp3dBlendingInfo(std::string b1, std::string b2, std::string b3, std::string b4, XMFLOAT4 w)
	{
		//mIndices = i;
		mBone[0] = b1;
		mBone[1] = b2;
		mBone[2] = b3;
		mBone[3] = b4;
		mWeights = w;
	}
};

struct Warp3DBlendingIndexWeightPair
{
	//FBX_DATATYPE3 	   m_position;	//위치값

	//BlendingIndexWeightPair
	std::vector<std::pair<std::string, float>>			mInfo;

	Warp3DBlendingIndexWeightPair()
	{
		mInfo.reserve(4);
	}
};

class CWarpAnimation
{
public:
	//CWarpAnimation(const std::string& fileName, float scale = 1.0f);
	CWarpAnimation(const std::string& animfileName, float scale = 1.0f);
	CWarpAnimation(CWarpAnimation*);
	~CWarpAnimation();

	unsigned int GetKeyframeLegth() const { return m_KeyframeLength; }
	unsigned int GetBoneCount() const { return m_BoneCount; }
	float		 GetScale() const { return m_Scale; }

	size_t GetkeyframeDataCount() const { return m_BoneKeyframeList.size(); }
	BoneKeyframedata GetBoneKeyframeData(std::string boneName)
	{
		for (auto &d : m_BoneKeyframeList)
		{
			if (boneName == d.mBoneName)
				return d;
		}
		GMessageBox("BoneSearch Error", "No keyframedata");
	}

	////Warp3dBone GetBone(int index) { return m_BoneList[index]; }
	UINT GetBoneIndex(std::string boneName)
	{
		std::string null;
		null = "null";
		if (null == boneName.c_str())
		{
			return 0;
		}

		for (auto &d : m_BoneKeyframeList)
		{
			if (boneName == d.mBoneName)
				return d.mIndex;
		}
	}

	UINT GetBoneListIndex(std::string boneName)
	{
		for (auto &d : m_BoneKeyframeList)
		{
			if (boneName == d.mBoneName)
				return d.mListIndex;
		}
		return 0;	//'null'또는 없는 명칭일 경우
	}

	//Warp3dBlendingInfo GetBlendingIndexWeightPair(int index) { return m_blendingInfo[index]; }
	//Warp3dBone GetBone(int index) { return m_BoneList[index]; }
	std::vector<std::pair<std::string, float>> GetBlendingIndexWeightPair(int index) { return m_blendingInfoList[index].mInfo; }
	std::unordered_map<UINT, Warp3DBlendingIndexWeightPair> GetBlendingInfoList() { return m_blendingInfoList; }
	//Warp3dBlendingInfo GetBlendingInfo(int index) { return m_BlendingInfoList[index]; }
	//UINT GetBlendingInfoSize() { return m_BlendingInfoList.size(); }
	//std::vector<Warp3dBlendingInfo> GetBlendingInfoList() { return m_BlendingInfoList; }
	std::vector<BoneKeyframedata> GetBoneKeyframeList() { return m_BoneKeyframeList; }
	BoneKeyframedata GetBoneKeyframedata(int index) { return m_BoneKeyframeList[index]; }
	//XMFLOAT4X4 GetSkeletonKeyframeGlobalTransform(unsigned int boneIndex, int keyframeNum)
	//{
	//	return m_BoneKeyframeList[boneIndex].GetKeyframe(keyframeNum).mGlobalTransform;
	//}

	FbxAMatrix GetSkeletonKeyframeGlobalTransform(unsigned int boneIndex, int keyframeNum)
	{
		return m_BoneKeyframeList[boneIndex].GetKeyframe(keyframeNum).mGlobalTransform;
	}

	unsigned int GetWeightVertexCount() { return m_WeightVertexCount; }

	unsigned int GetStartKeyframe() const { return (m_StartKeyframe); }
	unsigned int GetEndKeyframe() const { return (m_EndKeyframe); }
private:
	float						m_Scale;
	unsigned int				m_KeyframeLength;
	unsigned int				m_StartKeyframe;
	unsigned int				m_EndKeyframe;
	unsigned int				m_BoneCount;
	unsigned int				m_WeightVertexCount;
	std::vector<BoneKeyframedata>	m_BoneKeyframeList;

	//std::vector<Warp3dBone>			m_BoneList;
	//std::vector<Warp3dBlendingInfo> m_BlendingInfoList;

	std::unordered_map<UINT, Warp3DBlendingIndexWeightPair> m_blendingInfoList;
};

