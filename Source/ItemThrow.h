#pragma once

#include "stdafx.h"
#include "ItemBasic.h"
#include "FontManager.h"
#include "Scene.h"

class ItemThrow : public ItemBasic
{
public:
	ItemThrow(std::string name = "") : ItemBasic()
	{
		ItemThrow::name = "ItemThrow";
		tag = TAG_TYPE::TAG_ITEM;
		nItemType = ITEM_TYPE::ITEM_TYPE_THROW;
	}

	virtual ~ItemThrow() {}

	//변수를 선언하시오
private:
	int						nThrowCount;

	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		ItemBasic::Start(gameObject);
		nThrowCount = 5;
		nItemType = ITEM_TYPE::ITEM_TYPE_THROW;

		sUIname = "Apple";

	}

	void SetThrowCount(int n){ nThrowCount = n; }
	//KYT '16.05.05
	/*
		Componet를 넘겨주기 직전에 실행됩니다.
	*/
	bool BeforeMove()
	{
		if (false == ItemBasic::BeforeMove())
			return false;

		cPlayerstate->GainThrowItem(nThrowCount);
		return true;
	}

	//KYT '16.05.05
	/*
		OnPlayerUpdate Player에게 붙어있는 Componet들만 실행이 됩니다.
	*/
	virtual void OnPlayerUpdate(float fTimeElapsed)
	{

		#ifdef DEBUG_MODE_UI
			FONTMGR->MessagePush("기본", 250, 150 + nSlot * 15, 15, YT_Color(255, 0, 0), L"(nSlot %d), (InputKey : %d) (nGID : %d)", nSlot, nInputKey, nGID);
		#endif


		if (INPUT->OnlyKeyDown((YT_KEY)nInputKey))
		{
			SOUNDMGR->Play("투척", pPlayer->GetPositionXMFLOAT3());

			#ifndef _CONNECT_SERVER_
				CScene::GetInstance()->AddBullet(pPlayer->GetPositionXMFLOAT3(), pPlayer->GetLook(), cPlayerstate->GetID());
			#endif

			if(cPlayerstate)cPlayerstate->UseThrowItem();
			ItemBasic::RemoveComponnet();

		}

	}

};

