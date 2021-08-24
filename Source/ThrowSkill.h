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

	//�Լ��� �����Ͻÿ�
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
				//���������� �̰� Ʈ�������������
				cPlayerState->UseSKill(nSkillOrder);
				Use();//������ ����ߴٰ� �����

			}

			#else

				if (INPUT->OnlyKeyDown(YT_KEY::YK_RBUTTON))
				{
					//���������� �̰� Ʈ�������������
					cPlayerState->UseSKill(nSkillOrder);

					CScene::GetInstance()->AddThrowSkill(pPlayer->GetPositionXMFLOAT3(), pPlayer->GetLook(), cPlayerState->GetID());
					Use();//������ ����ߴٰ� �����
				}
		#endif
		}
	}
};
