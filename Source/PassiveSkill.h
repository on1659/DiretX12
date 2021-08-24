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
	

	//�Լ��� �����Ͻÿ�
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject){ Skill::Start(gameObject); }


	bool IsActivce()  const { return bSkillActive; }
	void StartSkill() { bSkillActive = true; }
	void EndSkill() { bSkillActive = true; }
};


 /*
  ����
   ->
   ->
   ->
   -=>
 */