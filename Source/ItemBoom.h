#pragma once

#include "stdafx.h"
#include "ItemBasic.h"
#include "FontManager.h"
#include "Scene.h"
#include "PlayerState.h"

class ItemBoom : public ItemBasic
{

public:
	ItemBoom(std::string name = "") : ItemBasic()
	{
		ItemBoom::name = "ItemBoom";
		tag = TAG_TYPE::TAG_ITEM;
		nItemType = ITEM_TYPE::ITEM_TYPE_FAKE;
	}

	virtual ~ItemBoom() {}

private:
	// - �÷��̾ ��Ƶ� ��ġ.
	XMFLOAT3			xf3LockPosition;

	// - �÷��̾��� ��ġ�� ������ �������� �����´�.
	CPlayer*			cPlayer;

	//�Լ��� �����Ͻÿ�
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		ItemBasic::Start(gameObject);
		tag = TAG_TYPE::TAG_ITEM;
		nItemType = ITEM_TYPE::ITEM_TYPE_TRAP;
		sUIname = "";
		SOUNDMGR->Play("��_�ɸ�", gameObject->GetPositionXMFLOAT3());
	}

	virtual void Animate(float fTimeElapsed)
	{
		ItemBasic::Animate(fTimeElapsed);

		if ((0.0f < GetTime() && GetTime() < 0.3f)
			|| (0.6f < GetTime() && GetTime() < 0.9f)
			|| (1.2f < GetTime() && GetTime() < 1.5f)
			|| (1.8f < GetTime() && GetTime() < 2.1f))
		{
			INPUT->StartVibrate();
			FONTMGR->MessagePush("ī��", 480, 120, 80, YT_Color(255, 0, 0), L"���� �ɸ�!");
		}
#ifndef _CONNECT_SERVER_
		if (cPlayer)
			cPlayer->SetPosition(xf3LockPosition);
#endif
		if (1.5f < GetTime() && GetTime() < 1.6f)
			INPUT->StopVibrate();

		if (GetTime() > 2.5f)
		{
			cPlayerstate = nullptr;
			SPACE->StaticDestory(m_gameObject);
			INPUT->StopVibrate();

		}
	}

	void SetLockPosition(const XMFLOAT3& pos) { xf3LockPosition = pos; }
	void SetPlayer(CPlayer* player)
	{
		cPlayer = player; 
		if (cPlayer->name == "Player")
		{
			INPUT->StartVibrate();
		}
		cPlayer->StartHeadBangBang();
		INPUT->Update(0.17f);
	}
};

