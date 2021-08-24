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

	//������ �����Ͻÿ�
private:
	int						nThrowCount;

	//�Լ��� �����Ͻÿ�
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
		Componet�� �Ѱ��ֱ� ������ ����˴ϴ�.
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
		OnPlayerUpdate Player���� �پ��ִ� Componet�鸸 ������ �˴ϴ�.
	*/
	virtual void OnPlayerUpdate(float fTimeElapsed)
	{

		#ifdef DEBUG_MODE_UI
			FONTMGR->MessagePush("�⺻", 250, 150 + nSlot * 15, 15, YT_Color(255, 0, 0), L"(nSlot %d), (InputKey : %d) (nGID : %d)", nSlot, nInputKey, nGID);
		#endif


		if (INPUT->OnlyKeyDown((YT_KEY)nInputKey))
		{
			SOUNDMGR->Play("��ô", pPlayer->GetPositionXMFLOAT3());

			#ifndef _CONNECT_SERVER_
				CScene::GetInstance()->AddBullet(pPlayer->GetPositionXMFLOAT3(), pPlayer->GetLook(), cPlayerstate->GetID());
			#endif

			if(cPlayerstate)cPlayerstate->UseThrowItem();
			ItemBasic::RemoveComponnet();

		}

	}

};

