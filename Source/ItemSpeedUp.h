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

	//������ �����Ͻÿ�
private:
	
	//�Լ��� �����Ͻÿ�
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
		�̰� �÷��̾���� ����˴ϴ�.
		���� ù������ �׻� PlayerComponet�� ����֤����ϴ�.
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
