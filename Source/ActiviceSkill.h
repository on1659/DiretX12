#pragma once

#include "stdafx.h"
#include "Skill.h"

class ActiviceSkill : public Skill
{
public:
	ActiviceSkill(std::string name = "") : Skill(name)
	{
		ActiviceSkill::name = "ActiviceSkill";
	}

	virtual ~ActiviceSkill() {}

	//�Լ��� �����Ͻÿ�
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject) { Skill::Start(gameObject); }
};
