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

	//������ �����Ͻÿ�
private:


	//�Լ��� �����Ͻÿ�
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
		�浹üũ ����.
		���� �����ε� ���� �ʾƵ� �������ϴ�.
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
		Componet�� �Ѱ��ֱ� ������ ����˴ϴ�.
	*/
	virtual bool BeforeMove()
	{

	}
	//KYT '16.05.05
	/*
	OnPlayerUpdate Player���� �پ��ִ� Componet�鸸 ������ �˴ϴ�.
	*/
	virtual void OnPlayerUpdate(float fTimeElapsed)
	{

	}

	
};
