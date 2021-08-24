#pragma once

#include "stdafx.h"
#include "Player.h"

class CAnimationComponent;

#define SMASHBOX_SIZE 0.1f
#define BACKBOX_SIZE 0.1f

class PlayerState : public Component
{
public:

	PlayerState(std::string name = "")
	{
		PlayerState::name = "PlayerState";
		tag = TAG_TYPE::TAG_NOTHING;
		m_sCharacterName = "";
		bAnimationInit = true;
		m_MyRoomPlayerIndex = 0;
	}

	virtual ~PlayerState() { m_pPlayer = nullptr; }

	//변수를 선언하시오
private:
	
	bool			m_bInputPossible;

	float			m_fSpeed;
	float			m_bVisible;

	int				m_nThrowItem;
	DWORD			m_dwDirection;

	int 			m_bHaveItemCheck[4];

	int				m_lastItem;

	CPlayer*		m_pPlayer;

	//FreeCamera 만들기용
	bool			m_bFreeCamera;
	std::string		m_sCharacterName;

public:
	void SetDirection(DWORD direction) { m_dwDirection = direction; }
	DWORD GetDirection() const { return m_dwDirection; }


	//함수를 정의하시오
public:

	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		Component::Start(gameObject);

		if (gameObject)
		{
			m_pPlayer = nullptr;
			m_pPlayer = static_cast<CPlayer*>(gameObject.get());
		}

		m_gameObject.reset();

		m_fSpeed = 3.0f;
		m_nThrowItem = 0;
		m_bVisible = 0.0f;


		//키 입력을 받을지 아닐지
		m_bInputPossible = true;

		//현재 키 상황
		m_dwDirection = 0;

		m_bHaveItemCheck[0] = ITEM_TYPE::ITEM_TYPE_NOTHING;
		m_bHaveItemCheck[1] = ITEM_TYPE::ITEM_TYPE_NOTHING;
		m_bHaveItemCheck[2] = ITEM_TYPE::ITEM_TYPE_NOTHING;
		m_bHaveItemCheck[3] = ITEM_TYPE::ITEM_TYPE_NOTHING;

		//마지막 아이템키
		m_nHp = 1;
		m_nKillCount = 0;
		m_nDeadCount = 0;

		fSmashDistance = 1.5f;
		fSmashHeight = 0.5f;
		#ifdef BOUNDING_ORIENTED_BOX
				m_bcSmashBoundingBox.Extents = XMFLOAT3(BLOCK_SCALE * 0.1f, BLOCK_SCALE * 0.1f, BLOCK_SCALE * 0.1f);
		#else
				m_bcSmashBoundingBox.Extents = XMFLOAT3(BLOCK_SCALE * 0.1f, BLOCK_SCALE * 0.1f, BLOCK_SCALE * 0.1f);
		#endif

		xmBackPosition = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		fBackPositionY = 1.0f;
		#ifdef BOUNDING_ORIENTED_BOX
				m_bcBackBoundingBox.Extents = XMFLOAT3(BLOCK_SCALE * 0.1f, BLOCK_SCALE * 0.1f, BLOCK_SCALE * 0.1f);
		#else
				m_bcBackBoundingBox.Extents = XMFLOAT3(BLOCK_SCALE * 0.1f, BLOCK_SCALE * 0.1f, BLOCK_SCALE * 0.1f);
		#endif

		m_bFreeCamera = false;

		//HeadBang
		m_fHead = 0.0f;


		//------------------------------------------------------------------------------------
		float f = Helper::RuntimeDataModify(float(), "f_PlayerSmashScale");
		#ifdef BOUNDING_ORIENTED_BOX
				m_bcSmashBoundingBox.Extents = XMFLOAT3(BLOCK_SCALE * f, BLOCK_SCALE * f, BLOCK_SCALE * f);
		#else
				m_bcSmashBoundingBox.Extents = XMFLOAT3(BLOCK_SCALE * f, BLOCK_SCALE * f, BLOCK_SCALE * f);
		#endif
		
		fSmashDistance = Helper::RuntimeDataModify(float(), "f_PlayerSmashDistance");

		//Back
		float f2 = Helper::RuntimeDataModify(float(), "f_PlayerBackScale");
		#ifdef BOUNDING_ORIENTED_BOX
				m_bcBackBoundingBox.Extents = XMFLOAT3(BLOCK_SCALE * f2, BLOCK_SCALE * f2, BLOCK_SCALE * f2);
		#else
				m_bcBackBoundingBox.Extents = XMFLOAT3(BLOCK_SCALE * f2, BLOCK_SCALE * f2, BLOCK_SCALE * f2);
		#endif

