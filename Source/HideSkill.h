#pragma once

#include "stdafx.h"
#include "ActiviceSkill.h"
#include "Scene.h"

class HideSkill : public ActiviceSkill
{
public:
	HideSkill(std::string name = "") : ActiviceSkill(name)
	{
		HideSkill::name = "HideSkill";
	}

	virtual ~HideSkill() {}

	//�Լ��� �����Ͻÿ�
public:

	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		ActiviceSkill::Start(gameObject);

		#ifdef _CONNECT_SERVER_
			fCoolDown = COOLTIME_HIDE;
		#else
			fCoolDown = 1;
		#endif

		fPosX = 190.0f;
		fPosY = 490.0f;
	}

	virtual void OnPlayerUpdate(float fTimeElapsed)
	{
		if (IsUse())
		{
			if (INPUT->KeyDown(YT_KEY::YK_H))
			{
				cPlayerState->SetVisible(fTimeElapsed);
				auto pos = m_gameObject->GetPositionXMFLOAT3();
				auto look = pPlayer->GetLookVectorXMFLOAT3();


				PARTICLE->StartParticle("���̵�", pos.x, pos.y + 0.5f, pos.z, 5);							// - ���̵� ��ų�� ���� ���ť���
				PARTICLE->StartSprtieAnimation("����Ҳ�", pos.x - look.x, pos.y + 0.5f, pos.z- -look.z, 1);					// - ����Ҳ� ��������Ʈ


				//������ ����ߴٰ� �����
					Use();

				//RSH '16.08.22
				/*
					���� ȿ����.(�ڱ� �ڽ�)
				*/
				SOUNDMGR->Play("����", pos);
			}
		}
		HideSkill::HideMode(fTimeElapsed);
	}


	void HideMode(float fTimeElapsed)
	{
		float fTime = cPlayerState->GetVisible();
		if (fTime > 0.0f)
		{
			cPlayerState->SetVisible(fTimeElapsed);

			if (fTime > PERSIST_HIDE_TIME)
			{
				cPlayerState->SetVisible(0.0f);
				ActiviceSkill::Use();
			}
		}
	}


};


