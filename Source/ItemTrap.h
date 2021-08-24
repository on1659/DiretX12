#pragma once

#include "stdafx.h"
#include "ItemBasic.h"
#include "FontManager.h"
#include "Scene.h"
#include "ItemFake.h"

class ItemTrap : public ItemBasic
{

public:
	ItemTrap(std::string name = "") : ItemBasic()
	{
		ItemTrap::name = "ItemTrap";
		tag = TAG_TYPE::TAG_FAKE_ITEM;
		nItemType = ITEM_TYPE::ITEM_TYPE_TRAP;
	}

	virtual ~ItemTrap() {}

	//������ �����Ͻÿ�
private:

	//�Լ��� �����Ͻÿ�
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		ItemBasic::Start(gameObject);
		tag = TAG_TYPE::TAG_ITEM;
		nItemType = ITEM_TYPE::ITEM_TYPE_TRAP;
		sUIname = "Cherry";
	}

	virtual void OnPlayerUpdate(float fTimeElapsed)
	{

		#ifdef DEBUG_MODE_UI
			FONTMGR->MessagePush("�⺻", 250, 150 + nSlot * 15, 15, YT_Color(255, 0, 0), L"(nSlot %d), (InputKey : %d) (nGID : %d)", nSlot, nInputKey, nGID);
		#endif
		/*
			nInputKey�� �ش��ϴ� Ű�� ������(1~4) �÷��̾� ��ġ�� Fake Item�� ��ġ�Ѵ�.
		*/
		if (INPUT->OnlyKeyDown((YT_KEY)nInputKey))
		{
			#ifndef _CONNECT_SERVER_
				CScene::GetInstance()->AddFakeInstall(m_gameObject->GetPositionXMFLOAT3());
			#endif
			ItemBasic::RemoveComponnet();
		}
	}

};

