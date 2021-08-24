#pragma once

#include "stdafx.h"

#include "PlayerState.h"
#include "InputManager.h"
#include "BulletComponent.h"
#include "Timer.h"
#include "TerrainPlayer.h"
#include "ItemFake.h"
#include "GameFramework.h"

//#define _CONNECT_SERVER_

class PlayerComponet : public Component
{

private:
protected:
	PlayerState*		cPlayerState;
	CPlayer*			pPlayer;

	float				m_fDuringStunTime;
	float				m_fLimitBewteenAngle;

	

public:
	PlayerComponet(std::string _name = "")
	{
		name = "PlayerComponet";
		tag = TAG_TYPE::TAG_PlayerComponet;
		m_fDuringStunTime = 5.0f;
		m_fLimitBewteenAngle = Helper::RuntimeDataModify(float(), "f3_Angle");
	}

	virtual ~PlayerComponet()
	{
		pPlayer = nullptr;
		cPlayerState = nullptr;
	}

	//������ �����Ͻÿ�

	//�Լ��� �����Ͻÿ�
public:
	virtual void Start(std::shared_ptr<CGameObject> gameObject)
	{
		Component::Start(gameObject);
		pPlayer = static_cast<CPlayer*>(m_gameObject.get());
		cPlayerState = pPlayer->GetPlayerState();

		if (cPlayerState->GetCharcaterName() == "MEI")
			m_fDuringStunTime = MEI_STUN_TIME;
		else
			m_fDuringStunTime = LIZZY_STUN_TIME;
	}

