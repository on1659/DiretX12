#pragma once
//RSH 2016.04.21
/*
�÷��̾� ĳ���� AJ�� �ִϸ��̼��� �����ϴ� ������Ʈ ��ũ��Ʈ�Դϴ�.
*/

#include "stdafx.h"
#include "Timer.h"
#include "AnimationComponent.h"
#include "PlayerState.h"

class PlayerAnimation : public CAnimationComponent
{
public:
	PlayerAnimation(std::string _name = "JunoPlayerStateComp")
	{
		name = _name;
	}

	virtual ~PlayerAnimation() {}

	bool		isStanding;


	//RSH '16.08.10
	/*
		��¦ ������ �� ��Ÿ�� �ִϸ��̼� �ð� ������ ���� �������
	*/
	float														m_fSmashTime;
	float														m_fSmashCurrentTime;
	bool														m_bSamshStart;

	float														m_fStartDelay;
	float														m_fStartCurrentDelay;

	std::chrono::time_point<std::chrono::steady_clock>			m_clockPoint;

public:
	virtual void Start(std::shared_ptr<CGameObject> gameobject)
	{
		CAnimationComponent::Start(gameobject);
		ProcessAnimationClips();
		//m_currentKeyframe = 0.0f;
		m_iCurrentKeyframe = 0;
		m_fTimeElapsedStack = 0.0;
		isStanding = true;
	}

