#pragma once

#include "stdafx.h"
#include "ItemBasic.h"
#include "Timer.h"
#include "Player.h"


class PlayerHeadBangBang : public ItemBasic
{
public:
	PlayerHeadBangBang(std::string name = "")
	{
		PlayerHeadBangBang::name = "HeadBangBang";
		tag = TAG_TYPE::TAG_NOTHING;
	}

	virtual ~PlayerHeadBangBang() {}

	//변수를 선언하시오
private:
	CPlayer*		pPlayer;
	int				fRadian;
	float			fScale;
	float			fHeight;
	float			fSpeed;

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
		float x = fScale * static_cast<float>(std::cos(RADAIN(fRadian)));
		float z = fScale * static_cast<float>(std::sin(RADAIN(fRadian)));

		XMFLOAT3 xf3 = pPlayer->GetPositionXMFLOAT3();
		xf3.y += fHeight;
		xf3.x += x;
		xf3.z += z;

		m_gameObject->SetPosition(xf3);

		fRadian += fSpeed;

		#ifdef _RUNTIME_DEBUGFILE
			if (INPUT->OnlyKeyDown(YK_9))
			{
				XMFLOAT3 x3 = Helper::RuntimeDataModify(XMFLOAT3(), "xf3_HeadBangBang");
				fScale = x3.x;
				fHeight = x3.y;
				fSpeed = x3.z;
			}
		#endif
	}

	void SetPlayer(CPlayer* player) { pPlayer = player; }

	void SetGameObject(std::shared_ptr<CGameObject> object)
	{
		m_gameObject.reset();
		m_gameObject = object;
	}
};
