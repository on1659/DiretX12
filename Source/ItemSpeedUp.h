#pragma once

#include "stdafx.h"
#include "ItemBasic.h"
#include "Timer.h"

class ItemSpeedUp : public ItemBasic
{

public:
	ItemSpeedUp(std::string name = "ItemSpeedUp") : ItemBasic()
	{
		ItemSpeedUp::name = name;
		tag = TAG_TYPE::TAG_ITEM;
		nItemType = ITEM_TYPE::ITEM_TYPE_SPEED_UP;
	
	}

	virtual ~ItemSpeedUp() { }

	//변수를 선언하시오
private:
	
	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		ItemBasic::Start(gameObject);
	}

	bool BeforeMove()
	{
		if (false == ItemBasic::BeforeMove())
			return false;
		cPlayerstate->SetSpeed(cPlayerstate->GetSpeed()* 2);
		return true;
	}

	/*
		이건 플레이어에서만 실행됩니다.
		따라서 첫번쨰는 항상 PlayerComponet가 들어있ㅇ습니다.
	*/
	virtual void OnPlayerUpdate(float fTimeElapsed)
	{ 
		if (fTime > 5.0f)
		{
			cPlayerstate->SetSpeed(cPlayerstate->GetSpeed() * 0.5f);
			ItemBasic::RemoveComponnet();
		}

	}

};
