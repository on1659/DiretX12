////////////////////////////////////////////////////////////////////////////////
// Filename: FBXLoader.h
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#define FBX_DATATYPE2 XMFLOAT2
#define FBX_DATATYPE3 XMFLOAT3
#define FBX_DATATYPE4 XMFLOAT4
#define FBX_FPS	  FbxTime::eFrames30
//#define FBX_GLOBAL_MANAGER


//20160409 �ʱ� ������ �Ͽ� ������ �غ�������, �ӵ����� ū ���̸� ������ ���Ͽ���.
#ifdef FBX_GLOBAL_MANAGER
	static FbxManager *g_FBXManager;
	static FbxIOSettings *g_FBXIOsettings;
	static bool bManagerSetting = false;
#endif

struct BlendingIndexWeightPair
{
	//FBX_DATATYPE3 	   m_position;	//��ġ��

	//BlendingIndexWeightPair
	std::vector<std::pair<unsigned int, float>>			mInfo;

	BlendingIndexWeightPair()
	{
		mInfo.reserve(4);
	}
};

struct FBXSDK_MESH_NODE
{
	//20160121 ��� ������ �ʿ伺�� ���� �����Ͽ���
	//���� �ʿ��ұ�?
	std::string		name;			//��� �̸�
	std::string		parentName;		//�θ��� �̸�

	std::vector<FBX_DATATYPE3> m_positions;	//��ġ��

	std::vector<FBX_DATATYPE3> m_normals;	//��ְ�
	std::vector<FBX_DATATYPE2> m_textureCoords;	//UV��

	int *m_indices;		//�ε��� ����Ʈ
	int m_numIndices;	//�ε����� ����
	int m_polygonCount; //������ ����

						//DX11���� ������ �Ҷ����� D3DXMatrix�� ��ȯ�ؾ��� ���̴�.

	void Release()
	{
		m_positions.clear();
		m_normals.clear();
		m_textureCoords.clear();
		m_indices = nullptr;
		m_numIndices = 0;
		m_polygonCount = 0;
	}
};

//struct AnimationLayerData
//{
//	std::string			m_path;
//	FbxImporter			*m_animFBXimporter;
//	FbxScene			*m_animFBXScene;
//	mutable FbxTime		m_animTimeLength;
//	mutable FbxTime		m_animTimeStart;
//	mutable FbxTime		m_animTimeEnd;
//};

struct KeyFrame
{
	FbxLongLong		mFrameNum;
	FbxAMatrix		mGlobalTransform;
};

struct Joint
{
	int mParentIndex;
	std::string mName;

	FbxAMatrix mGlobalBindposeInverse;
	FbxNode* mNode;

	bool	mHasBonedata;

	std::vector<KeyFrame> mKeyFrames;

	Joint() :
		mNode(nullptr)
	{
		mGlobalBindposeInverse.SetIdentity();
		mParentIndex = -1;
		mKeyFrames.clear();
	}

	~Joint()
	{
		mKeyFrames.clear();
	}
};

struct ModelSkeleton
{
	std::vector<Joint> m_joints;
};

class FBXModelLoader
{
public:
	FBXModelLoader(std::string path, float modelScale = 1.0f, bool isVoxel = false);
	FBXModelLoader(FbxScene* scene, float modelScale = 1.0f, bool isVoxel = false);
	~FBXModelLoader();

	void CreateFBXScene(std::string filename);
	void ImportFBX(std::string path);
	virtual void ProcessMeshInfo();
	virtual void ProcessMeshInfo(FbxScene*);
	virtual void TraversalFBXTree_forMesh(FbxNode* Node);

	//FBXSDK_MESH_NODE�� ���� ������ ��´�.
	void LoadVertexInformation(FbxMesh* pMesh, FBXSDK_MESH_NODE*);
	//void LoadIndicesInformation(FbxMesh* pMesh, FBXSDK_MESH_NODE*);
	void LoadNormalInformation(FbxMesh* pMesh, FBXSDK_MESH_NODE*);
	void LoadUVInformation(FbxMesh* pMesh, FBXSDK_MESH_NODE*);

