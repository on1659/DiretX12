#pragma once
//RSH 2016.04.21
/*
플레이어 캐릭터 AJ의 애니메이션을 제어하는 컴포넌트 스크립트입니다.
*/

#include "stdafx.h"
#include "AnimationComponent.h"
#include "BulletComponent.h"
#include "MonsterState.h"
#include "Timer.h"

class CMonsterAnimationComponent : public CAnimationComponent
{

public:

	CMonsterAnimationComponent(std::string _name = "")
	{
		name = _name;
		tag = TAG_TYPE::TAG_NOTHING;
		cMonsterState = nullptr;
	}

	virtual ~CMonsterAnimationComponent()
	{
	}

	//변수를 선언하시오
private:
	MonsterState*					cMonsterState;

	//함수를 정의하시오
public:
	void ProcessAnimationClips()
	{
		AnimationClip anim_idle;
		AnimationClip anim_run;
		AnimationClip anim_falldown;

		anim_idle = { AnimState::ANI_IDLE, 55, 340, true };
		anim_run = { AnimState::ANI_RUN, 0, 38, true };
		anim_falldown = { AnimState::ANI_FALLDOWN, 355, 440, false };

		m_AnimationClips.push_back(anim_idle);
		m_AnimationClips.push_back(anim_run);
		m_AnimationClips.push_back(anim_falldown);

		m_currentAnimation = m_AnimationClips[AnimState::ANI_RUN];

		//UINT randomState = rand() % 2;
		//
		//switch (randomState)
		//{
		//case 0:
		//	m_currentAnimation = m_AnimationClips[AnimState::ANI_IDLE];
		//	break;
		//case 1:
		//	m_currentAnimation = m_AnimationClips[AnimState::ANI_RUN];
		//	break;
		//}
	}

	virtual void Start(std::shared_ptr<CGameObject> gameobject)
	{
		Component::Start(gameobject);
		ProcessAnimationClips();

		m_iCurrentKeyframe = m_currentAnimation.m_startKeyframe +(rand()%30);
		m_fTimeElapsedStack = 0.0;

		cMonsterState = static_cast<MonsterState*>(m_gameObject.get()->GetComponent(0));
	}

	virtual void Animate(float fTimeElapsed)
	{
		if (false == cMonsterState->IsAlive())
			SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_FALLDOWN]);

		if (m_iCurrentKeyframe >= m_currentAnimation.m_endKeyframe - 1)
		{
			if (true == m_currentAnimation.m_isLoop)
			{
				m_iCurrentKeyframe = m_currentAnimation.m_startKeyframe;
			}
			else
			{
				//반복하는 애니메이션이 아닐 경우...
				//이에 해당하는 애니메이션: 때리기, 투척, 죽음, 아이템 사용

				if (m_currentAnimation.m_state == AnimState::ANI_FALLDOWN)
				{
					//죽었을 경우에 해당. 임시로 IDLE상태로 전환되도록 한다.
					SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_IDLE]);
				}
				else
				{
					SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_IDLE]);
				}
			}
		}
		else
		{
			m_fTimeElapsedStack += 30.0f *fTimeElapsed * (m_gameObject->GetAnimationSpeed());
			if (1.0f <= m_fTimeElapsedStack)
			{
				m_iCurrentKeyframe++;
				m_fTimeElapsedStack = 0.0f;
			}
			//m_currentKeyframe += 30.0f*fTimeElapsed*(m_gameObject->GetAnimationSpeed());
		}

		if (m_currentAnimation.m_state == AnimState::ANI_FALLDOWN)
		{
			m_gameObject->SetCurrentKeyframe(m_iCurrentKeyframe);
			return;
		}

		m_gameObject->SetCurrentKeyframe(m_iCurrentKeyframe);
	}
};

