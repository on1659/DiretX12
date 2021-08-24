#pragma once

#include "stdafx.h"

class UpDownPatrol : public Component
{ 
public:
	UpDownPatrol(std::string name = "")
	{
		UpDownPatrol::name = "UpDownPatrol";
		tag = TAG_TYPE::TAG_NOTHING;
	}

	virtual ~UpDownPatrol() {}

	//변수를 선언하시오
private:
	float						fRange;
	float						fSpeed;

	float						fMoveAccure;
	std::shared_ptr<CGameObject> pPumpObject;

	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		Component::Start(gameObject);
		fSpeed = 5.0f; 
		fRange = 2.5f;
	}

	void SetPumpObject(std::shared_ptr<CGameObject> pumpObject)
	{
		pPumpObject = pumpObject;
	}

	virtual void Animate(float fTimeElapsed)
	{
		Trasnlate(fSpeed * fTimeElapsed, 0.0f, 1.0f, 0.0f);
	
		fMoveAccure += fSpeed * fTimeElapsed;
		
		if (abs(fMoveAccure) > fRange)
		{
			fMoveAccure = 0.0f;
			fSpeed *= -1;
		}
	}

};
