#pragma once
//RSH 2016.04.21
/*
플레이어 캐릭터 AJ의 애니메이션을 제어하는 컴포넌트 스크립트입니다.
*/

#include "stdafx.h"
#include "BulletComponent.h"
#include "PlayerComponet.h"

class OtherPlayerComponent : public PlayerComponet
{

public:

	OtherPlayerComponent(std::string _name = "")
	{
		name = _name;
		tag = TAG_TYPE::TAG_NOTHING;
	}

	virtual ~OtherPlayerComponent()
	{
	}

	//변수를 선언하시오
private:
	int					m_MyRoomPlayerIndex;

	float				fSpeed;


	//함수를 정의하시오
public:

	void Start(std::shared_ptr<CGameObject> gameobject)
	{
		PlayerComponet::Start(gameobject);
		fSpeed = cPlayerState->GetSpeed();

		if (cPlayerState->GetCharcaterName() == "MEI")
			m_fDuringStunTime = MEI_STUN_TIME;
		else
			m_fDuringStunTime = LIZZY_STUN_TIME;

	}

	//KYT '16.06.28
	/*
		Imsi
	*/
	void Animate(float fTimeElapsed)
	{
		#ifndef _CONNECT_SERVER_
			if (cPlayerState->GetID() ==1) OtherPlayerComponent::Move(fTimeElapsed);
		#endif

		#ifdef DEBUG_MODE_UI
			if(m_gameObject->name == "OtherPlayer1")
				FONTMGR->MessagePush(720, 50, 20, YT_Color(0, 0, 0), L"(%1.f, %1.f, %1.f)", GetPositionXMFLOAT3().x, GetPositionXMFLOAT3().y, GetPositionXMFLOAT3().z);	
		#endif

		PlayerComponet::HeadBangBang(fTimeElapsed);
	}

	//해줘야한당
	virtual void OnPlayerUpdate(float fTimeElapsed) {}

	//KYT '16.07.12
	/*
		Imis
	*/
	void Move(float fTimeElapsed)
	{
		DWORD dwDirection = 0;
		if (INPUT->KeyDown(YT_KEY::YK_I))
			dwDirection |= DIR_FORWARD;
		if (INPUT->KeyDown(YT_KEY::YK_K))
			dwDirection |= DIR_BACKWARD;
		if (INPUT->KeyDown(YT_KEY::YK_J))
			dwDirection |= DIR_LEFT;
		if (INPUT->KeyDown(YT_KEY::YK_L))
			dwDirection |= DIR_RIGHT;
		pPlayer->Move(dwDirection, fSpeed * fTimeElapsed, true);
		cPlayerState->SetDirection(dwDirection);
	}

};

