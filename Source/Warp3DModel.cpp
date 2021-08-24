#include "stdafx.h"
#include "Warp3DModel.h"

CWarp3DModel::CWarp3DModel(const std::string& fileName, float scale)
{
	m_ModelList.clear();
	m_sFilepath = fileName;
	std::ifstream warp3dFile;
	warp3dFile.open(fileName);

	if (warp3dFile.fail())
	{
		std::cout << fileName << "을 읽는 과정에서 오류가 발생하였습니다." << std::endl;
	}

	//std::string line;			
	std::string dataComment;	//#
	std::string dataKeyword;	//:
	std::string nonString;		//

	XMFLOAT2 tempVec2;
	XMFLOAT3 tempVec3;

	ModelInfo modelinfo;

	warp3dFile >> dataComment;
	//오브젝트 개수
	//오브젝트 명칭
	warp3dFile >> dataKeyword >> modelinfo.m_nVertex;	//정점 개수
	warp3dFile >> dataKeyword >> modelinfo.m_nFace;		//면 개수
														//warp3dFile >> dataKeyword >> nonString;				//축(Axis) 정보. 일단 받지 않음
														//warp3dFile >> dataKeyword >> m_hasBone;				//뼈대를 가지고 있는가?
	modelinfo.m_nIndices = modelinfo.m_nFace * 3;

	modelinfo.m_lVertexPosList.reserve(modelinfo.m_nVertex);
	modelinfo.m_lVertexNormalList.reserve(modelinfo.m_nVertex);
	modelinfo.m_lFaceIndexList.reserve(modelinfo.m_nFace * 3);
	modelinfo.m_lVertexUVList.reserve(modelinfo.m_nFace * 3);

	//정점 위치
	warp3dFile >> dataComment;
	for (int i = 0; i < modelinfo.m_nVertex; ++i)
	{
		warp3dFile >> dataKeyword >> tempVec3.x >> tempVec3.y >> tempVec3.z;
		tempVec3.x *= scale;
		tempVec3.y *= scale;
		tempVec3.z *= scale;
		modelinfo.m_lVertexPosList.push_back(tempVec3);
	}

	modelinfo.SetMaxMinPos();

	//정점 노멀
	warp3dFile >> dataComment;
	for (int i = 0; i < modelinfo.m_nFace * 3; ++i)
	{
		warp3dFile >> dataKeyword >> tempVec3.x >> tempVec3.y >> tempVec3.z;
		modelinfo.m_lVertexNormalList.push_back(tempVec3);
	}

	//정점 인덱스
	warp3dFile >> dataComment;
	for (int i = 0; i < modelinfo.m_nFace; ++i)
	{
		warp3dFile >> dataKeyword >> tempVec3.x >> tempVec3.y >> tempVec3.z;
		modelinfo.m_lFaceIndexList.push_back((unsigned int)tempVec3.x);
		modelinfo.m_lFaceIndexList.push_back((unsigned int)tempVec3.y);
		modelinfo.m_lFaceIndexList.push_back((unsigned int)tempVec3.z);
	}

	//정점 UV
	warp3dFile >> dataComment;
	for (int i = 0; i < modelinfo.m_nFace * 3; ++i)
	{
		warp3dFile >> dataKeyword >> tempVec2.x >> tempVec2.y;
		modelinfo.m_lVertexUVList.push_back(tempVec2);
	}

	m_ModelList.push_back(modelinfo);

	//if (true == m_hasBone)
	//{
	//	warp3dFile >> dataComment;
	//	int boneCount;
	//	warp3dFile >> dataKeyword >> boneCount;
	//	//m_BoneList.reserve(boneCount);
	//
	//	warp3dFile >> dataComment;
	//	for (int i = 0; i < boneCount; i++)
	//	{
	//		Warp3dBone tempBone;
	//		float matrix11, matrix12, matrix13;
	//		float matrix21, matrix22, matrix23;
	//		float matrix31, matrix32, matrix33;
	//		float matrix41, matrix42, matrix43;
	//
	//		warp3dFile >> tempBone.mName >> tempBone.mIndex
	//			>> matrix11 >> matrix12 >> matrix13
	//			>> matrix21 >> matrix22 >> matrix23
	//			>> matrix31 >> matrix32 >> matrix33
	//			>> matrix41 >> matrix42 >> matrix43;
	//
	//		tempBone.mGlobalBindposeInverse
	//			= XMMATRIX(
	//				matrix11, matrix12, matrix13, 0,
	//				matrix21, matrix22, matrix23, 0,
	//				matrix31, matrix32, matrix33, 0,
	//				matrix41, matrix42, matrix43, 1);
	//
	//		m_BoneList.push_back(tempBone);
	//	}
	//
	//	//weight값 추출
	//	warp3dFile >> dataComment;
	//
	//	for (int i = 0; i < modelinfo.m_nVertex; ++i)
	//	{
	//		XMUINT4		tempIndices;
	//		XMFLOAT4	tempweights;
	//
	//		warp3dFile >> dataKeyword
	//			>> tempIndices.x >> tempIndices.y >> tempIndices.z >> tempIndices.w >> nonString
	//			>> tempweights.x >> tempweights.y >> tempweights.z >> tempweights.w;
	//
	//		Warp3dBlendingInfo tempBlendingInfo(tempIndices, tempweights);
	//		m_BlendingInfo.push_back(tempBlendingInfo);
	//	}
	//
	//}

	warp3dFile.close();
}

CWarp3DModel::CWarp3DModel(CWarp3DModel *model)
{
	m_sFilepath = model->GetFilePath();
	m_ModelList.clear();

	for (int i = 0; i < model->GetModelCount(); ++i)
	{
		m_ModelList.push_back(model->GetModelInfo(i));
	}
	//m_hasBone = model->HasBone();
	//
	//for (int i = 0; i < model->GetBoneCount(); ++i)
	//{
	//	m_BoneList.push_back(model->GetBone(i));
	//}
	//
	//for (int k = 0; k < model->GetBlendingInfoSize(); ++k)
	//{
	//	m_BlendingInfo.push_back(model->GetBlendingInfo(k));
	//}
}

CWarp3DModel::~CWarp3DModel()
{
}
