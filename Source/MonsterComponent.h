#pragma once
//RSH 2016.04.21
/*
플레이어 캐릭터 AJ의 애니메이션을 제어하는 컴포넌트 스크립트입니다.
*/

#include "stdafx.h"
#include "AnimationComponent.h"
#include "GameObject.h"
#include "MonsterState.h"
#include "GameFramework.h"

class MonsterComponent : public Component
{
public:

	MonsterComponent(std::string sName = "MonsterComponent")
	{
		name = sName;
		tag = TAG_TYPE::TAG_NOTHING;
	}

	virtual ~MonsterComponent()
	{
		cMonsterState = nullptr;
	}

private:
	MonsterState*		cMonsterState;
	float				fDistance;
	float				fDegreeRange;
	float				fAngle;

public:

	virtual void Start(std::shared_ptr<CGameObject> gameobject)
	{
		Component::Start(gameobject);
		cMonsterState = static_cast<MonsterState*>(m_gameObject->GetComponent(0));
		fDistance = 5.0f;
		fDegreeRange = 68.0f;
		fAngle = 0.0f;
	}

	virtual void Animate(float fTimeElapsed)
	{
#ifndef _CONNECT_SERVER_
		if (cMonsterState->IsHeadBang())
		{
			cMonsterState->SetTarget(nullptr);
			fAngle = 0.0f;
			return;
		}
		MonsterComponent::Serch();

		auto target = cMonsterState->GetTarget();

		if (target)
		{
			//MonsterComponent::Chase(fTimeElapsed, target);
		}
#endif
	}

	//KYT '16.04.24
	/*
		충돌체크 예시.
		굳이 오버로딩 하지 않아도 괜찮습니다.
	*/
	virtual void Collision(std::shared_ptr<CGameObject> other, ObjectType type)
	{
		switch (type)
		{
		
		case ObjectType::DynamicObject:
			if (other->tag == TAG_TYPE::TAG_BULLET)
			{
				BulletComponent* bullet = static_cast<BulletComponent*>(other->GetComponent(0));
				if (bullet)
				{
					if (cMonsterState->IsSameTeam(bullet->GetId()))
						return;
						
						std::string name = other->name;
						
						if (name.find(HASHTABLE->STUN_ITEM()) < name.size())
						{
							cMonsterState->StartHeadBangBang();
							CScene::GetInstance()->AddHeadBangBang(GetPositionXMFLOAT3(), cMonsterState);
						}
					
						else if (name.find(HASHTABLE->FROZEN_ITEM()) < name.size())
						{
							cMonsterState->StartHeadBangBang();
							CScene::GetInstance()->AddHeadBangBang(GetPositionXMFLOAT3(), cMonsterState);
						}
						other->DynamicDestory(other);
					}

			}
			break;
		}
	}

	void Chase(float fTimeElapsed, CPlayer* target)
	{
		m_gameObject->Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), fAngle * fTimeElapsed * 2.0f);
		Component::Trasnlate(m_gameObject, 3.0f * fTimeElapsed, cMonsterState->GetDirection());
		CScene::GetInstance()->AddMonsterHeadCube(GetPositionXMFLOAT3(), target->GetPlayerState()->GetID());
	}

	void Serch()
	{
		auto vPlayer = cMonsterState->GetPlayerContainer();

		for (auto &player : vPlayer)
		{
			auto target = cMonsterState->GetTarget();

			if (target)
			{
				if (CircleCollision(player, fDistance * 2.0f))
				{
					if (!부채꼴충돌하고회전각설정(target, fDegreeRange * 1.17f))
					{
						cMonsterState->SetTarget(nullptr);
						fAngle = 0.0f;
					}
				}

			}

			else if (CircleCollision(player, fDistance))
			{
				//FONTMGR->MessagePush("기본", L"부채꼴", 500, 450, 30, YT_Color(255, 0, 0));
				if (부채꼴충돌하고회전각설정(player, fDegreeRange))
				{
					cMonsterState->SetTarget(player);
					auto id = player->GetPlayerState()->GetID();

					if(id == 0 || id == 1)
						cMonsterState->ChangeMonsterShader(MonsterTeam::MonsterTeam_A_Team);
					else
						cMonsterState->ChangeMonsterShader(MonsterTeam::MonsterTeam_B_Team);
					return;
				}
			}
		}
		cMonsterState->SetTarget(nullptr);
		fAngle = 0.0f;
	}

	//Serch를 위한 함수들
private:

	bool CircleCollision(CPlayer *player, float dis) 
	{
		auto a = m_gameObject->GetPositionXMFLOAT3();
		auto b = player->GetPositionXMFLOAT3();
		return dis > MathCalu::Length(Vector2(a.x, a.z), Vector2(b.x, b.z));
	}

	bool 부채꼴충돌하고회전각설정(CPlayer* player, float fRange)
	{
		if (player == nullptr)return false;

		XMFLOAT3 f3;
		cMonsterState->SetDirection(XMVector3Normalize(player->GetPosition() - GetPosition()));

		XMStoreFloat3(&f3, XMVector3Dot(cMonsterState->GetDirection(), GetLook()));

		if (std::abs(f3.x) <= 1)
			fAngle = (f3.x > 0.0f) ? D3DXToDegree(std::acos(f3.x)) : 90.0f;

		if (fAngle >= 89)return false;
		
		if (fRange > fAngle);
		{
			// uㆍ(v x d);
			XMStoreFloat3(&f3, XMVector3Dot(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), XMVector3Cross(GetLook(), cMonsterState->GetDirection())));
			if (f3.x < 0)fAngle *= -1;
			return true;
		}
		return false;
	}

	bool 부채꼴충돌하고회전각설정2(CPlayer* player, float fRange)
	{
		if (player == nullptr)return false;
		XMFLOAT3 f3;
		cMonsterState->SetDirection(XMVector3Normalize(player->GetPosition() - GetPosition()));

		XMStoreFloat3(&f3, XMVector3Dot(cMonsterState->GetDirection(), GetLook()));

		if (std::abs(f3.x) <= 1)
			fAngle = (f3.x > 0.0f) ? D3DXToDegree(std::acos(f3.x)) : 90.0f;

		FONTMGR->MessagePush(20, 250, 25, YT_Color(150, 0, 30), L"(fRange > fAngle) | (%1.3f, %1.3f) ", fRange, fAngle);
		if ((int)fAngle >= 80)
		{
			FONTMGR->MessagePush(20, 275, 25, YT_Color(150, 0, 30), L"(False)");
			return false;
		}
		if (fRange > abs(fAngle))
		{
			FONTMGR->MessagePush(20, 275, 25, YT_Color(150, 0, 30), L"(True)");
			// uㆍ(v x d);
			XMStoreFloat3(&f3, XMVector3Dot(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), XMVector3Cross(GetLook(), cMonsterState->GetDirection())));
			if (f3.x < 0)fAngle *= -1;
			return true;
		}
		FONTMGR->MessagePush(20, 275, 25, YT_Color(150, 0, 30), L"(False)");
		return false;
	}

};

