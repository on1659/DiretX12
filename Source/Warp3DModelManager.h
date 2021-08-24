#pragma once
#include "stdafx.h"
#include "Warp3DModel.h"
#include "WARP3DMesh.h"
#include "WarpAnimation.h"

class CWarp3DModelManager : public CSingleTonBase<CWarp3DModelManager>
{
public:
	CWarp3DModelManager();
	~CWarp3DModelManager();

private:

	std::map<std::string, CWarp3DModel>		m_mModelData;
	unsigned int							m_nModelData;

	std::map<std::string, CWarpAnimation>	m_mAnimationData;
	unsigned int							m_nAnimationData;
	bool									m_bLoad;

public:

	void Load();
	void InitModelData();
	void InitAnimationData();
	void ReleseInstance();

	void AddModelData(std::string filepath, float scale = 1.0f);
	void AddAnimationData(std::string animfilepath, float scale = 1.0f);

	bool SearchModelData(const std::string& fileName);
	bool SearchAnimationData(const std::string& fileName);

	CWarp3DModel GetModelData(const std::string& fileName);
	CWarpAnimation GetAnimationData(const std::string& fileName);
};

