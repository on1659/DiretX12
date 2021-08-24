#pragma once
#include "stdafx.h"

struct ModelInfo
{
public:

	XMFLOAT3 GetPosition(unsigned int faceindex) { return m_lVertexPosList[faceindex]; }
	XMFLOAT3 GetNormal(unsigned int faceindex) { return m_lVertexNormalList[faceindex]; }
	XMFLOAT2 GetUV(unsigned int faceindex) { return m_lVertexUVList[faceindex]; }
	UINT GetFaceIndex(unsigned int num) { return m_lFaceIndexList[num]; }
	UINT GetIndiecesCount() { return static_cast<UINT>(m_lFaceIndexList.size()); }

	void SetMaxMinPos()
	{
		m_fMaxX = m_lVertexPosList[0].x;
		m_fMinX = m_lVertexPosList[0].x;
		m_fMaxY = m_lVertexPosList[0].y;
		m_fMinY = m_lVertexPosList[0].y;
		m_fMaxZ = m_lVertexPosList[0].z;
		m_fMinZ = m_lVertexPosList[0].z;

		for (auto &currpos : m_lVertexPosList)
		{
			if (m_fMaxX < currpos.x) { m_fMaxX = currpos.x; }
			if (m_fMinX > currpos.x) { m_fMinX = currpos.x; }
			if (m_fMaxY < currpos.y) { m_fMaxY = currpos.y; }
			if (m_fMinY > currpos.y) { m_fMinY = currpos.y; }
			if (m_fMaxZ < currpos.z) { m_fMaxZ = currpos.z; }
			if (m_fMinZ > currpos.z) { m_fMinZ = currpos.z; }
		}
	}
public:
	std::string					m_sName;

	std::vector<XMFLOAT3>		m_lVertexPosList;
	std::vector<XMFLOAT3>		m_lVertexNormalList;
	std::vector<UINT>			m_lFaceIndexList;
	std::vector<XMFLOAT2>		m_lVertexUVList;

	UINT						m_nVertex;
	UINT						m_nFace;
	UINT						m_nIndices;

	float						m_fMaxX;
	float						m_fMinX;
	float						m_fMaxY;
	float						m_fMinY;
	float						m_fMaxZ;
	float						m_fMinZ;
};


class CWarp3DModel
{
public:
	CWarp3DModel(const std::string& fileName, float scale = 1.0f);
	CWarp3DModel(CWarp3DModel*);
	~CWarp3DModel();

	std::string GetFilePath() const { return m_sFilepath; }
	ModelInfo GetModelInfo(int index = 0) const { return m_ModelList[index]; }
	size_t GetModelCount() const { return m_ModelList.size(); }

private:
	std::string						m_sFilepath;
	std::vector<ModelInfo>			m_ModelList;
};

