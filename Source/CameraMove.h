#pragma once

#include "stdafx.h"

class CameraMove : public Component
{
public:
	CameraMove(std::string name = "")
	{
		CameraMove::name = "CameraMove";
		tag = TAG_TYPE::TAG_NOTHING;
		fSpeed = 1.0f;
	}

	virtual ~CameraMove() {}

	//변수를 선언하시오
private:
	float		fSpeed;

	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		Component::Start(gameObject);
	}

	virtual void Animate(float fTimeElapsed)
	{
		//Move(fTimeElapsed);
	}

	void Move(float fTimeElapsed)
	{
	}
};