	void PlayerMove(float fTimeElapsed)
	{
		float fSpeed = cPlayerState->GetSpeed();
		bool bTwBar = (!CGameFramework::GetInstance()->IsTwBar());


		POINT ptOldCursorPos = INPUT->GetOldCursorPos();
		DWORD dwDirection = 0;

		//KYT '16.05.09
		/*
			Ÿ�� �ִϸ��̼ǽ�, input���� ���� ����.
		*/
		//if (cPlayerState->GetInputPossible())
			///*Ű������ ���� ������ ��ȯ�Ѵ�. ȭ��nǥ Ű(���桯, ���硯, ���衯, ���顯)�� ������ �÷��̾ ������/����(���� x-��), ��/��(���� z-��)�� �̵��Ѵ�. ��Page Up���� ��Page Down�� Ű�� ������ �÷��̾ ��/�Ʒ�(���� y-��)�� �̵��Ѵ�.*/
		if (INPUT->KeyDown(YK_W) || INPUT->KeyDown(YK_UP))    dwDirection |= DIR_FORWARD;
		if (INPUT->KeyDown(YK_A) || INPUT->KeyDown(YK_LEFT))  dwDirection |= DIR_LEFT;
		if (INPUT->KeyDown(YK_S) || INPUT->KeyDown(YK_DOWN))  dwDirection |= DIR_BACKWARD;
		if (INPUT->KeyDown(YK_D) || INPUT->KeyDown(YK_RIGHT)) dwDirection |= DIR_RIGHT;
		if (INPUT->KeyDown(YK_X)) dwDirection |= DIR_UP;
		if (INPUT->KeyDown(YK_Z)) dwDirection |= DIR_DOWN;


		//���������� ���� ��ǲ���°ǵ� �̰� ������Ʈ�� �־ �����ϴ°� ������;
		if (cPlayerState->IsAnimationInit())dwDirection = 0;

		static bool bMoveDy = false;
		static bool bMoveDx = false;
		float cxDelta = 0.0f, cyDelta = 0.0f;
		if (GetCapture() == g_hWnd && !bTwBar )
		{
			SetCursor(nullptr);
			POINT ptCursorPos;
			GetCursorPos(&ptCursorPos);
			cxDelta = (float)(ptCursorPos.x - ptOldCursorPos.x) / 3.0f;
			cyDelta = (float)(ptCursorPos.y - ptOldCursorPos.y) / 3.0f;
			SetCursorPos(ptOldCursorPos.x, ptOldCursorPos.y);
			if (!bMoveDx && !bMoveDy &&
				(abs(cyDelta) > 1.0f || abs(cxDelta) > 1.0f))
			{
				(abs(cyDelta) > abs(cxDelta)) ? bMoveDx = true : bMoveDy = true;
			}
		}
		else if ((INPUT->IsGamepadRightThumb_outsideDeadzone()))
		{
			cxDelta = (float)(INPUT->GetGamepad().sThumbRX / 32767.0f) * 3.0f;
			cyDelta = (float)(INPUT->GetGamepad().sThumbRY / 32767.0f) * 3.0f;

			if (!bMoveDx && !bMoveDy &&
				(abs(cyDelta) > 1.0f || abs(cxDelta) > 1.0f))
			{
				(abs(cyDelta) > abs(cxDelta)) ? bMoveDx = true : bMoveDy = true;
			}
		}

		else
		{
			bMoveDx = false;
			bMoveDy = false;
		}

		if (cxDelta || cyDelta)
		{
			if (false)
			{
				if (bMoveDx)
					pPlayer->Rotate(cyDelta, 0.0f, 0.0f);
				else if (bMoveDy)
					pPlayer->Rotate(0.0f, 0.0f, -cxDelta);
			}

			else if (INPUT->KeyDown(YK_LBUTTON) || INPUT->IsGamepadRightThumb_outsideDeadzone())
			{
				//JJY '16.04.01 plus
				/*ȸ������ ������ ������*/
				#ifdef _CONNECT_SERVER_
								if (bMoveDx)
								{
									//�Ӱ谢 �����Ͽ� 1.0f���� �ø� ������
									if (abs(cyDelta) >= 1.0f)
									{
										P_CS_INGAME_ROTATE_EVENT cs_ingame_rotate_event;
										cs_ingame_rotate_event.size = sizeof(P_CS_INGAME_ROTATE_EVENT);
										cs_ingame_rotate_event.type = CS_INGAME_ROTATE_EVENT;
										cs_ingame_rotate_event.cxDelta = 0.0f;
										cs_ingame_rotate_event.cyDelta = cyDelta;
										TCP->SendPacket((unsigned char*)&cs_ingame_rotate_event);

										INPUT->SetOldcyDelta(abs(cyDelta));
									}
								}
								else if (bMoveDy)
								{
									//�Ӱ谢 �����Ͽ� 1.0f���� �ø� ������
									if (abs(cxDelta) >= 1.0f)
									{
										P_CS_INGAME_ROTATE_EVENT cs_ingame_rotate_event;
										cs_ingame_rotate_event.size = sizeof(P_CS_INGAME_ROTATE_EVENT);
										cs_ingame_rotate_event.type = CS_INGAME_ROTATE_EVENT;
										cs_ingame_rotate_event.cxDelta = cxDelta;
										cs_ingame_rotate_event.cyDelta = 0.0f;
										TCP->SendPacket((unsigned char*)&cs_ingame_rotate_event);

										INPUT->SetOldcxDelta(abs(cxDelta));
									}
								}
				#else
				if (bMoveDx)
					pPlayer->Rotate(cyDelta, 0.0f, 0.0f);
				else if (bMoveDy)
					pPlayer->Rotate(0.0f, cxDelta, 0.0f);
#endif
			}
		}

		//JJY 16'.04.16 
		/*
			Ű �Է¿� ���� �÷��̾� ������ ����
		*/
		float fResizeSpeed = 1.0f;
		

		if (INPUT->KeyDown(YT_KEY::YK_LSHIFT))
		{
			::GBulrChange(YT_BlurMode::BLUR_RUN);
			#ifndef _CONNECT_SERVER_
				fResizeSpeed *= 10.0f;
			#endif
			pPlayer->SetAnimationSpeed(PLAYER_RUN_ANISPEED);
		}

		else
		{
			::GBulrChange(YT_BlurMode::BLUR_NOTHING);
			if (PLAYER_ANISPEED != pPlayer->GetAnimationSpeed())
				pPlayer->SetAnimationSpeed(PLAYER_ANISPEED);
		}

		#ifdef DEBUG_MODE_UI
			auto distance = fResizeSpeed * fSpeed * fTimeElapsed;
			FONTMGR->MessagePush("�⺻", 70, 30, 15, YT_Color(20, 20, 10), L"Player idx : %d | Distance : %1.3f", cPlayerState->GetID(), distance);
		#endif

		#ifndef _CONNECT_SERVER_
		if (dwDirection != 0)
			pPlayer->Move(dwDirection, fResizeSpeed * fSpeed * fTimeElapsed, false);

			#pragma region[Thrid Algorithm]
					{
						bool unMove = false;
						int nx, nz;
						nx = Helper::Rounding(pPlayer->GetPositionXMFLOAT3().x) / BLOCK_SCALE;
						nz = Helper::Rounding(pPlayer->GetPositionXMFLOAT3().z) / BLOCK_SCALE;

						if (nx > WORLDSIZE - 2) nx = WORLDSIZE - 2;
						if (nz > 158) nz = 160 - 2;

						//std::cout << "n(" << nx << "," << nz << ")"<< std::endl;
						BoundingBox objBound;
						BoundingBox playerBound = pPlayer->GetBoundingBox();
						objBound.Extents = XMFLOAT3(BLOCK_SCALE * 0.5f, BLOCK_SCALE * 0.5f, BLOCK_SCALE * 0.5f);

						//printf("Bound : (%1.1f, %1.1f)", playerBound.Center.y, playerBound.Extents.y);
						for (int z = 1; z >= -1; --z)
						{
							for (int x = 1; x >= -1; --x)
							{
								if (nx + x < 0 || nz + z < 0)continue;

								int ny = CONTENTMGR->GetMapArrary(nx + x, nz + z);
								if (unMove) x = z = -1;
								else if (ny == 1)
								{
									objBound.Center =
										XMFLOAT3(
										(nx + x) * BLOCK_SCALE + (BLOCK_SCALE * 0.1f),
											ny * BLOCK_SCALE + (BLOCK_SCALE * 0.5f),
											(nz + z) * BLOCK_SCALE + (BLOCK_SCALE * 0.1f)
										);
									unMove |= playerBound.Intersects(objBound);
								}
							}
						}
						//if (unMove) pPlayer->UnMove(dwDirection, fResizeSpeed * fSpeed * fTimeElapsed, false);
					}
			#pragma endregion

		#endif

		pPlayer->Update(fTimeElapsed);

		#ifndef _CONNECT_SERVER_
			cPlayerState->SetDirection(dwDirection);
		#endif

	}

