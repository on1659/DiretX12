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

	//������ �����Ͻÿ�
private:
	//�Լ��� �����Ͻÿ�
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		ItemBasic::Start(gameObject);
		fTime = 0.0f;
	}

	//�������� �Դ� ���ÿ� Hide
	bool BeforeMove()
	{
		//false = ItemBasic::BeforeMove();
		cPlayerstate->SetVisible(TIMEMGR->GetTimeElapsed());// SetSpeed(state->GetSpeed() * 10);
		return true;
	}

	/*
		�̰� �÷��̾���� ����˴ϴ�.
		���� ù������ �׻� PlayerComponet�� ����֤����ϴ�.
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
