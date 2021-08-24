#pragma once

#include "stdafx.h"
#include "ActiviceSkill.h"

class TeleportSkill : public ActiviceSkill
{
public:
	TeleportSkill(std::string name = "") : ActiviceSkill(name)
	{
		TeleportSkill::name = "TeleportSkill";
	}

	virtual ~TeleportSkill() {}

	//함수를 정의하시오
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{ 
		ActiviceSkill::Start(gameObject); 
		fCoolDown = COOLTIME_TELEPORT;
		fPosX = 245.0f;
		fPosY = 490.0f;
	}
	
	virtual void OnPlayerUpdate(float fTime)
	{
		if (IsUse())
		{
			#ifdef _CONNECT_SERVER_
			if (INPUT->OnlyKeyDown(YT_KEY::YK_G))
			{
				Use();
				SOUNDMGR->Play("텔레포트", pPlayer->GetPositionXMFLOAT3());
			}
			#else
				if (INPUT->OnlyKeyDown(YT_KEY::YK_H))
				{
					//서버에서는 이걸 트루조건해줘야함
					cPlayerState->UseSKill(nSkillOrder);

					DWORD dwDirection = cPlayerState->GetDirection();

					XMVECTOR d3dxvShift = XMVectorZero();
					if (dwDirection & DIR_FORWARD)	d3dxvShift += pPlayer->GetLookVector();
					if (dwDirection & DIR_BACKWARD) d3dxvShift -= pPlayer->GetLookVector();
					if (dwDirection & DIR_RIGHT)	d3dxvShift += pPlayer->GetRightVector();
					if (dwDirection & DIR_LEFT)		d3dxvShift -= pPlayer->GetRightVector();
					XMVector3Normalize(d3dxvShift);

					XMVECTOR xm = XMVector3Normalize(d3dxvShift) * 1.5f;
					
					XMVECTOR pos = pPlayer->GetPosition();
					pPlayer->SetPosition(XMVectorAdd(xm, pos));

					XMFLOAT3 xm3 = pPlayer->GetPositionXMFLOAT3();
					if (CONTENTMGR->GetMapArrary(static_cast<int>(xm3.x), static_cast<int>(xm3.z)) > 0)
						pPlayer->SetPosition(pos);
					pPlayer->GetCamera()->RegenerateViewMatrix();

					auto position = pPlayer->GetPositionXMFLOAT3();
					SOUNDMGR->Play("텔레포트", position);
					PARTICLE->StartParticle("텔레포트", position.x, position.y + 0.5f, position.z, 5);		// - 하이드 스킬을 위한 노란큐브들

					Use();
				}
			#endif
		}
	}
};
	

