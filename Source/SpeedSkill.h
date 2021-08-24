#pragma once

//KYT '16.07.19
/*
	SpeedUp Skill
*/

#include "stdafx.h"
#include "PassiveSkill.h"

class SpeedSkill : public PassiveSkill
{
public:
	SpeedSkill(std::string name = "") : PassiveSkill(name)
	{
		SpeedSkill::name = "SpeedSkill";
	}

	virtual ~SpeedSkill() {}

private:
	float fSpeedUp;
	float fPlayerSpeed;
	bool  bSpeedUpCheck;

public:
	void Start(std::shared_ptr<CGameObject> gameObject)
	{
		PassiveSkill::Start(gameObject); 
		fSpeedUp = 1.2f;
		fPlayerSpeed = cPlayerState->GetSpeed();
		bSpeedUpCheck = false;
		cPlayerState->SetPassiveSkillSpeedUp(true);
	}
};


/*
ÇÇÀÚ
->
->
->
-=>
*/