	virtual void ProcessAnimationClips()
	{
		AnimationClip anim_idle;
		AnimationClip anim_run;
		AnimationClip anim_falldown;
		AnimationClip anim_leftstrafe;
		AnimationClip anim_righttstrafe;
		AnimationClip anim_left_forward_diagonal;
		AnimationClip anim_right_forward_diagonal;
		AnimationClip anim_run_backward;
		AnimationClip anim_smash;

		//RSH '16.08.11
		/*
			�ִϸ��̼� �߰�: ���ӽ���or������ �� �������� �ִϸ��̼� �߰�
		*/
		AnimationClip anim_startfall;

		anim_idle = { AnimState::ANI_IDLE, 0, 59, true };
		anim_run = { AnimState::ANI_RUN, 0, 21, true };
		anim_falldown = { AnimState::ANI_FALLDOWN, 0, 73, false };
		anim_leftstrafe = { AnimState::ANI_LEFT_STRAFE, 0, 20, true };
		anim_righttstrafe = { AnimState::ANI_RIGHT_STRAFE, 0, 20, true };
		anim_left_forward_diagonal = { AnimState::ANI_LEFT_FOWARD_DIAGONAL, 0, 25, true };
		anim_right_forward_diagonal = { AnimState::ANI_RIGHT_FOWARD_DIAGONAL, 0, 24, true };
		anim_run_backward = { AnimState::ANI_RUN_BACKWARD, 0, 17, true };
		anim_smash = { AnimState::ANI_SMASH, 0, 50, false };
		anim_startfall = { AnimState::ANI_STARTFALL, 5, 60, false };

		m_AnimationClips.push_back(anim_idle);
		m_AnimationClips.push_back(anim_run);
		m_AnimationClips.push_back(anim_falldown);
		m_AnimationClips.push_back(anim_leftstrafe);
		m_AnimationClips.push_back(anim_righttstrafe);
		m_AnimationClips.push_back(anim_left_forward_diagonal);
		m_AnimationClips.push_back(anim_right_forward_diagonal);
		m_AnimationClips.push_back(anim_run_backward);
		m_AnimationClips.push_back(anim_smash);
		m_AnimationClips.push_back(anim_startfall);

		SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_IDLE]);
		//SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_STARTFALL]);

		m_bSamshStart = false;

		//RSH '16.08.08
		/*
		������ ����� �����ϴ� �ð�(2��)
		*/
		m_fSmashTime = 2000.0f;

		//RSH '16.08.08
		/*
		���� ����� �����ϴ� �ð�(3��)
		*/
		m_fStartDelay = 3000.0f;
		m_fStartCurrentDelay = 0.0f;
	}

	virtual void Animate(float fTimeElapsed)
	{
		if(m_currentAnimation.m_state != AnimState::ANI_STARTFALL)
			cPlayerState->AnimationInitEnd();

		#ifndef _CONNECT_SERVER_
			if (INPUT->OnlyKeyDown(YK_F6)) 
				::GBulrChange(YT_BlurMode::BLUR_RESPAWN);
		#endif
		#ifdef DEBUG_MODE_UI
			FONTMGR->MessagePush("�⺻", 40, 250, 20, YT_Color(0, 0, 0), L"(%1.f, %1.f, %1.3f %1.3f)", gBulrMode.x, gBulrMode.y, gBulrMode.z, gBulrMode.w);
		#endif

		//RSH '16.08.08
		/*
			���� ��� ���� ���ǹ�.(���� ����or ������)
		*/
		if (m_currentAnimation.m_state == AnimState::ANI_STARTFALL)
		{
			if (m_fStartCurrentDelay == 0.0f)
			{
				m_clockPoint = std::chrono::high_resolution_clock::now();
				m_fStartCurrentDelay = 1.0f;
			}
			else
			{
				auto t = std::chrono::high_resolution_clock::now() - m_clockPoint;
				m_fStartCurrentDelay = (int)std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
			}

			if (m_iCurrentKeyframe >= m_currentAnimation.m_endKeyframe - 1)
			{
				if (m_fStartCurrentDelay >= m_fStartDelay)
				{
					SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_IDLE]);
				}
				else
					m_iCurrentKeyframe = m_currentAnimation.m_endKeyframe - 1;
			}
			else
			{
				m_fTimeElapsedStack += 30.0f * fTimeElapsed*(m_gameObject->GetAnimationSpeed());
				if (1.0f <= m_fTimeElapsedStack)
				{
					m_iCurrentKeyframe++;
					m_fTimeElapsedStack = 0.0f;
				}
			}
			m_gameObject->SetCurrentKeyframe(m_iCurrentKeyframe);
			return;
		}

		if (m_iCurrentKeyframe >= m_currentAnimation.m_endKeyframe - 1)
		{
			if (true == m_currentAnimation.m_isLoop)
			{
				m_iCurrentKeyframe = m_currentAnimation.m_startKeyframe;
			}
			else
			{
				//�ݺ��ϴ� �ִϸ��̼��� �ƴ� ���...
				//�̿� �ش��ϴ� �ִϸ��̼�: ������, ��ô, ����, ������ ���

				if (m_currentAnimation.m_state == AnimState::ANI_SMASH) m_bSamshStart = false;

				switch (m_currentAnimation.m_state)
				{
					case AnimState::ANI_SMASH:
						SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_IDLE]);
						break;
					case AnimState::ANI_FALLDOWN:
						m_iCurrentKeyframe = m_currentAnimation.m_endKeyframe - 1;
						break;
					default:
						break;
				}
			}
		}
		else
		{
			if ((m_currentAnimation.m_state != AnimState::ANI_SMASH))
			{
				m_fTimeElapsedStack += 30.0f * fTimeElapsed*(m_gameObject->GetAnimationSpeed());
				if (1.0f <= m_fTimeElapsedStack)
				{
					m_iCurrentKeyframe++;
					m_fTimeElapsedStack = 0.0f;
				}
			}
			else
			{
				//RSH '16.08.08
				/*
				��¦�������� ��쿡�� �������� ������ Ÿ�̹� ������ �ؾߵǱ� ������
				2�� ���� ��¦������ �ִϸ��̼��� ������ �����Ͽ����Ѵ�.
				*/
				if (!m_bSamshStart)
				{
					m_clockPoint = std::chrono::high_resolution_clock::now();
					m_fSmashCurrentTime = 0.0f;
					m_iCurrentKeyframe = 0;
					m_bSamshStart = true;
				}
				else
				{
					auto t = std::chrono::high_resolution_clock::now() - m_clockPoint;
					m_fSmashCurrentTime += (int)std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
					if (m_fSmashCurrentTime > m_fSmashTime) m_fSmashCurrentTime = m_fSmashTime;
					m_iCurrentKeyframe 
						= (int)((m_fSmashCurrentTime * (m_currentAnimation.m_endKeyframe - m_currentAnimation.m_startKeyframe)) / m_fSmashTime);
					m_clockPoint = std::chrono::high_resolution_clock::now();
				}
			}
		}

		if (false == cPlayerState->IsLife())
		{
			if (m_currentAnimation.m_state != AnimState::ANI_FALLDOWN)
			{
				SmashParticle();
				SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_FALLDOWN]);
			}
		}

		if (m_currentAnimation.m_state == AnimState::ANI_FALLDOWN)
		{
			m_gameObject->SetCurrentKeyframe(m_iCurrentKeyframe);
			return;
		}

		if (false == m_currentAnimation.m_isLoop)
		{
			//int keynum = (int)m_currentKeyframe;
			//if (keynum > m_currentAnimation.m_endKeyframe) keynum = m_currentAnimation.m_endKeyframe - 1;
			m_gameObject->SetCurrentKeyframe(m_iCurrentKeyframe);
			return;
		}

		DWORD dwDirection;
		if(cPlayerState) dwDirection = cPlayerState->GetDirection();

		if (INPUT->OnlyKeyDown(YK_SPACE))
		{
			INPUT->StartVibrate(0, 30000);
			if (m_currentAnimation.m_state != AnimState::ANI_SMASH)
			{
				SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_SMASH]);
				cPlayerState->SetInputPossible(false);
				return;
			}
		}

		if (INPUT->KeyDown(YK_W) || INPUT->KeyDown(YK_UP))
		{
			if (INPUT->KeyDown(YK_A) || INPUT->KeyDown(YK_LEFT))
			{
				if (m_currentAnimation.m_state != AnimState::ANI_LEFT_FOWARD_DIAGONAL)
				{
					SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_LEFT_FOWARD_DIAGONAL]);
					return;
				}
			}

			else if (INPUT->KeyDown(YK_D) || INPUT->KeyDown(YK_RIGHT))
			{
				if (m_currentAnimation.m_state != AnimState::ANI_RIGHT_FOWARD_DIAGONAL)
				{
					SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_RIGHT_FOWARD_DIAGONAL]);
					return;
				}
			}

			else if (m_currentAnimation.m_state != AnimState::ANI_RUN)
			{
				SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_RUN]);
				return;
			}
		}
		else
		{

			if (INPUT->KeyDown(YK_S) || INPUT->KeyDown(YK_DOWN))
			{
				if (m_currentAnimation.m_state != AnimState::ANI_RUN_BACKWARD)
				{
					SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_RUN_BACKWARD]);
					return;
				}
			}
			else
			{
				if (INPUT->KeyDown(YK_A) || INPUT->KeyDown(YK_LEFT))
				{
					if (m_currentAnimation.m_state != AnimState::ANI_LEFT_STRAFE)
					{
						SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_LEFT_STRAFE]);
						return;
					}
				}

				else if (INPUT->KeyDown(YK_D) || INPUT->KeyDown(YK_RIGHT))
				{
					if (m_currentAnimation.m_state != AnimState::ANI_RIGHT_STRAFE)
					{
						SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_RIGHT_STRAFE]);
						return;
					}
				}
			}
		}

		{
			if ((INPUT->KeyUp(YK_W) || INPUT->KeyUp(YK_UP)
				|| INPUT->KeyUp(YK_A) || INPUT->KeyUp(YK_LEFT)
				|| INPUT->KeyUp(YK_D) || INPUT->KeyUp(YK_RIGHT)
				|| INPUT->KeyUp(YK_S) || INPUT->KeyUp(YK_DOWN))
				)
			{
				if (m_currentAnimation.m_state != AnimState::ANI_IDLE)
				{
					SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_IDLE]);
					return;
				}
			}
		}
		
		m_gameObject->SetCurrentKeyframe(m_iCurrentKeyframe);
	}

	void Collision(std::shared_ptr<CGameObject> other, ObjectType type)
	{
		switch (type)
		{
		case ObjectType::PlayerSmashBoundingBox:
			//RSH '16.07.18
			/*
			����÷��̾�� ���� �� Ŭ���̾�Ʈ�� �÷��̾�� �¾Ҵ�.
			�浹�� �Ͼ ��� ����÷��̾��� �̺�Ʈ �߻�
			1. �������� �ִϸ��̼� ������
			2.
			*/
			if ((other->GetCurrentAnimationState() == AnimState::ANI_SMASH)
				&& (other->GetCurrentKeyframe() <= 30) && (other->GetCurrentKeyframe() >= 20)
				&& (m_currentAnimation.m_state != AnimState::ANI_FALLDOWN))
			{
				SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_FALLDOWN]);
				return;
			}
			break;
		}
	}
};

