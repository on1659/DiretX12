#pragma once

#include "stdafx.h"

class CloudPatrol : public Component
{
public:
	CloudPatrol(std::string name = "")
	{
		CloudPatrol::name = "CloudPatrol";
		tag = TAG_TYPE::TAG_NOTHING;
	}

	virtual ~CloudPatrol() {}

	//변수를 선언하시오
private:
	float				fRange;
	float				fSpeed;

	float				fMoveAccure;
	std::shared_ptr<CGameObject> pPumpObject;

	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		Component::Start(gameObject);
		fSpeed = 2.0f;

		fRange = 100.0f;
	}

	void SetPumpObject(std::shared_ptr<CGameObject> pumpObject)
	{
		pPumpObject = pumpObject;
	}

	void SetSpeed(float speed)
	{
		fSpeed = speed;
	}

	virtual void Animate(float fTimeElapsed)
	{
		Trasnlate(fSpeed * fTimeElapsed, 1.0f, 0.0f, 0.0f);
		//pPumpObject->MoveUp(fTimeElapsed * fSpeed);

		fMoveAccure += fSpeed * fTimeElapsed;

		if (abs(fMoveAccure) > fRange)
		{
			
			fMoveAccure = 0.0f;
			fSpeed *= -1;
		}
	}

};
