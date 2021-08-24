#include "stdafx.h"
#include "TerrainPlayer.h"
#include "HeightMapTerrain.h"
#include "ThirdPersonCamera.h"

#include "SpaceShipCamera.h"
#include "FirstPersonCamera.h"
#include "ThirdPersonCamera.h"
#include "FreeCamera.h"

#include "PlayerState.h"
#include "PlayerComponet.h"
#include "PlayerAnimation.h"

#include "InstancedObjectsShader.h"

#include "QuadrangleMesh.h"
#include "ShadowMapShader.h"
#include "BulletShader.h"

#include "FBXSingleMesh.h"

//Componet
#include "BulletComponent.h"

#include "ItemThrow.h"


CTerrainPlayer::CTerrainPlayer(int nMeshes) : CPlayer(nMeshes)
{
	m_pCamera = nullptr;
	SetCompoent(std::make_unique<PlayerComponet>());
	SetCompoent(std::make_unique<PlayerAnimation>());
}

CTerrainPlayer::~CTerrainPlayer()
{
	if (m_pCamera) m_pCamera.reset();
}

void CTerrainPlayer::Release()
{
	CPlayer::Release();
	if (m_pCamera) m_pCamera.reset();
}

void CTerrainPlayer::ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return;
	
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		SetFriction(250.0f);
		SetGravity(XMVectorSet(0.0f, -400.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMVectorSet(0.0f, 20.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;
	case SPACESHIP_CAMERA:
		SetFriction(125.0f);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, SPACESHIP_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;
		//chagne Gravity
	case THIRD_PERSON_CAMERA:
		SetFriction(250.0f);

		#ifdef _CONNECT_SERVER_
			SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		#else
			SetGravity(XMFLOAT3(0.0f, -9.8f, 0.0f));
		#endif
		//SetGravity(XMFLOAT3(0.0f, -9.8f, 0.0f));

		//---------------------------------------------------
		//임시로 중력 끔
		//---------------------------------------------------
		//SetGravity(XMFLOAT3(0.0f, -9.8f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetRotation(19.0f, 0.0f, 0.0f);

		//JJY '16.04.28
		/*
			플레이어 카메라 수정
		*/
		//m_pCamera->SetOffset(XMVectorSet(-0.4f, 1.17f, -4.0f, 0.0f));
		m_pCamera->SetOffset(XMVectorSet(-0.4f, 0.17f, -2.5f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;

	case FREE_CAMERA:
		SetFriction(125.0f);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, FREE_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;
	default:
		break;
	}

	//Update(fTimeElapsed);
}

void CTerrainPlayer::OnPlayerUpdated(float fTimeElapsed)
{
	//KYT '16.04.28
	/*
		캐릭터 y값 수정
	*/
	//---------------------------------------------------
	//임시로 중력 끔
	//---------------------------------------------------
	#ifndef _CONNECT_SERVER_
		XMFLOAT3 pos = GetPositionXMFLOAT3();
		if (pos.y < BLOCK_SCALE * 5.0f)
		{
			//RSH '16.07.16
			/*
				캐릭터 변경에 따른 캐릭터 높이 변경 BLOCK_SCALE*1.6f => BLOCK_SCALE*1.0f
				7월 17일에 서버 개발자 귀환시, 보고.
			*/
			pos.y = BLOCK_SCALE;
			//m_d3dxvPosition.y = BLOCK_SCALE;
			SetPosition(XMLoadFloat3(&pos));
		}
	#endif
	//---------------------------------------------------
	
	//CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pPlayerUpdatedContext;
	//XMFLOAT3 d3dxvScale = pTerrain->GetScaleXMFLOAT3();
	//XMFLOAT3 d3dxvPlayerPosition;
	//XMStoreFloat3(&d3dxvPlayerPosition, GetPosition());
	//int z = (int)(d3dxvPlayerPosition.z / (d3dxvScale.z));
	//bool bReverseQuad = ((z % 2) != 0);
	//float fHeight = pTerrain->GetHeight(d3dxvPlayerPosition.x, d3dxvPlayerPosition.z, bReverseQuad) + 6.0f;
	//if (d3dxvPlayerPosition.y < fHeight)
	//{
	//	XMFLOAT3 d3dxvPlayerVelocity;
	//	XMStoreFloat3(&d3dxvPlayerVelocity, GetVelocity());
	//	d3dxvPlayerVelocity.y = 0.0f;
	//	SetVelocity(XMLoadFloat3(&d3dxvPlayerVelocity));
	//	d3dxvPlayerPosition.y = fHeight;
	//	SetPosition(XMLoadFloat3(&d3dxvPlayerPosition));
	//}
}

void CTerrainPlayer::OnCameraUpdated(float fTimeElapsed)
{
	//XMFLOAT3 pos = m_pCamera->GetPositionXMFLOAT3();
	//if (pos.y < CHARACTER_DEFAULT_HEIGHT)
	//{
	//	XMFLOAT3 d3dxvPlayerVelocity;
	//	XMStoreFloat3(&d3dxvPlayerVelocity, GetVelocity());
	//	d3dxvPlayerVelocity.y = 0.0f;
	//	SetVelocity(XMLoadFloat3(&d3dxvPlayerVelocity));
	//	pos.y = CHARACTER_DEFAULT_HEIGHT;
	//	m_pCamera->SetPosition(pos);
	//}

	//CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pCameraUpdatedContext;
	//XMFLOAT3 d3dxvScale;
	//XMStoreFloat3(&d3dxvScale, pTerrain->GetScale());
	//XMFLOAT3 d3dxvCameraPosition;
	////XMStoreFloat3(&d3dxvCameraPosition, m_pCamera->GetPosition());
	//d3dxvCameraPosition = m_pCamera->GetPositionXMFLOAT3();
	//int z = (int)(d3dxvCameraPosition.z / d3dxvScale.z);
	//bool bReverseQuad = ((z % 2) != 0);
	//float fHeight = pTerrain->GetHeight(d3dxvCameraPosition.x, d3dxvCameraPosition.z, bReverseQuad) + 5.0f;
	//
	//if (d3dxvCameraPosition.y <= fHeight)
	//{
	//	d3dxvCameraPosition.y = fHeight;
	//	m_pCamera->SetPosition(d3dxvCameraPosition);
	//	//카메라 위치로 셋룩업하더라구..
	//
	//}
}

void CTerrainPlayer::UpdateConstBuffers(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pCamera) m_pCamera->UpdateConstBuffers(pd3dDeviceContext);
}

void CTerrainPlayer::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
	
		XMVECTOR d3dxvShift = XMVectorZero();
		if (dwDirection & DIR_FORWARD) d3dxvShift += XMLoadFloat3(&m_d3dxvLook) * fDistance;
		if (dwDirection & DIR_BACKWARD) d3dxvShift -= XMLoadFloat3(&m_d3dxvLook) * fDistance;
		if (dwDirection & DIR_RIGHT) d3dxvShift += XMLoadFloat3(&m_d3dxvRight) * fDistance;
		if (dwDirection & DIR_LEFT) d3dxvShift -= XMLoadFloat3(&m_d3dxvRight) * fDistance;
		if (dwDirection & DIR_UP) d3dxvShift += XMLoadFloat3(&m_d3dxvUp) * fDistance;
		if (dwDirection & DIR_DOWN) d3dxvShift -= XMLoadFloat3(&m_d3dxvUp) * fDistance;

		Move(d3dxvShift, bUpdateVelocity);

	}
}

