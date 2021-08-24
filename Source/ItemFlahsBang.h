#pragma once

#include "stdafx.h"
#include "ItemBasic.h"
#include "Timer.h"
#include "Player.h"
#include "Scene.h"

class ItemFlahsBang : public ItemBasic
{
public:
	ItemFlahsBang(std::string name = "")
	{
		ItemFlahsBang::name = "ItemFlahsBang";
		tag = TAG_TYPE::TAG_ITEM;
	}

	virtual ~ItemFlahsBang() {}
	                                             
	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		ItemBasic::Start(gameObject);
		nItemType = ITEM_TYPE::ITEM_TYPE_THROW;
		sUIname = "Doughunt";
	}
	
	virtual void OnPlayerUpdate(float fTimeElapsed)
	{
		#ifdef DEBUG_MODE_UI
			FONTMGR->MessagePush("기본", 250, 150 + nSlot * 15, 15, YT_Color(255, 0, 0), L"(nSlot %d), (InputKey : %d) (nGID : %d)", nSlot, nInputKey, nGID);
		#endif

		if (INPUT->OnlyKeyDown((YT_KEY)nInputKey))
		{
			//::GBulrChange(YT_BlurMode::BLUR_FLASH);
			#ifndef _CONNECT_SERVER_
				CScene::GetInstance()->AddFlashItem(pPlayer->GetPositionXMFLOAT3(), pPlayer->GetLook(), cPlayerstate->GetID());
			#endif
			if (cPlayerstate)cPlayerstate->UseThrowItem();
			ItemBasic::RemoveComponnet();
		}
	}

};
