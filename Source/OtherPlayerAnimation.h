#pragma once

//RSH '16.04.21
/*
	플레이어 캐릭터 AJ의 애니메이션을 제어하는 컴포넌트 스크립트입니다.
*/

#include "stdafx.h"
#include "AnimationComponent.h"
#include "BulletComponent.h"

class OtherPlayerAnimation : public CAnimationComponent
{

public:

	OtherPlayerAnimation(std::string _name = "")
	{
		name = _name;
		tag = TAG_TYPE::TAG_NOTHING;
	}

	virtual ~OtherPlayerAnimation()
	{
	}

	//변수를 선언하시오
private:
	int					m_MyRoomPlayerIndex;

	//RSH '16.08.10
	/*
	등짝 떄리기 및 스타팅 애니메이션 시간 고정을 위한 멤버변수
	*/
	float														m_fSmashTime;
	float														m_fSmashCurrentTime;
	bool														m_bSamshStart;

	float														m_fStartDelay;
	float														m_fStartCurrentDelay;

	std::chrono::time_point<std::chrono::steady_clock>			m_clockPoint;

	//발소리
	float														m_fFootStepTime;

	void ProcessAnimationClips()
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
		애니메이션 추가: 게임시작or리스폰 시 떨어지는 애니메이션 추가
		*/
		AnimationClip anim_startfall;

		anim_idle = { AnimState::ANI_IDLE, 0, 58, true };
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

		SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_STARTFALL - 1]);
		//SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_STARTFALL]);

		m_bSamshStart = false;

		//RSH '16.08.08
		/*
		때리기 모션이 동작하는 시간(2초)
		*/
		m_fSmashTime = 2000.0f;

		//RSH '16.08.08
		/*
		시작 모션이 동작하는 시간(3초)
		*/
		m_fStartDelay = 3000.0f;
		m_fStartCurrentDelay = 0.0f;
	}

	//함수를 정의하시오