void CTerrainPlayer::Move(float fxOffset, float fyOffset, float fzOffset)
{
	CPlayer::Move(fxOffset, fyOffset, fzOffset);
}

void CTerrainPlayer::Move(const XMVECTOR& d3dxvShift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)
	{
		XMStoreFloat3(&m_d3dxvVelocity, XMLoadFloat3(&m_d3dxvVelocity) + d3dxvShift);
	}
	else
	{
		m_pCamera->Move(d3dxvShift);
		if (m_pCamera->GetMode() == FREE_CAMERA)return;
		XMVECTOR d3dxvPosition = XMLoadFloat3(&m_d3dxvPosition);
		d3dxvPosition = d3dxvPosition + d3dxvShift;
		XMStoreFloat3(&m_d3dxvPosition, d3dxvPosition);
	}
}

void CTerrainPlayer::SetPosition(float x, float z)
{
	XMFLOAT3 f3 = GetPositionXMFLOAT3();
	f3.x = x; f3.z = z;
	Move((XMLoadFloat3(&f3) - GetPosition()), false);
}

void CTerrainPlayer::SetPosition(const XMVECTOR& d3dxvPosition)
{
	XMVECTOR v = XMLoadFloat3(&m_d3dxvPosition);
	Move((d3dxvPosition - v), false);
}

void CTerrainPlayer::SetPosition(const XMFLOAT3& d3dxvPosition)
{
	XMVECTOR v = XMLoadFloat3(&m_d3dxvPosition);
	Move((XMLoadFloat3(&d3dxvPosition) - v), false);
}

void CTerrainPlayer::SetPositionNotMove(const XMFLOAT3& d3dxvPosition)
{
	XMVECTOR v = XMLoadFloat3(&m_d3dxvPosition);
	m_pCamera->Move((XMLoadFloat3(&d3dxvPosition) - v));
	if (m_pCamera->GetMode() == FREE_CAMERA)return;
	m_d3dxvPosition = d3dxvPosition;
}

