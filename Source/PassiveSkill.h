#pragma once

#include "stdafx.h"
#include "Skill.h"

class PassiveSkill : public Skill
{
public:
	PassiveSkill(std::string name = "") : Skill(name)
	{
		PassiveSkill::name = "PassiveSkill";
		bSkillActive = false;
	}

	virtual ~PassiveSkill() {}

protected:
	bool bSkillActive;
	

	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject){ Skill::Start(gameObject); }


	bool IsActivce()  const { return bSkillActive; }
	void StartSkill() { bSkillActive = true; }
	void EndSkill() { bSkillActive = true; }
};


 /*
  피자
   ->
   ->
   ->
   -=>
 */