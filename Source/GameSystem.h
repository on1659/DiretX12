#pragma once

//KYT '16.06.26
/*
	GameSystem
	GAME_PLAY_MODE - 패킷매니저에서 따옴
*/

#include "stdafx.h"
#include "FontManager.h"
#include "PlayerState.h"
#include "Component.h"

class CGameSystem : public Object
{
protected:

	ID3D11Device*						 m_pd3dDvice;
										 
	FontMessageType						 m_fmsgTime;
	int									 m_nGamePlayMode;
	int									 m_nTeamReadScore;
	int									 m_nTeamBlueScore;
										 
	int									 m_nGameState;
	float								 m_fGameTime;
										 
	float								 m_fMaxTime;
	int									 m_nRemainTime;

	std::vector<CPlayer*>				 m_vPlayers;
	std::vector<PlayerState*>			 m_vPlayersState;


	//
	std::string							 m_sBulletinView;
	bool								 m_bBulletin;

public:	

	CGameSystem() : 
		  Object() 
		, m_pd3dDvice(nullptr)
		, m_fmsgTime()
		, m_nGamePlayMode(0)
		, m_nTeamBlueScore(0)
		, m_nTeamReadScore(0)
		, m_nGameState(GAME_PLAY_STATE::GAME_PLAY_PLAYING)
		, m_fGameTime(0.0f)
		, m_bBulletin(false)
	{
		m_vPlayers.clear();
		m_vPlayersState.clear();
		
		m_sBulletinView = "BulletinView";

	}

	virtual ~CGameSystem()
	{

	}

	virtual void Release() = 0;

	virtual void Load(ID3D11Device* pd3dDevice) = 0;

	virtual void Update(float fTimeElapsed) = 0;

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext) = 0;

	virtual void ChangeState() = 0;

	virtual bool OnCollision() = 0;

	template <class T>
	void LoadPlayer(const T& player){ m_vPlayers.push_back(player); }

	template <class First, class... Rest>
	void LoadPlayer(const First& first, const Rest&... rest)
	{
		m_vPlayers.push_back(first);
		LoadPlayer(rest...); // recursive call using pack expansion syntax
	}

	virtual void LastLoad()
	{
		std::sort(m_vPlayers.begin(), m_vPlayers.end(),
		[](const auto& object, const auto& other)
		{
			PlayerState* a = (PlayerState*)object->GetComponent(0);
			PlayerState* b =  (PlayerState*)other->GetComponent(0);
			return (a->GetID() > b->GetID());
		}
		);

		for (auto &player : m_vPlayers) 
			m_vPlayersState.push_back(player->GetPlayerState());
	}

	//
	bool IsGameEnd() const { return (CGameSystem::GetGameMode() == GAME_PLAY_STATE::GAME_PLAY_STOP); }

	//Time
	void UpdateTime(float fTimeElapsed) { m_fGameTime += fTimeElapsed; }
	void InitTime() { m_fGameTime = 0.0f; }

	//Mode GetSte
	int GetGameMode() const { return m_nGameState; }
	void SetGameMode(int state)  { m_nGameState = state; }


};

