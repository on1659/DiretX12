#pragma once

//KYT '16.07.19
/*
	EvasionSkill Skill
*/


#include "stdafx.h"
#include "PassiveSkill.h"

class EvasionSkill : public PassiveSkill
{

public:

	EvasionSkill(std::string name = "") : PassiveSkill(name)
	{
		EvasionSkill::name = "SpeedSkill";
	}

	virtual ~EvasionSkill() {}

private:


public:
	void Start(std::shared_ptr<CGameObject> gameObject)
	{
		PassiveSkill::Start(gameObject);
		cPlayerState->SetPassiveSkillEvasion(true);
	}

	void EvasionSuccess()
	{
	}
};