void CTerrainPlayer::Rotate(float x, float y, float z)
{
	XMMATRIX mtxRotate;
	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) || (nCurrentCameraMode == THIRD_PERSON_CAMERA))
	{
		if (x != 0.0f)
		{
			m_fPitch += x;
			if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
			if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		}
		if (y != 0.0f)
		{
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		if (z != 0.0f)
		{
			m_fRoll += z;
			if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}
		m_pCamera->Rotate(x, y, z);

		if (y != 0.0f)
		{
			mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_d3dxvUp), XMConvertToRadians(y));
			XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
			XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		}
	}
	else if (nCurrentCameraMode == SPACESHIP_CAMERA || (nCurrentCameraMode == FREE_CAMERA))
	{
		m_pCamera->Rotate(x, y, z);
		if (x != 0.0f)
		{
			mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_d3dxvRight), XMConvertToRadians(x));
			XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
			XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
		}
		if (y != 0.0f)
		{
			mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_d3dxvUp), XMConvertToRadians(y));
			XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
			XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		}
		if (z != 0.0f)
		{
			mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_d3dxvLook), XMConvertToRadians(z));
			XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
			XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		}
	}


	XMStoreFloat3(&m_d3dxvLook, XMVector3Normalize(XMLoadFloat3(&m_d3dxvLook)));
	XMStoreFloat3(&m_d3dxvRight, XMVector3Cross(XMLoadFloat3(&m_d3dxvUp), XMLoadFloat3(&m_d3dxvLook)));
	XMStoreFloat3(&m_d3dxvRight, XMVector3Normalize(XMLoadFloat3(&m_d3dxvRight)));
	XMStoreFloat3(&m_d3dxvUp, XMVector3Cross(XMLoadFloat3(&m_d3dxvLook), XMLoadFloat3(&m_d3dxvRight)));
	XMStoreFloat3(&m_d3dxvUp, XMVector3Normalize(XMLoadFloat3(&m_d3dxvUp)));
}

void CTerrainPlayer::Update(float fTimeElapsed)
{
	DWORD nCurrentCameraMode = m_pCamera->GetMode();

	if (nCurrentCameraMode == THIRD_PERSON_CAMERA || nCurrentCameraMode == FREE_CAMERA)
	{
		m_pCamera->Update(fTimeElapsed);
		m_pCamera->SetLookAt(XMLoadFloat3(&m_d3dxvPosition));
	}

	if(m_pCamera) m_pCamera->RegenerateViewMatrix();

	//RSH '16.05.21
	/*
		등짝때리는 범위박스를 업데이트합니다.
	*/
}

void CTerrainPlayer::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	if (m_pCamera->GetMode() == FIRST_PERSON_CAMERA) return;
	
	CPlayer::Render(pd3dDeviceContext, pCamera);

	#ifdef DEBUG_MODE_UI
		auto matrix = GetWorldMatrixFLOAT();
		FONTMGR->MessagePush("기본", 655, 15, 25, YT_Color(0, 0, 0), L"WorldMatrix \n (%1.f, %1.f, %1.f)", matrix._41, matrix._42, matrix._43);
	#endif
}

std::shared_ptr<CCamera> CTerrainPlayer::OnChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	std::shared_ptr<CCamera> pNewCamera = nullptr;

	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		pNewCamera = std::make_shared<CFirstPersonCamera>(m_pCamera);
		break;
	case SPACESHIP_CAMERA:
		pNewCamera = std::make_shared<CSpaceShipCamera>(m_pCamera);
		break;
	case THIRD_PERSON_CAMERA:
		pNewCamera = std::make_shared<CThirdPersonCamera>(m_pCamera);
		break;
	case FREE_CAMERA:
		pNewCamera = std::make_shared<CFreeCamera>(m_pCamera);
		break;
	}

	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) {}

	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_d3dxvUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_d3dxvRight.y = 0.0f;
		m_d3dxvLook.y = 0.0f;
		XMStoreFloat3(&m_d3dxvRight, XMVector3Normalize(XMLoadFloat3(&m_d3dxvRight)));
		XMStoreFloat3(&m_d3dxvLook, XMVector3Normalize(XMLoadFloat3(&m_d3dxvLook)));
		m_fPitch = 0.0f;
		m_fRoll = 0.0f;

		D3DXVECTOR3 v3 = D3DXLoadFLOAT3(m_d3dxvLook);
		m_fYaw = (float)D3DXToDegree(acosf(D3DXVec3Dot(&D3DXVECTOR3(0.0f, 0.0f, 1.0f), &(v3))));
		//m_fYaw = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMLoadFloat3(&m_d3dxvLook)))));
		if (m_d3dxvLook.x < 0.0f) m_fYaw = -m_fYaw;
	}

	if (pNewCamera)
	{
		pNewCamera->SetMode(nNewCameraMode);
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) m_pCamera.reset();


	//KYT '16.07.09
	/*
		Player State
	*/
	//PlayerState* state = (PlayerState*)GetComponent(0);
	//bool mode = true;
	//if (nNewCameraMode == FREE_CAMERA) mode = false;
	//if (state)state->SetFreeCamera(mode);

	return(pNewCamera);
}
