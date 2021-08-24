#pragma once

#include "stdafx.h"
#include "AnimationComponent.h"
#include "BulletComponent.h"
#include "Timer.h"
#include "Scene.h"

class MonsterState : public Component
{
public:

	MonsterState(std::string sName = "MonsterState")
	{
		name = sName;
		tag = TAG_TYPE::TAG_NOTHING;
	}

	virtual ~MonsterState()
	{
	}

	//변수를 선언하시오
private:
	std::vector<CPlayer*>							vOtherPlayer;
	std::vector<CPlayer*>							vSameTeamPlayer;
	std::vector<CPlayer*>							vOtherTeamPlayer;
	int												nTeam;
	bool											bChase;
	CPlayer*										pTarget;

	XMVECTOR										xmMoveDir;

	std::vector
		<std::shared_ptr<CInstancedObjectsShader>>	m_vMonsterShader;

	bool											nHp;



	//함수를 정의하시오
public:

	virtual void Start(std::shared_ptr<CGameObject> gameobject)
	{
		Component::Start(gameobject);

		nTeam = MonsterTeam::MonsterTeam_A_Team;
		bChase = false;
		pTarget = nullptr;

		m_bHeadBnagbang = false;
		xmMoveDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		m_gameObject->Rotate(0, 45, 0);
		
		MonsterState::SetHp(1);
	}

	void SetTeam(int n)
	{
		nTeam = n;
	}

	//살아있니
	void SetHp(const int& hp) { nHp = hp; }
	bool IsAlive() const { return nHp > 0 ? true : false; }
	int GetHp() const { return nHp; }

	//같은팀인가요/
	bool IsSameTeam(int id)
	{
		if (id == 0 || id == 1)		// 플레이어가 A팀일때
		{
			if (nTeam == MonsterTeam::MonsterTeam_A_Team)	// 같은팀이란거임
				return true;
			else					// 다른팀이란거임
				return false;
		}
		else						// 플레이어가 B팀일때
		{
			if (nTeam == MonsterTeam::MonsterTeam_B_Team)	// 같은팀이란거임
				return true;
			else					// 다른팀이란거임
				return false;
		}
	}

	//Monster
	void SetMonsterShader(std::shared_ptr<CInstancedObjectsShader> pMonsterShader) { m_vMonsterShader.push_back(pMonsterShader); }

	void SetMonsterShader(std::vector<std::shared_ptr<CInstancedObjectsShader>> vMonsterShader)
	{
		for (auto &pShader : vMonsterShader)SetMonsterShader(pShader);
	}

	void ChangeMonsterShader(int n)
	{
		m_gameObject->SetShader(m_vMonsterShader[n], false);
	}

	//플레이어 세팅 - SamePlayer, OtherTeamPlayer 컨테이너로 나뉘어짐
	void SetPlayer(const std::vector<CPlayer*> vPlayer)
	{
		for (auto &player : vPlayer) SetPlayer(player);
	}

	//플레이어 세팅 - SamePlayer, OtherTeamPlayer 컨테이너로 나뉘어짐
	void SetPlayer(CPlayer* player)
	{
		int id = player->GetPlayerState()->GetID();

		vOtherPlayer.push_back(player);		//플레이어 전체

		if (IsSameTeam(id))
			vSameTeamPlayer.push_back(player);
		else					// 다른팀이란거임
			vOtherTeamPlayer.push_back(player);

		std::sort(vOtherPlayer.begin(), vOtherPlayer.end(),
			[](CPlayer* a, CPlayer* b)
		{
			return (a->GetPlayerState()->GetID() < b->GetPlayerState()->GetID());
		}
		);
	}

	//팀이 바뀔떄 호출하면 서로 플레이어가 서로 나뉘어짐
	void ChangeTeam(CPlayer* player)
	{
		int id = player->GetPlayerState()->GetID();

		if (false == IsSameTeam(id))
		{
			for (auto &player : vSameTeamPlayer) player = nullptr;
			for (auto &player : vOtherPlayer)	 player = nullptr;

			vOtherPlayer.clear();
			vSameTeamPlayer.clear();

			for (auto &player : vOtherPlayer)
			{
				if (IsSameTeam(player->GetPlayerState()->GetID()))
					vSameTeamPlayer.push_back(player);
				else
					vOtherPlayer.push_back(player);
			}
		}
	}

	CPlayer* GetPlayer(int n) const { return vOtherPlayer[n]; }
	std::vector<CPlayer*> GetPlayerContainer() const { return vOtherPlayer; }

	void SetTarget(CPlayer* target) { pTarget = target; }
	CPlayer* GetTarget() const { return pTarget; }

	void SetDirection(const XMVECTOR& dir) { xmMoveDir = dir; }
	XMVECTOR GetDirection() const { return xmMoveDir; }

private:
	bool											m_bHeadBnagbang;

public:
	void  StartHeadBangBang() 
	{
		m_bHeadBnagbang = true; 
	}
	void  StopHeadBangBang() { m_bHeadBnagbang = false; }
	bool  IsHeadBang() const { return m_bHeadBnagbang; }

};

