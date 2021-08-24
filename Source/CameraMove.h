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

	//������ �����Ͻÿ�
private:
	float		fSpeed;

	//�Լ��� �����Ͻÿ�
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
