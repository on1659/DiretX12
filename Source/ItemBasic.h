#pragma once
#pragma once

#include "stdafx.h"
#include "Player.h"
#include "PlayerState.h"
#include "UIState.h"
#include "HashTable.h"
/*
sUIname 에 UI Name을 매치해줘야한다.
*/

class ItemBasic : public Component
{
public:
	ItemBasic(std::string name = "ItemBasic")
	{
		ItemBasic::name = name;
		tag = TAG_TYPE::TAG_ITEM;

		cPlayerstate = nullptr;
		fTime = 0.0f;
		nItemType = 0;
	}


	virtual ~ItemBasic() {}

protected:
	PlayerState*			cPlayerstate;
	CPlayer*				pPlayer;
	float					fTime;
	int						nItemType;

	int						nDefaultKeyRange;
	int						nInputKey;
	int						nSlot;

	std::string				sUIname;

private:
	bool					bDie;
	int						nPlayerId;


	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		Component::Start(gameObject);
		nInputKey = YK_END;
		nSlot = -1;
		bDie = false;
		sUIname.clear();
		sUIname = HASHTABLE->GetItemName(nItemType);

	}

	//아이템을 먹는 동시에 Hide
	bool BeforeMove()
	{
		fTime = 0.0f;
		if (nullptr == m_gameObject)return false;
		cPlayerstate = static_cast<PlayerState*>(m_gameObject->GetComponent(0));

		if (cPlayerstate == nullptr)return false;

		nSlot = cPlayerstate->GainItem(nItemType);
		pPlayer = static_cast<CPlayer*>(m_gameObject.get());
		if (nSlot == -1)
		{
			m_gameObject->DestoryComponetWithId(nGID);
			return false;
		}
		nInputKey = nSlot + YK_1;

		CUIShader* pShader = UIMGR->UseItemUiShader(nSlot, sUIname);

		if (nullptr != pShader)
		{
			pShader->GetUIStateComponet()->EatItem();
			pShader->GetUIStateComponet()->SetUseItenNumber((nInputKey - YK_1));
		}

		return true;
	}

	void Animate(float fTimeElasped)
	{
		fTime += fTimeElasped;
	}

	void RemoveComponnet()
	{
		m_gameObject->DestoryItemComponet(nGID, nSlot);
	}

	virtual void Destory()
	{
		Component::Destory();
		cPlayerstate->UseItem(nSlot);

		UIMGR->EndUseItemUiShader(nSlot, sUIname);
		cPlayerstate = nullptr;
	}

	//Time
	float GetTime() const { return fTime; }

	//Slot
	int GetSlot() const { return nSlot; }

	void SetId(int id) { nPlayerId = id; }
	int GetId() const { return nPlayerId; }

};