		XMFLOAT3 f3 = Helper::RuntimeDataModify(XMFLOAT3(), "xf3_PlayerBackDistance");
		xmBackPosition = XMVectorSet(f3.x, f3.y, f3.z, 1.0f);
		fBackPositionY = f3.y;

		m_bPassiveSKillEvasion = false;
		m_bPassiveSKillSpeedUp = false;

		for (int i = 0; i < 4; ++i) m_bSkillUse[i] = false;
		m_nSkillCount = 0;

	}

	#ifdef DEBUG_MODE_UI
	virtual void OnPlayerUpdate(float fTimeElapsed)
	{
		if (m_pPlayer->name == "Player")
		{
			for (int i = 0; i < 4; ++i)
				FONTMGR->MessagePush("기본", 150, 150 + i * 15, 15, YT_Color(255, 0, 0), L"(Item %d)", m_bHaveItemCheck[i]);
		}
	}
	#endif

	#ifdef _RUNTIME_DEBUGFILE
	virtual void OnPlayerUpdate(float fTimeElapsed)
	{
		m_dwDirection = 0;
		
		if (INPUT->OnlyKeyDown(YT_KEY::YK_F5))
		{
			//Smash
			float f = Helper::RuntimeDataModify(float(), "f_PlayerSmashScale");
			
			::ConsoleShow(m_bcSmashBoundingBox.Extents, "\nBefore Extents : ");
			
			m_bcSmashBoundingBox.Extents = XMFLOAT3(BLOCK_SCALE * f, BLOCK_SCALE * f, BLOCK_SCALE * f);
			::ConsoleShow(m_bcSmashBoundingBox.Extents, "After Extents : ");


			fSmashDistance = Helper::RuntimeDataModify(float(), "f_PlayerSmashDistance");

			//Back
			float f2 = Helper::RuntimeDataModify(float(), "f_PlayerBackScale");
			m_bcBackBoundingBox.Extents = XMFLOAT3(BLOCK_SCALE * f2, BLOCK_SCALE * f2, BLOCK_SCALE * f2);

			XMFLOAT3 f3 = Helper::RuntimeDataModify(XMFLOAT3(), "xf3_PlayerBackDistance");
			xmBackPosition = XMVectorSet(f3.x, f3.y, f3.z, 1.0f);
			fBackPositionY = f3.y;

			std::cout << std::endl;
			std::cout << "---------------------------------------------------" << std::endl;
			std::cout << "---------------------------------------------------" << std::endl;
			std::cout << "PlayerSmashScale : " << f << std::endl;
			std::cout << "PlayerSmashDistance : " << fSmashDistance << std::endl;
			std::cout << "------" << std::endl;
			std::cout << "PlayerBackScale : " << f2 << std::endl;
			std::cout << "PlayerBackDistance(" << f3.x << "," << f3.y << "," << f3.z << ")" << std::endl;
			std::cout << "---------------------------------------------------" << std::endl;
			std::cout << "---------------------------------------------------" << std::endl;

			//m_bcBackBoundingBox.Extents = XMFLOAT3(BLOCK_SCALE * f3.x, BLOCK_SCALE * f3.y, BLOCK_SCALE * f3.z);
		}
		
		if (INPUT->OnlyKeyDown(YT_KEY::YK_8))
		{
			m_fHead = m_fHead ? 0.0f : 0.01f;
		}

	}
	#endif

	//KYT '16.05.12
	/*
		Item을 캐릭터에 장착하고, 어디에 장착되어있는지를 리턴한다.
	*/
	int GainItem(int itemType)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_bHaveItemCheck[i] == ITEM_TYPE::ITEM_TYPE_NOTHING)
			{
				m_bHaveItemCheck[i] = itemType;
				m_lastItem = i;
				return m_lastItem;
			}

			/*
				같은 아이템이라도 누적이 되지 않는다.
			*/
			//if (m_bHaveItemCheck[i] == itemType)
			//{
			//	return i;
			//}
		}

		m_lastItem = -1;
		return m_lastItem;
	}
	void UseItem(int slot)
	{
		if (slot == -1)return;
		m_bHaveItemCheck[slot] = ITEM_TYPE::ITEM_TYPE_NOTHING;
	}

	// Get & Set
	void SetSpeed(const float& speed) { m_fSpeed = speed; }
	float GetSpeed() const { return m_fSpeed; }

	void SetVisible(const float& fTimeElasped)
	{
		if (fTimeElasped == 0.0f)
			m_bVisible = 0.0f;
		else
			m_bVisible += fTimeElasped;
	}
	float GetVisible() const { return m_bVisible; }

	void GainThrowItem(const int& n) { m_nThrowItem++; }
	void UseThrowItem() { m_nThrowItem--; }
	int  GetThrowItem() const { return m_nThrowItem; }

	void SetInputPossible(const bool& possible) { m_bInputPossible = possible; }
	bool GetInputPossible() const { return m_bInputPossible; }

	void SetFreeCamera(const bool& mode) { m_bFreeCamera = mode; }
	bool IsFreeCameraAlive() const { return m_bFreeCamera; }

	int GetLastGetUseItem() const { return m_lastItem; }
	
	void SetCharcaterName(const std::string& name) { m_sCharacterName = name; }
	std::string GetCharcaterName() const { return m_sCharacterName; }

	//KYT '16.06.28 
	//KYT '16.07.21		-> 추가
	/*
		Player Game System Data
	*/
