#pragma once

#include "SingleTon.h"
#include "MyStruct.h"
#include <string>
#include <vector>

class CHashTable : public CSingleTonBase<CHashTable>
{
private:
	std::vector<std::string> vItemType;
	std::vector<std::string> vPlayerUI;
	
	std::vector<std::string> vFont;
	

public:
	CHashTable()
	{
		Load();
		ItemLoad();
		vPlayerUI.resize(4);
	}
		
	~CHashTable()
	{

	}

	void Load()
	{
		sStunItem	= "StunItem";
		sFrozenItem = "FrozenItem";
	}

	void ItemLoad()
	{
		vItemType.resize((int)ITEM_TYPE::ITEM_TYPE_END);
		vItemType[0]						= "Nothing";
		vItemType[(int)ITEM_TYPE_SPEED_UP]	= "Bugger";
		vItemType[(int)ITEM_TYPE_HIDE]		= "Doughunt";
		vItemType[(int)ITEM_TYPE_THROW]		= "Apple";
	}

	void PlayerLoad(std::string path, int n = -1)
	{
		if (n == -1)
			vPlayerUI.push_back(path);
		else
			vPlayerUI[n] = path;
	}

	std::string GetPlayerPath(int n) const { return vPlayerUI[n]; }
	std::vector<std::string> GetPlayerPaths() const { return vPlayerUI; }
	std::string GetItemName(int n) const { return vItemType[n]; }


public:
	std::string sStunItem;
	std::string sFrozenItem;

public:
	  std::string STUN_ITEM() const { return sStunItem; }
	std::string FROZEN_ITEM() const { return sFrozenItem; }
};

