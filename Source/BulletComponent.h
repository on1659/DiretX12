#pragma once

#include "stdafx.h"
#include "ItemBasic.h"
#include "MonsterState.h"

class BulletComponent : public ItemBasic
{
public:
	BulletComponent(std::string name = "BulletComponent")
	{
		BulletComponent::name = name;
		tag = TAG_TYPE::TAG_ITEM_USE;
	}

	virtual ~BulletComponent() {}

	//변수를 선언하시오
private:
	XMVECTOR		vLook;
	float			fSpeed;

	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		ItemBasic::Start(gameObject);
		fSpeed = 20.0f;
	}

	virtual void Animate(float fTimeElapsed)
	{
		ItemBasic::Animate(fTimeElapsed);

		Move(fTimeElapsed);

		if (fTime > 10.0f)
		{
			ObjectDestory();
			return;
		}
	
		// 클라에서만쓰는 자진삭제 코드
		int nx, nz;
		if (!m_gameObject)return;
		nx = (int)(Helper::Rounding(m_gameObject->GetPositionXMFLOAT3().x) / BLOCK_SCALE);
		nz = (int)(Helper::Rounding(m_gameObject->GetPositionXMFLOAT3().z) / BLOCK_SCALE);

		if (nx >= 252) nx = 251;
		if (nz >= 160) nz = 159;
		if (nx < 0) nx = 0;
		if (nz < 0) nz = 0;

		if (1 == CONTENTMGR->GetMapArrary(nx, nz)) ObjectDestory();

	}

	void Move(float fTimeElapsed)
	{
		XMVECTOR pos = m_gameObject->GetPosition();
		pos += vLook * fTimeElapsed * fSpeed;
		//pos += vLook * distnace
		m_gameObject->SetPosition(pos);
	}

	void SetLook(XMVECTOR v3Look) { vLook = v3Look; }

	void ObjectDestory()
	{
		if (fTime < 0.5f)return;
		m_gameObject->SetActive(false);
		SPACE->DynamicDestory(m_gameObject);
	}
};