private:
	int				m_nHp;					//추후 1로 바꿈
	int				m_nKillCount;			//Kill Count
	int				m_nDeadCount;			//Dead Count
	int				m_nID;					//무슨팀이니
	std::string		m_sPath;
	
	
	float			m_fHead;	
	bool			m_bPassiveSKillEvasion;
	bool			m_bPassiveSKillSpeedUp;

	bool			m_bSkillUse[4];
	int				m_nSkillCount;


public:
	void SetHp(int hp) { m_nHp = hp; }
	int   GetHp() const { return m_nHp; }
	bool IsLife() const { return m_nHp == 0 ? false : true; }
	void OffSetHp(int hp = 1) { m_nHp += hp; }

	void SetKillCount(int killCount) { m_nKillCount = killCount; }
	void AddKillCount(int count) { m_nKillCount += count; }
	void SubKillCount(int count) { m_nKillCount -= count; }
	int GetKillCount() const { return m_nKillCount; }

	void SetDeadCount(int count) { m_nDeadCount = count; }
	void AddDeadCount(int count) { m_nDeadCount += count; }
	void SubDeadCount(int count) { m_nDeadCount -= count; }
	int GetDeadCount() const { return m_nDeadCount; }

	void SetID(int id) { m_nID = id; }
	int  GetID() const { return m_nID; }

	void  StartHeadBangBang(float fTime = 0.01f) { m_fHead += fTime; }
	void  StopHeadBangBang() { m_fHead = 0.0f; }
	float GetHeadBang() const { return m_fHead;  }
	bool  IsHeadBang() const { return m_fHead > 0 ? true : false; }

	void SetPath(std::string path) { m_sPath = path; }
	std::string GetPath() const { return m_sPath; }

	//회피
	void SetPassiveSkillEvasion(const bool& flag) { m_bPassiveSKillEvasion = flag; }
	bool GetPassiveSkillEvasion() const { return m_bPassiveSKillEvasion; }

	//스피드업
	void SetPassiveSkillSpeedUp(const bool& flag) { m_bPassiveSKillSpeedUp = flag; }
	bool GetPassiveSkillSpeedUp() const { return m_bPassiveSKillSpeedUp; }


	//스킬 네임정해주는거
	int GetSkillOrdering() { return m_nSkillCount++; }

	void UseSKill(int n) { m_bSkillUse[n] = true; }

	bool IsSKillUse(int n)
	{
		if (m_nSkillCount > n)
		{
			if (n >= 0)
			{
				return m_bSkillUse[n];
			}
		}
		::GMessageBox(L"SKill Error", L"스킬 배열을 초과했습니다.");
		return false;
	}

private:
#ifdef BOUNDING_ORIENTED_BOX
	DirectX::BoundingOrientedBox m_bcSmashBoundingBox;
#else
	BoundingBox		m_bcSmashBoundingBox;
#endif
	float			fSmashDistance;
	float			fSmashHeight;

#ifdef BOUNDING_ORIENTED_BOX
	DirectX::BoundingOrientedBox m_bcBackBoundingBox;
#else
	BoundingBox		m_bcBackBoundingBox;
#endif
	float			fBackPositionY;
	XMVECTOR		xmBackPosition;

