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


	//�Լ��� �����Ͻÿ�
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

		//��õ��� �浹���� ����
		if (GetTime() > 1.5f)
			m_gameObject->tag = TAG_TYPE::TAG_EAT_FAKE_ITEM;
	}


	// �浹�ҋ� ���Ⱑ �ҷ��� 
	//�������� �Ծ �Ѿ�� Ʈ���� ��ġ�ϰ� �Ѿ���Ѵ�.

	bool BeforeMove()
	{
		INPUT->StartVibrate();
		RemoveComponnet();
		return true;
	}

};

