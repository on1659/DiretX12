#pragma once

#include "stdafx.h"
#include "ItemBasic.h"
#include "FontManager.h"
#include "Skill.h"

class ItemCooldownDecrease : public ItemBasic
{
public:
	ItemCooldownDecrease(std::string name = "") : ItemBasic()
	{
		ItemCooldownDecrease::name = "ItemThrow";
	}

	virtual ~ItemCooldownDecrease() {}

	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		ItemBasic::Start(gameObject);
	}
	
	//KYT '16.05.05
	/*
		Componet를 넘겨주기 직전에 실행됩니다.
	*/
	bool BeforeMove()
	{
		auto vComponent = m_gameObject->GetComponents(TAG_TYPE::TAG_SKILL);

		for (auto &component : vComponent)
			static_cast<Skill*>(component)->HalfCoolDown();

		return true;
	}
	
	void OnPlayerUpdate(float fTimeElapsed)
	{
		ItemBasic::RemoveComponnet();
	}
};

