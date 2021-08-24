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

	//�Լ��� �����Ͻÿ�
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
				SOUNDMGR->Play("�ڷ���Ʈ", pPlayer->GetPositionXMFLOAT3());
			}
			#else
				if (INPUT->OnlyKeyDown(YT_KEY::YK_H))
				{
					//���������� �̰� Ʈ�������������
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
					SOUNDMGR->Play("�ڷ���Ʈ", position);
					PARTICLE->StartParticle("�ڷ���Ʈ", position.x, position.y + 0.5f, position.z, 5);		// - ���̵� ��ų�� ���� ���ť���

					Use();
				}
			#endif
		}
	}
};
	

