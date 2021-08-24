#include "stdafx.h"
#include "Warp3DModelManager.h"

CWarp3DModelManager::CWarp3DModelManager() 
{
	m_bLoad = false;
}


CWarp3DModelManager::~CWarp3DModelManager()
{
	m_mModelData.clear();
	m_mAnimationData.clear();
}

void CWarp3DModelManager::Load()
{
	m_nModelData = 0;
	m_nAnimationData = 0;

	if (false == m_bLoad)
	{
		InitModelData();
		InitAnimationData();
	}
	m_bLoad = true;
}

void CWarp3DModelManager::InitModelData()
{
	//Map
	
	//Map Collision
	AddModelData("../Assets/01_MapData/Map/01_Cupcake02_1x1.warp3d");
	AddModelData("../Assets/01_MapData/Map/02_Rubyringcandy_1x1.warp3d");
	AddModelData("../Assets/01_MapData/Map/03_Candymachine_1x1.warp3d");
	AddModelData("../Assets/01_MapData/Map/04_Chocostrawberry_3x3.warp3d");
	AddModelData("../Assets/01_MapData/Map/05_Cupcake01_3x3.warp3d");
	AddModelData("../Assets/01_MapData/Map/06_Rollcake02_3x3.warp3d");
	AddModelData("../Assets/01_MapData/Map/08_CookieHouse_1x1.warp3d");
	AddModelData("../Assets/01_MapData/Map/09_ColaBottle_1x1.warp3d");

	AddModelData("../Assets/01_MapData/Map/Star.warp3d", 0.3f);
	AddModelData("../Assets/01_MapData/Map/Bush01.warp3d");
	AddModelData("../Assets/01_MapData/Map/CloudB.warp3d");
	AddModelData("../Assets/01_MapData/Map/CloudA.warp3d");

	//Item
	AddModelData("../Assets/02_ModelData/05_Item/item_Apple.warp3d",0.7f);
	AddModelData("../Assets/02_ModelData/05_Item/item_BugerJelly.warp3d", 0.7f);
	AddModelData("../Assets/02_ModelData/05_Item/item_Doughnut.warp3d", 0.7f);
	AddModelData("../Assets/02_ModelData/05_Item/item_Cherry.warp3d",0.4f);
	AddModelData("../Assets/02_ModelData/05_Item/item_Trap.warp3d");
	AddModelData("../Assets/02_ModelData/05_Item/item_WartermelonBar.warp3d");

	//Decorate
	AddModelData("../Assets/01_MapData/Decorate/Decorate01_Dasisies.warp3d");
	AddModelData("../Assets/01_MapData/Decorate/Decorate02_OrangePoppies.warp3d");
	AddModelData("../Assets/01_MapData/Decorate/Decorate03_PurpleTulips.warp3d");
	AddModelData("../Assets/01_MapData/Decorate/Decorate04_Orchids.warp3d");

	//Bunny
	AddModelData("../Assets/02_ModelData/03_Monster/Zombunny.warp3d");

	//Player
	//AddModelData("../Assets/02_ModelData/06_Character_Query01/QueryChan01.warp3d");
	AddModelData("../Assets/02_ModelData/06_Character_Query01/QueryChan01_SmallHead.warp3d");
	//
}

void CWarp3DModelManager::InitAnimationData()
{
	//Bunny
	AddAnimationData("../Assets/02_ModelData/03_Monster/Zombunny.warpanim");

	//Player
	AddAnimationData("../Assets/02_ModelData/06_Character_Query01/QueryChan01[idle].warpanim");
	AddAnimationData("../Assets/02_ModelData/06_Character_Query01/QueryChan01[running_inPlace].warpanim");
	AddAnimationData("../Assets/02_ModelData/06_Character_Query01/QueryChan01[running_backward_inPlace].warpanim");
	AddAnimationData("../Assets/02_ModelData/06_Character_Query01/QueryChan01[left_strafe_inPlace].warpanim");
	AddAnimationData("../Assets/02_ModelData/06_Character_Query01/QueryChan01[right_strafe_inPlace].warpanim");
	AddAnimationData("../Assets/02_ModelData/06_Character_Query01/QueryChan01[jog_forwardLeft_diagonal_inPlace].warpanim");
	AddAnimationData("../Assets/02_ModelData/06_Character_Query01/QueryChan01[jog_forwardRight_diagonal_inPlace].warpanim");
	AddAnimationData("../Assets/02_ModelData/06_Character_Query01/QueryChan01[smash].warpanim");
	AddAnimationData("../Assets/02_ModelData/06_Character_Query01/QueryChan01[falldown_front].warpanim");
	AddAnimationData("../Assets/02_ModelData/06_Character_Query01/QueryChan01[falling_flat_impact].warpanim");
}

void CWarp3DModelManager::ReleseInstance()
{
	m_mModelData.clear();
	m_mAnimationData.clear();
	m_bLoad = false;
}

void CWarp3DModelManager::AddModelData(std::string filepath, float scale)
{
	CWarp3DModel model(filepath, scale);

	m_mModelData.insert(std::pair<std::string, CWarp3DModel*>(filepath, &model));
	m_nModelData++;
}

void CWarp3DModelManager::AddAnimationData(std::string animfilepath, float scale)
{
	CWarpAnimation anim(animfilepath, scale);

	m_mAnimationData.insert(std::pair<std::string, CWarpAnimation>(animfilepath, anim));
	m_nAnimationData++;
}

bool CWarp3DModelManager::SearchModelData(const std::string& fileName)
{
	if (m_mModelData.find(fileName) == m_mModelData.end())
	{
		//std::cout << fileName << "을 찾지 못하였습니다" << std::endl;
		GMessageBox("ModelInfo search fail!", fileName);
		return false;
	}

	return true;
}

bool CWarp3DModelManager::SearchAnimationData(const std::string& fileName)
{
	if (m_mAnimationData.find(fileName) == m_mAnimationData.end())
	{
		//std::cout << fileName << "을 찾지 못하였습니다" << std::endl;
		//GMessageBox("AnimationInfo search fail!", fileName);
		return false;
	}

	return true;
}

CWarp3DModel CWarp3DModelManager::GetModelData(const std::string& fileName)
{
	if (!SearchModelData(fileName))
	{
		//std::cout << fileName << "을 찾지 못하였습니다" << std::endl;
		//GMessageBox("ModelInfo search fail!", fileName);
		exit(0);
	}
	else
	{
		auto finder = m_mModelData.find(fileName);
		return finder->second;
	}
}

CWarpAnimation CWarp3DModelManager::GetAnimationData(const std::string& fileName)
{

	if (!SearchAnimationData(fileName))
	{
		//std::cout << fileName << "을 찾지 못하였습니다" << std::endl;
		GMessageBox("AnimationInfo search fail!", fileName);
		exit(0);
	}
	else
	{
		auto finder = m_mAnimationData.find(fileName);
		return finder->second;
	}

}