public:
#ifdef BOUNDING_ORIENTED_BOX
	DirectX::BoundingOrientedBox GetSmashBoundingBox()
	{
		m_bcSmashBoundingBox.Center = m_pPlayer->GetBoundingBox().Center;
		m_bcSmashBoundingBox.Center.y += fSmashHeight;

		//월드 행렬에 회전 행렬을 쿼터니언으로 담는다.
		//XMStoreFloat4(&(m_bcSmashBoundingBox.Orientation), XMQuaternionRotationMatrix(m_pPlayer->GetWorldMatrix()));

		XMVECTOR xm = DirectX::XMVectorAdd(m_pPlayer->GetLook() * fSmashDistance, XMLoadFloat3(&m_bcSmashBoundingBox.Center));
		XMStoreFloat3(&m_bcSmashBoundingBox.Center, xm);

		return m_bcSmashBoundingBox;
	}

	DirectX::BoundingOrientedBox GetBackBoudingBox()
	{
		m_bcBackBoundingBox.Center = m_pPlayer->GetBoundingBox().Center;

		//월드 행렬에 회전 행렬을 쿼터니언으로 담는다.
		XMStoreFloat4(&(m_bcBackBoundingBox.Orientation), XMQuaternionRotationMatrix(m_pPlayer->GetWorldMatrix()));

		XMVECTOR xm = m_pPlayer->GetLook() * xmBackPosition;
		XMStoreFloat3
		(
			&m_bcBackBoundingBox.Center,
			DirectX::XMVectorSubtract(XMLoadFloat3(&m_bcBackBoundingBox.Center), xm)
		);

		m_bcBackBoundingBox.Center.y = fBackPositionY;
		return m_bcBackBoundingBox;
	}
#else
	BoundingBox GetSmashBoundingBox()
	{
		//m_bcSmashBoundingBox.Center = m_pPlayer->GetBoundingBox().Center;
		//m_bcSmashBoundingBox.Center.y += fSmashHeight;
		//XMVECTOR xm = DirectX::XMVectorAdd(m_pPlayer->GetLook() * fSmashDistance, XMLoadFloat3(&m_bcSmashBoundingBox.Center));
		//
		//XMStoreFloat3(&m_bcSmashBoundingBox.Center, xm);
		//return m_bcSmashBoundingBox;

		m_bcSmashBoundingBox.Center = m_pPlayer->GetPositionXMFLOAT3();

		XMVECTOR playerDirection = XMVector3Normalize(m_pPlayer->GetLook());

		XMVECTOR xm = playerDirection * fSmashDistance; //xmBackPosition;
		XMStoreFloat3
		(
			&m_bcSmashBoundingBox.Center,
			DirectX::XMVectorAdd(XMLoadFloat3(&m_bcSmashBoundingBox.Center), xm)
		);

		m_bcSmashBoundingBox.Center.y += fSmashHeight;

		return m_bcSmashBoundingBox;
	}

	BoundingBox GetBackBoudingBox()
	{
		//m_bcBackBoundingBox.Center = m_pPlayer->GetBoundingBox().Center;
		//XMVECTOR xm = m_pPlayer->GetLook() * xmBackPosition;
		//XMStoreFloat3
		//(
		//	&m_bcBackBoundingBox.Center,
		//	DirectX::XMVectorSubtract(XMLoadFloat3(&m_bcBackBoundingBox.Center), xm)
		//);
		//
		//m_bcBackBoundingBox.Center.y = fBackPositionY;
		//return m_bcBackBoundingBox;

		m_bcBackBoundingBox.Center = m_pPlayer->GetPositionXMFLOAT3();
		
		XMVECTOR playerDirection = XMVector3Normalize(m_pPlayer->GetLook(false));
		
		XMVECTOR xm = playerDirection * 0.5;
		XMStoreFloat3
		(
			&m_bcBackBoundingBox.Center, 
			DirectX::XMVectorSubtract(XMLoadFloat3(&m_bcBackBoundingBox.Center), xm)
		);

		m_bcBackBoundingBox.Center.y = fBackPositionY;

		return m_bcBackBoundingBox;
	}
#endif
	
private:
	int m_MyRoomPlayerIndex;

public:
	//JJY '16.05.01
	/*
		Otherplayer 인덱스값 넣어줌
	*/
	void SetRoomPlayerIndex(int idx) {	m_MyRoomPlayerIndex = idx; }
	int GetRoomPlayerIndex() const { return m_MyRoomPlayerIndex; }

public:
	void Respawn() 
	{
		m_pPlayer->Respawn();
	}


private:
	bool bAnimationInit;
public:
	void AnimationInitEnd() { bAnimationInit = false; }
	bool IsAnimationInit() const { return bAnimationInit; }
};



