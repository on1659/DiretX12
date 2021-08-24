#pragma once

#include "stdafx.h"
#include "ActiviceSkill.h"
#include "Scene.h"

class ThrowSkill : public ActiviceSkill
{
public:
	ThrowSkill(std::string name = "") : ActiviceSkill(name)
	{
		ThrowSkill::name = "ThrowSkill";
	}

	virtual ~ThrowSkill() {} 

	//함수를 정의하시오
public:

	virtual void Start(std::shared_ptr<CGameObject> gameObject) 
	{ 
		ActiviceSkill::Start(gameObject); 
		fCoolDown = COOLTIME_THROW;
		fPosX = 245.0f;
		fPosY = 490.0f;
	}

	virtual void OnPlayerUpdate(float fTime)
	{
		if (IsUse())
		{
			#ifdef _CONNECT_SERVER_
			if (INPUT->OnlyKeyDown(YT_KEY::YK_G))
			{
				//서버에서는 이걸 트루조건해줘야함
				cPlayerState->UseSKill(nSkillOrder);
				Use();//아이템 사용했다고 사용함

			}

			#else

				if (INPUT->OnlyKeyDown(YT_KEY::YK_RBUTTON))
				{
					//서버에서는 이걸 트루조건해줘야함
					cPlayerState->UseSKill(nSkillOrder);

					CScene::GetInstance()->AddThrowSkill(pPlayer->GetPositionXMFLOAT3(), pPlayer->GetLook(), cPlayerState->GetID());
					Use();//아이템 사용했다고 사용함
				}
		#endif
		}
	}
};