	FBX_DATATYPE3 GetVertices(int index) { return m_pMeshNodeInfo[0].m_positions[index]; }
	FBX_DATATYPE3 GetNormals(int index) { return m_pMeshNodeInfo[0].m_normals[index]; }
	FBX_DATATYPE2 GetTextureCoords(int index) { return m_pMeshNodeInfo[0].m_textureCoords[index]; }
	int* GetIndices(int index = 0) { return m_pMeshNodeInfo[index].m_indices; }
	int GetIndiecesCount(int index = 0) { return m_pMeshNodeInfo[index].m_numIndices; }
	int GetPolygonCount(int index = 0) { return m_pMeshNodeInfo[index].m_polygonCount; }

	FbxScene *GetFBXScene() { return m_scene; }


	//�ε��� ���� �����ΰ�?
	bool IsVoxel() { return m_isVoxel; }

	float GetModelScale() { return m_modelScale; }
	void SetModelScale(float scale) { m_modelScale = scale; }
	

	//RSH 2016.04.23
	/*
		�ٿ�� �ڽ��� ���� x,y,z���� �ִ�&�ּҸ� ���ϴ� �Լ��� �ʿ��ϴ�.
	*/
	float GetHighestX() { return m_highestX; };
	float GetLowestX() { return m_lowestX; };
	float GetHighestY() { return m_highestY; };
	float GetLowestY() { return m_lowestY; };
	float GetHighestZ() { return m_highestZ; };
	float GetLowestZ() { return m_lowestZ; };

protected:
	#ifndef FBX_GLOBAL_MANAGER
		FbxManager *m_manager;
		FbxIOSettings *m_ioSettings;
	#endif

	FbxImporter *m_importer;
	FbxScene *m_scene;

	std::vector<FBXSDK_MESH_NODE> m_pMeshNodeInfo;

	bool m_isVoxel;	//�� Ŭ�������� �����ϴ� FBX���Ͽ� ���� �������ΰ�?

	float m_modelScale;

	std::string m_fileName;


	bool m_getfirstPos;
	float m_highestX;
	float m_lowestX;
	float m_highestY;
	float m_lowestY;
	float m_highestZ;
	float m_lowestZ;
};


class FBXAnimationModelLoader : public FBXModelLoader
{
public:
	FBXAnimationModelLoader(std::string filename, float modelScale = 1.0f, bool isVoxel = false);
	~FBXAnimationModelLoader();

	virtual void ProcessAnimationInfo();
	virtual void TraversalFBXTree_forAnimation(FbxNode* Node);

	void ProcessSkeletonHierarchy(FbxNode* inRootNode);
	void ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex);

	void LoadControlPoints(FbxNode* inNode);
	//void LoadSkeletalInfo(FbxMesh*, FbxAMatrix geometryTransform);
	void LoadSkeletalInfo(FbxNode* inNode);

	int GetAnimStartTime(int keyframeLayer = 0) { return (int)m_animTimeStart.GetFrameCount(FBX_FPS); }
	int GetAnimEndTime(int keyframeLayer = 0) { return (int)m_animTimeEnd.GetFrameCount(FBX_FPS); }
	FbxTime GetAnimTimeLength(int keyframeLayer = 0) { return m_animTimeLength; }

	FbxAMatrix GetGeometryTransformation(FbxNode* inNode);
	unsigned int FindJointIndexUsingName(std::string jointName);

	std::unordered_map<unsigned int, BlendingIndexWeightPair> GetBlendingInfo() { return m_blendingInfo; }
	std::vector<std::pair<unsigned int, float>> GetBlendingIndexWeightPair(int index) { return m_blendingInfo[index].mInfo; }
	ModelSkeleton GetSkeleton() { return m_Skeleton; }
	Joint GetSkeletonJoint(unsigned int boneindex) { return m_Skeleton.m_joints[boneindex]; }
	size_t GetSkeletonJointCount() { return m_Skeleton.m_joints.size(); }
	FbxAMatrix GetSkeletonJointGlobalTransform(unsigned int jointIndex, int keyframeNum);

	bool HasAnitamionKeyframe();

private:

	std::string			m_path;
	mutable FbxTime		m_animTimeLength;
	mutable FbxTime		m_animTimeStart;
	mutable FbxTime		m_animTimeEnd;

	FbxNode												*m_processedNode;
	//FbxAnimLayer										*m_currentAnimLayer;
	FbxArray<FbxString*>								m_animStackNameArray;

	ModelSkeleton										m_Skeleton;


	std::unordered_map<unsigned int, BlendingIndexWeightPair> m_blendingInfo;
};