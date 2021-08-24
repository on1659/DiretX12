#pragma once

#include "stdafx.h"
#include "ItemBasic.h"
#include "FontManager.h"
#include "Scene.h"
#include "ItemBoom.h"
#include "RotatingScript.h"

class ItemFake : public ItemBasic
{

public:
	ItemFake(std::string name = "") : ItemBasic()
	{
		ItemFake::name = "ItemFake";
		tag = TAG_TYPE::TAG_FAKE_ITEM;
		nItemType = ITEM_TYPE::ITEM_TYPE_FAKE;
	}

	virtual ~ItemFake() {}


	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		ItemBasic::Start(gameObject);
		tag = TAG_TYPE::TAG_FAKE_ITEM;
		nItemType = ITEM_TYPE::ITEM_TYPE_TRAP;
		sUIname = "";
		m_gameObject->nGID = gFakeItemInstancedID++;
	}

	void Animate(float fTimeElapsed)
	{
		ItemBasic::Animate(fTimeElapsed);

		//잠시동안 충돌에서 제외
		if (GetTime() > 1.5f)
			m_gameObject->tag = TAG_TYPE::TAG_EAT_FAKE_ITEM;
	}


	// 충돌할떄 여기가 불려짐 
	//아이템이 먹어서 넘어갈떄 트랩을 설치하고 넘어가야한다.

	bool BeforeMove()
	{
		INPUT->StartVibrate();
		RemoveComponnet();
		return true;
	}

};

