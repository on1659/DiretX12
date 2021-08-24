#pragma once

#include "stdafx.h"
#include "TerrainPlayer.h"
#include "PlayerState.h"


class Skill : public Component
{
public:
	Skill(std::string name = "")
	{
		Skill::name = "Skill";
		tag = TAG_TYPE::TAG_SKILL;
	}

	virtual ~Skill()
	{
		pPlayer = nullptr;
		cPlayerState = nullptr;
	}

	//변수를 선언하시오
protected:

	CTerrainPlayer* pPlayer;
	PlayerState*	cPlayerState;

	float			fCoolDown;
	float			fLastUseTime;
	bool			bUsePossible;
	float			fPosX;
	float			fPosY;

	int				nSkillOrder;

	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		Component::Start(gameObject);
		pPlayer = (CTerrainPlayer*)m_gameObject.get();
		cPlayerState = pPlayer->GetPlayerState();
		fCoolDown = 0.0f;
		fLastUseTime = 0.0f;
		bUsePossible = true;
		nSkillOrder = cPlayerState->GetSkillOrdering();
	}

	virtual void Animate(float fTimeElapsed)
	{
		auto t = static_cast<int>(fCoolDown - fLastUseTime);
		if (bUsePossible)
		{
			t = 0;
			#ifdef DEBUG_MODE_UI
				FONTMGR->MessagePush("기본", fPosX - 3, fPosY + 32, 25, YT_Color(70, 30, 30), L" %d ", t);
			#endif
		}
		else
		{
			t++;
		
			if (false == INPUT->KeyDown(YT_KEY::YK_TAB))
			{
				FONTMGR->MessagePush("기본", fPosX - 5, fPosY + 30, 25, YT_Color(70, 30, 30), L" %d ", t);
			}
		}
		if (!bUsePossible)
		{
			fLastUseTime += fTimeElapsed;
			if (fLastUseTime - fCoolDown > 0.0f) bUsePossible = true;
		}

	}

	void Use() { bUsePossible = false; fLastUseTime = 0.0f; }
	bool IsUse() const { return bUsePossible; }
	void HalfCoolDown() { fLastUseTime += (fCoolDown * 0.5f); }

};