	void HeadBangBang(float fTimeElapsed)
	{
		float fTime = cPlayerState->GetHeadBang();
		if (fTime > 0.0f)
		{
			cPlayerState->StartHeadBangBang(fTimeElapsed);

			if (fTime > m_fDuringStunTime)
				cPlayerState->StopHeadBangBang();
		}
	}

	//KYT '16.08.02
	/*
		Hidem SKill Script�� ���� ����
	*/
	virtual void OnPlayerUpdate(float fTimeElapsed)
	{

		if (false == cPlayerState->IsHeadBang())
		{
			PlayerComponet::PlayerMove(fTimeElapsed);
		}

		PlayerComponet::HeadBangBang(fTimeElapsed);
	}
	//KYT '16.04.24
	/*
	�浹üũ ����.
	���� �����ε� ���� �ʾƵ� �������ϴ�.
	*/
	virtual void Collision(std::shared_ptr<CGameObject> other, ObjectType type)
	{
		switch (type)
		{
		case ObjectType::StaticObject:
			#ifndef _CONNECT_SERVER_   
						if (other->tag == TAG_TYPE::TAG_ITEM)
						{
							m_gameObject->SetCompoent(move(other->MoveCompoent(m_gameObject, TAG_TYPE::TAG_ITEM)));
							other->StaticDestory(other);
							SOUNDMGR->Play("����", m_gameObject->GetPositionXMFLOAT3());
							return;
						}

						if (other->tag == TAG_TYPE::TAG_EAT_FAKE_ITEM)
						{
							m_gameObject->SetCompoent(move(other->MoveCompoent(m_gameObject, TAG_TYPE::TAG_ITEM)));
							other->StaticDestory(other);

							CScene::GetInstance()->AddTrap
							(
								m_gameObject->GetPositionXMFLOAT3()
								, static_cast<CPlayer*>(m_gameObject.get())
								, static_cast<PlayerState*>(m_gameObject->GetComponent(0))->GetID()
							);
						}
			#endif
			break;

		case ObjectType::DynamicObject:

			if (other->tag == TAG_TYPE::TAG_BULLET)
			{
				BulletComponent* bullet = (BulletComponent*)other->GetComponent(0);

				if (bullet)
				{
					if (cPlayerState->GetID() != bullet->GetId())
					{
						std::string name = other->name;

						if (name.find("FLASH") < name.size())
						{
							if(m_gameObject->name == "Player")
								::GBulrChange(YT_BlurMode::BLUR_FLASH);
						}
						else
							cPlayerState->StartHeadBangBang();
						other->DynamicDestory(other);
					}
				}
			}

			break;

			#ifndef _CONNECT_SERVER_
			case ObjectType::PlayerSmashBoundingBox:

				//RSH '16.07.18
				/*
					����÷��̾�� ���� �� Ŭ���̾�Ʈ�� �÷��̾�� �¾Ҵ�.
					�浹�� �Ͼ ��� ����÷��̾��� �̺�Ʈ �߻�
					1. �������� �ִϸ��̼� ������
					2. 
				*/
				if (other->GetCurrentAnimationState() != AnimState::ANI_SMASH)return;
				if (other->GetCurrentAnimationState() == AnimState::ANI_FALLDOWN) return;

				XMVECTOR vOtherLook = other->GetLook();
				XMVECTOR vPlayerLook = m_gameObject->GetLook();

				int keyFrame = other->GetCurrentKeyframe();

				if (20 <= keyFrame && keyFrame <= 30)
				{
					float mfBetweenAngle = GetBetweenAngle(vOtherLook, vPlayerLook);
					//std::cout << "Angle : " << angle << std::endl;

					//std::cout << "Hit : " << mfBetweenAngle << std::endl;
					//::GMessageBox("Hit?", to_string(mfBetweenAngle));
					if (mfBetweenAngle < m_fLimitBewteenAngle)
					{
						auto pos = m_gameObject->GetPositionXMFLOAT3();
						SOUNDMGR->Play("����", pos);
						cPlayerState->SetHp(0);
						PARTICLE->StartSprtieAnimation("���Ž�", pos.x, pos.y + 0.5f, pos.z, 1);	
					}
				}
				break;
			#endif
		}
	}


};