public:
		
	void Start(std::shared_ptr<CGameObject> gameobject)
	{
		CAnimationComponent::Start(gameobject);
		ProcessAnimationClips();
		m_MyRoomPlayerIndex = cPlayerState->GetRoomPlayerIndex();
		m_fFootStepTime = 0.0f;
		//m_currentKeyframe = 0.0f;
	}

	//KYT '16.06.28
	/*
		Imsi
	*/
	void Animate(float fTimeElapsed)
	{
		m_fFootStepTime += fTimeElapsed;

		OtherPlayerAnimation::Animation(fTimeElapsed);

		#ifndef _CONNECT_SERVER_
			if (cPlayerState->GetID() == 1) 
			{	
				if (INPUT->OnlyKeyBoardDown(YT_KEY::YK_H))
				{
					if (m_gameObject->GetCurrentAnimationState() != AnimState::ANI_SMASH)
					{
						SetAnimation(pPlayer, m_AnimationClips[AnimState::ANI_SMASH]);
						return;
					}
				}

			Ani(fTimeElapsed);
			}
		#endif
			PlayFootStep();
	}

	void Animation(float fTimeElapsed)
	{
		
		//RSH '16.08.08
		/*
			시작 모션 동작 조건문.(게임 시작or 리스폰)
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
					//m_bPlayStart = false;
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
				//반복하는 애니메이션이 아닐 경우...
				//이에 해당하는 애니메이션: 때리기, 투척, 죽음, 아이템 사용

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
				등짝때리기의 경우에는 서버에서 정밀한 타이밍 판정을 해야되기 떄문에
				2초 동안 등짝때리기 애니메이션을 나누어 제작하여야한다.
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

		//RSH 16.07.18
		/*
		충돌 시 쓰러지는 상대 플레이어 구현.
		쓰러지는 애니메이션 상태 중에는 쓰러지지 않는다.
		*/
		if (false == cPlayerState->IsLife())
		{
			if (m_currentAnimation.m_state != AnimState::ANI_FALLDOWN)
			{
				//RSH '16.08.12
				/*
					파티클도 렌더링 시작!
				*/
				SmashParticle();
				SetAnimation(pPlayer, m_AnimationClips[AnimState::ANI_FALLDOWN]);
			}
		}
		if (INPUT->OnlyKeyDown(YT_KEY::YK_O))
		{
			auto f3 = m_gameObject->GetPositionXMFLOAT3();
			PARTICLE->StartParticle("스매시큐브", f3.x, f3.y + 0.8f, f3.z);
		}

		if (m_currentAnimation.m_state == AnimState::ANI_FALLDOWN)
		{
			pPlayer->SetCurrentKeyframe(m_iCurrentKeyframe);
			return;
		}

		#ifndef _CONNECT_SERVER_
			pPlayer->SetCurrentKeyframe(m_iCurrentKeyframe);
			return;
		#endif

		if (TCP->GetRoomPlayerInfo(m_MyRoomPlayerIndex).player_input_info.key_SHIFT_Down)
		{
			pPlayer->SetAnimationSpeed(PLAYER_RUN_ANISPEED);
		}
		else
		{
			if (PLAYER_ANISPEED != pPlayer->GetAnimationSpeed())
			{
				pPlayer->SetAnimationSpeed(PLAYER_ANISPEED);
			}
		}

		if (false == m_currentAnimation.m_isLoop)
		{
			//int keynum = (int)m_currentKeyframe;
			//if (keynum > m_currentAnimation.m_endKeyframe) keynum = m_currentAnimation.m_endKeyframe - 1;
			pPlayer->SetCurrentKeyframe(m_iCurrentKeyframe);
			return;
		}

		//JJY '16.05.01
		/*
		Otherplayer 인풋으로 수정
		*/

		if (TCP->GetRoomPlayerInfo(m_MyRoomPlayerIndex).player_input_info.key_SPACE_Down)
		{
			if (m_currentAnimation.m_state != AnimState::ANI_SMASH)
			{
				SetAnimation(pPlayer, m_AnimationClips[AnimState::ANI_SMASH]);
				return;
			}
		}


		if (TCP->GetRoomPlayerInfo(m_MyRoomPlayerIndex).player_input_info.key_W_Down)
		{
			if (TCP->GetRoomPlayerInfo(m_MyRoomPlayerIndex).player_input_info.key_A_Down)
			{
				if (m_currentAnimation.m_state != AnimState::ANI_LEFT_FOWARD_DIAGONAL)
				{
					SetAnimation(pPlayer, m_AnimationClips[AnimState::ANI_LEFT_FOWARD_DIAGONAL]);
					return;
				}
			}

			else if (TCP->GetRoomPlayerInfo(m_MyRoomPlayerIndex).player_input_info.key_D_Down)
			{
				if (m_currentAnimation.m_state != AnimState::ANI_RIGHT_FOWARD_DIAGONAL)
				{
					SetAnimation(pPlayer, m_AnimationClips[AnimState::ANI_RIGHT_FOWARD_DIAGONAL]);
					return;
				}
			}

			else if (m_currentAnimation.m_state != AnimState::ANI_RUN)
			{
				SetAnimation(pPlayer, m_AnimationClips[AnimState::ANI_RUN]);
				return;
			}
		}
		else
		{

			if (TCP->GetRoomPlayerInfo(m_MyRoomPlayerIndex).player_input_info.key_S_Down)
			{
				if (m_currentAnimation.m_state != AnimState::ANI_RUN_BACKWARD)
				{
					SetAnimation(pPlayer, m_AnimationClips[AnimState::ANI_RUN_BACKWARD]);
					return;
				}
			}
			else
			{
				if (TCP->GetRoomPlayerInfo(m_MyRoomPlayerIndex).player_input_info.key_A_Down)
				{
					if (m_currentAnimation.m_state != AnimState::ANI_LEFT_STRAFE)
					{
						SetAnimation(pPlayer, m_AnimationClips[AnimState::ANI_LEFT_STRAFE]);
						return;
					}
				}

				else if (TCP->GetRoomPlayerInfo(m_MyRoomPlayerIndex).player_input_info.key_D_Down)
				{
					if (m_currentAnimation.m_state != AnimState::ANI_RIGHT_STRAFE)
					{
						SetAnimation(pPlayer, m_AnimationClips[AnimState::ANI_RIGHT_STRAFE]);
						return;
					}
				}
			}
		}

		if (
			TCP->GetRoomPlayerInfo(m_MyRoomPlayerIndex).player_input_info.key_W_Down == false
			&& TCP->GetRoomPlayerInfo(m_MyRoomPlayerIndex).player_input_info.key_A_Down == false
			&& TCP->GetRoomPlayerInfo(m_MyRoomPlayerIndex).player_input_info.key_S_Down == false
			&& TCP->GetRoomPlayerInfo(m_MyRoomPlayerIndex).player_input_info.key_D_Down == false
			)
		{
			if (m_currentAnimation.m_state != AnimState::ANI_IDLE)
			{
				SetAnimation(pPlayer, m_AnimationClips[AnimState::ANI_IDLE]);
				return;
			}
		}

		//int keynum = (int)m_currentKeyframe;
		//if (keynum > m_currentAnimation.m_endKeyframe) keynum = m_currentAnimation.m_endKeyframe - 1;
		pPlayer->SetCurrentKeyframe(m_iCurrentKeyframe);
	}


	void PlayFootStep()
	{
		if (m_currentAnimation.m_state == AnimState::ANI_IDLE)return;
		if (m_currentAnimation.m_state == AnimState::ANI_FALLDOWN)return;
		if (m_currentAnimation.m_state == AnimState::ANI_SMASH)return;
		if (m_currentAnimation.m_state == AnimState::ANI_STARTFALL)return;

		if (m_currentAnimation.m_state != AnimState::ANI_IDLE)
		{
			if (m_fFootStepTime > 0.3f)
			{
				SOUNDMGR->Play("발소리", m_gameObject->GetPositionXMFLOAT3());
				m_fFootStepTime = 0.0f;
				std::cout << " 발소리 " << std::endl;
			}
		}
	}

	void Ani(float fTimeElapsed)
	{
		auto UP    = YT_KEY::YK_I;
		auto LEFT  = YT_KEY::YK_J;
		auto DOWN  = YT_KEY::YK_K;
		auto RIGHT = YT_KEY::YK_L;
		auto SHIFT = YT_KEY::YK_H;

		DWORD dwDirection;
		if (cPlayerState) dwDirection = cPlayerState->GetDirection();

		if (INPUT->OnlyKeyDown(SHIFT))
		{
			INPUT->StartVibrate(0, 30000);
			if (m_currentAnimation.m_state != AnimState::ANI_SMASH)
			{
				SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_SMASH]);
				cPlayerState->SetInputPossible(false);
				return;
			}
		}

		if (INPUT->KeyDown(UP))
		{
			if (INPUT->KeyDown(LEFT))
			{
				if (m_currentAnimation.m_state != AnimState::ANI_LEFT_FOWARD_DIAGONAL)
				{
					SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_LEFT_FOWARD_DIAGONAL]);
					return;
				}
			}

			else if (INPUT->KeyDown(RIGHT))
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

			if (INPUT->KeyDown(DOWN))
			{
				if (m_currentAnimation.m_state != AnimState::ANI_RUN_BACKWARD)
				{
					SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_RUN_BACKWARD]);
					return;
				}
			}
			else
			{
				if (INPUT->KeyDown(LEFT))
				{
					if (m_currentAnimation.m_state != AnimState::ANI_LEFT_STRAFE)
					{
						SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_LEFT_STRAFE]);
						return;
					}
				}

				else if (INPUT->KeyDown(RIGHT))
				{
					if (m_currentAnimation.m_state != AnimState::ANI_RIGHT_STRAFE)
					{
						SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_RIGHT_STRAFE]);
						return;
					}
				}
			}
		}


		if (  (INPUT->KeyUp(UP)
			|| INPUT->KeyUp(LEFT)
			|| INPUT->KeyUp(RIGHT)
			|| INPUT->KeyUp(DOWN))
			)
		{
			if (m_currentAnimation.m_state != AnimState::ANI_IDLE)
			{
				SetAnimation(m_gameObject, m_AnimationClips[AnimState::ANI_IDLE]);
				return;
			}
		}

	}


};

