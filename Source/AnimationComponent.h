#pragma once

#include "Player.h"
#include "PlayerState.h"

class CAnimationComponent : public Component
{
	public:
		CAnimationComponent(std::string _name = "")
		{
			name = "CAnimationComponent";
			tag = TAG_TYPE::TAG_NOTHING;
		}
		
		virtual ~CAnimationComponent()
		{
			pPlayer = nullptr;
			cPlayerState = nullptr;
		}
	
		//변수를 선언하시오
	protected:
		//float							m_currentKeyframe;
		int								m_iCurrentKeyframe;
		float							m_fTimeElapsedStack;

		AnimationClip					m_currentAnimation;

		std::vector<AnimationClip>		m_AnimationClips;
		CPlayer*						pPlayer;
		PlayerState*					cPlayerState;
	
		//함수를 정의하시오
	public:
		void Start(std::shared_ptr<CGameObject> gameobject)
		{
			Component::Start(gameobject);
			pPlayer = static_cast<CPlayer*>(m_gameObject.get());
			cPlayerState = pPlayer->GetPlayerState();
		}

		void Respawn()
		{
			//cPlayerState->SetHp(1);
			SetAnimation(pPlayer, m_AnimationClips[AnimState::ANI_IDLE]);
		}

		void ProcessAnimationClips(){}

		void SetAnimation(std::shared_ptr<CGameObject> gameObject, AnimationClip clip)
		{
			m_currentAnimation = clip;
			m_fTimeElapsedStack = 0.0f;
			m_iCurrentKeyframe = clip.m_startKeyframe;
			gameObject->SetCurrentAnimationState(m_currentAnimation.m_state);
			gameObject->SetCurrentKeyframe(m_iCurrentKeyframe);
		}

		void SetAnimation(CPlayer* player, AnimationClip clip)
		{
			m_currentAnimation = clip;
			m_fTimeElapsedStack = 0.0f;
			m_iCurrentKeyframe = clip.m_startKeyframe;
			player->SetCurrentAnimationState(m_currentAnimation.m_state);
			player->SetCurrentKeyframe(m_iCurrentKeyframe);
		}

		void SmashParticle()
		{
			auto f3 = pPlayer->GetPositionXMFLOAT3();

			XMVECTOR xmLook = pPlayer->GetLook();
			XMVECTOR right = XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), xmLook);
			xmLook = xmLook * 0.2f;
			right = right * 0.1f;

			xmLook -= right;
			XMFLOAT3 d;
			XMStoreFloat3(&d, xmLook);


			PARTICLE->StartParticle("스매시", f3.x, f3.y + 1.0f, f3.z);
			PARTICLE->StartSprtieAnimation("스매시", f3.x - d.x, f3.y + 0.8f, f3.z - d.z);
		}
};

