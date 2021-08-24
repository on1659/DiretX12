#pragma once

#include "stdafx.h"

class Sample : public Component
{
public:
	Sample(std::string name = "") 
	{ 
		Sample::name = "Sample";
		tag = TAG_TYPE::TAG_NOTHING;
	}

	virtual ~Sample(){}

	//변수를 선언하시오
private:


	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		Component::Start(gameObject);
	}

	virtual void Animate(float fTimeElapsed)
	{
	}

	//KYT '16.04.24
	/*
		충돌체크 예시.
		굳이 오버로딩 하지 않아도 괜찮습니다.
	*/
	virtual void Collision(std::shared_ptr<CGameObject> other, ObjectType type)
	{
		switch (type)
		{
		case ObjectType::StaticObject:
			break;

		case ObjectType::DynamicObject:
			other->DynamicDestory(other);
			break;
		}
	}

	//KYT '16.05.05
	/*
		Componet를 넘겨주기 직전에 실행됩니다.
	*/
	virtual bool BeforeMove()
	{

	}
	//KYT '16.05.05
	/*
	OnPlayerUpdate Player에게 붙어있는 Componet들만 실행이 됩니다.
	*/
	virtual void OnPlayerUpdate(float fTimeElapsed)
	{

	}

	
};
