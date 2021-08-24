#pragma once

#include "stdafx.h"
#include "ItemBasic.h"
#include "Timer.h"


class ItemHide : public ItemBasic
{

public:

	ItemHide(std::string name = "ItemHide") : ItemBasic()
	{
		ItemHide::name = name;
		tag = TAG_TYPE::TAG_ITEM;
		nItemType = ITEM_TYPE::ITEM_TYPE_HIDE;
	}

	virtual ~ItemHide() { }

	//변수를 선언하시오
private:
	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		ItemBasic::Start(gameObject);
		fTime = 0.0f;
	}

	//아이템을 먹는 동시에 Hide
	bool BeforeMove()
	{
		//false = ItemBasic::BeforeMove();
		cPlayerstate->SetVisible(TIMEMGR->GetTimeElapsed());// SetSpeed(state->GetSpeed() * 10);
		return true;
	}

	/*
		이건 플레이어에서만 실행됩니다.
		따라서 첫번쨰는 항상 PlayerComponet가 들어있ㅇ습니다.
	*/
	virtual void OnPlayerUpdate(float fTimeElapsed)
	{
		if (fTime > 3.0f)
		{
			tag = TAG_TYPE::TAG_ITEM_USE;
			cPlayerstate->SetVisible(0.0f);
			ItemBasic::RemoveComponnet();
		}

	}

};
