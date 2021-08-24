#pragma once

#include "stdafx.h"
#include "ItemBasic.h"
#include "Timer.h"
#include "MonsterState.h"


class MonsterHeadBangBang : public Component
{
public:
	MonsterHeadBangBang(std::string name = "")
	{
		MonsterHeadBangBang::name = "MonsterHeadBangBang";
		tag = TAG_TYPE::TAG_NOTHING;
		xmfPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	virtual ~MonsterHeadBangBang() {}

	//변수를 선언하시오
private:
	int				fRadian;
	float			fScale;
	float			fHeight;
	float			fSpeed;
	XMFLOAT3		xmfPosition;

	float			fTime;
	MonsterState*	pMonsterState;

	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		fRadian = 0.0f;
		fScale = 0.2f;
		fHeight = 1.1f;
		fSpeed = 5.0f;
	}

	//KYT '16.05.05
	/*
	OnPlayerUpdate Player에게 붙어있는 Componet들만 실행이 됩니다.
	*/

	virtual void Animate(float fTimeElapsed)
	{
		fTime += fTimeElapsed;

		float x = fScale * static_cast<float>(std::cos(RADAIN(fRadian)));
		float z = fScale * static_cast<float>(std::sin(RADAIN(fRadian)));

		XMFLOAT3 xf3 = xf3 = xmfPosition;

		xf3.y += fHeight;
		xf3.x += x;
		xf3.z += z;

		m_gameObject->SetPosition(xf3);

		fRadian += fSpeed;

		if (fTime > 3.0f)
			m_gameObject->SetActive(false);

	}

	
	void SetPosition(XMFLOAT3 position) { xmfPosition = position; }

	void SetGameObject(std::shared_ptr<CGameObject> object)
	{
		m_gameObject.reset();
		m_gameObject = object;
	}

	void SetMonsterState(MonsterState* state)
	{
		pMonsterState = state;
		pMonsterState->StartHeadBangBang();
	}
	XMFLOAT3& GetMonsterPosition() const { return pMonsterState->GetPositionXMFLOAT3(); }


	virtual void Destory()
	{
		pMonsterState->StopHeadBangBang();
		pMonsterState = nullptr;
		m_gameObject = nullptr;
	}
	
	float GetTime() const { return fTime; }



};
