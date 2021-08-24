#include "stdafx.h"
#include "CameraManager.h"

#include "Camera.h"
#include "FirstPersonCamera.h"
#include "SpaceShipCamera.h"
#include "ThirdPersonCamera.h"
#include "FreeCamera.h"

#include "Player.h"

#include <iostream>
#include <vector>
#include <algorithm>

CCameraManager::CCameraManager()
{
	//m_pMainCamera = nullptr;
	m_vCamera.clear();
}


CCameraManager::~CCameraManager()
{
}

void CCameraManager::Load(ID3D11Device *pd3dDevice)
{
	auto pMainCamera = std::make_shared<CThirdPersonCamera>();
	pMainCamera->name = "MainCamera";
	pMainCamera->CreateConstBuffers(pd3dDevice);
	pMainCamera->GenerateViewMatrix();
	pMainCamera->OrthoCameraLoad(pd3dDevice);
	m_vCamera.push_back(pMainCamera);
}

void CCameraManager::ChangeCamera(std::shared_ptr<CCamera> pCamera, int index)
{
	if (pCamera)
	{
		if (m_vCamera[index]) m_vCamera[index] = pCamera;
		else ::GMessageBox(L"카메라가 없음", L"오류");
	}
}

void CCameraManager::SetData(CPlayer* pPlayer)
{
}

void CCameraManager::CreateCamera(ID3D11Device * pd3dDevice, DWORD nCameraMode)
{
	std::shared_ptr<CCamera> pNewCamera = nullptr;
	XMFLOAT3 d3dxvUp;
	float fPitch, fRoll, fYaw;

	switch (nCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		pNewCamera = std::make_shared<CFirstPersonCamera>(pNewCamera);
		break;
	case THIRD_PERSON_CAMERA:
		pNewCamera = std::make_shared<CThirdPersonCamera>(pNewCamera);
		break;
	case SPACESHIP_CAMERA:
		pNewCamera = std::make_shared<CSpaceShipCamera>(pNewCamera);
		break;
	case FREE_CAMERA:
		pNewCamera = std::make_shared<CFreeCamera>(pNewCamera);
		break;
	}

	pNewCamera->CreateConstBuffers(pd3dDevice);
	pNewCamera->GenerateViewMatrix();
	pNewCamera->OrthoCameraLoad(pd3dDevice);

	m_vCamera.push_back(pNewCamera);
}

std::shared_ptr<CCamera> CCameraManager::ChnageCamera(DWORD nCameraMode, int index)
{
	std::shared_ptr<CCamera> pNewCamera = nullptr;
	switch (nCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		pNewCamera = std::make_shared<CFirstPersonCamera>(GetCameara());
		break;
	case THIRD_PERSON_CAMERA:
		pNewCamera = std::make_shared<CThirdPersonCamera>(GetCameara());
		break;
	case SPACESHIP_CAMERA:
		pNewCamera = std::make_shared<CSpaceShipCamera>(GetCameara());
		break;
	case FREE_CAMERA:
		pNewCamera = std::make_shared<CFreeCamera>(GetCameara());
		break;
	}
	ChangeCamera(pNewCamera);

	return pNewCamera;
}


std::shared_ptr<CCamera> CCameraManager::CreateCamera
(
	ID3D11Device *pd3dDevice,
	DWORD nNewCameraMode, 
	DWORD nCurrentCameraMode,
	XMFLOAT3 d3dxvRight, 
	XMFLOAT3 d3dxvLook,
	std::shared_ptr<CCamera> pCamera
)

{
	std::shared_ptr<CCamera> pNewCamera = nullptr;
	XMFLOAT3 d3dxvUp;
	float fPitch, fRoll, fYaw;

	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		pNewCamera = std::make_shared<CFirstPersonCamera>(pCamera);
		break;
	case THIRD_PERSON_CAMERA:
		pNewCamera = std::make_shared<CThirdPersonCamera>(pCamera);
		break;
	case SPACESHIP_CAMERA:
		pNewCamera = std::make_shared<CSpaceShipCamera>(pCamera);
		break;
	case FREE_CAMERA:
		pNewCamera = std::make_shared<CFreeCamera>(pCamera);
		break;
	}
	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		d3dxvUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
		d3dxvRight.y = 0.0f;
		d3dxvLook.y = 0.0f;
		XMStoreFloat3(&d3dxvRight, XMVector3Normalize(XMLoadFloat3(&d3dxvRight)));
		 XMStoreFloat3(&d3dxvLook, XMVector3Normalize(XMLoadFloat3(&d3dxvLook)));
		fPitch = 0.0f;
		fRoll = 0.0f;

		D3DXVECTOR3 v3 = D3DXLoadFLOAT3(d3dxvLook);
		fYaw = (float)D3DXToDegree(acosf(D3DXVec3Dot(&D3DXVECTOR3(0.0f, 0.0f, 1.0f), &(v3))));
		//m_fYaw = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMLoadFloat3(&m_d3dxvLook)))));
		if (d3dxvLook.x < 0.0f) fYaw = -fYaw;
	}

	return(pNewCamera);
}


template<typename T>
std::shared_ptr<CCamera> CCameraManager::ChangeCamera(std::shared_ptr<CCamera> pOldCamera, bool isMainCamera)
{
	if (pOldCamera)
	{
		auto pCamera = std::make_shared<T>(pCamera);
		pCamera->name = pOldCamera->name;
		pCamera->GenerateViewMatrix();

		if (isMainCamera)
		{
			m_vCamera[0] = pCamera;
		}
		else
		{
			auto findCamera = std::find(m_vCamera.begin(), m_vCamera.end(), pCamera);
			m_vCamera.erase(findCamera);
			m_vCamera.push_back(pCamera);
			pOldCamera.reset();
		}
		return pCamera;
	}
	else
	{
		GMessageBox("ChangeCamera Error", "No Camera Instance");
		return nullptr;
	}
}

void CCameraManager::OnProcessingMouseMessage(UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CCameraManager::OnProcessingKeyboardMessage(UINT nMessageID, WPARAM wParam)
{
	if (nMessageID == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_F1:
		case VK_F2:
		case VK_F3:
			break;
		}
	}
}

void CCameraManager::ProcessInput(DWORD dwDirection, POINT ptOldCursorPos)
{
	//for (auto &pCamera : m_vCamera)
	//{
	//	if (pCamera->GetMode() == FREE_CAMERA)
	//	{
	//		static bool bMoveDy = false;
	//		static bool bMoveDx = false;
	//		float cxDelta = 0.0f, cyDelta = 0.0f;
	//		if (GetCapture() == g_hWnd)
	//		{
	//			SetCursor(nullptr);
	//			POINT ptCursorPos;
	//			GetCursorPos(&ptCursorPos);
	//			cxDelta = (float)(ptCursorPos.x - ptOldCursorPos.x) / 3.0f;
	//			cyDelta = (float)(ptCursorPos.y - ptOldCursorPos.y) / 3.0f;
	//			SetCursorPos(ptOldCursorPos.x, ptOldCursorPos.y);
	//			if (!bMoveDx && !bMoveDy &&
	//				(abs(cyDelta) > 1.0f || abs(cxDelta) > 1.0f))
	//				(abs(cyDelta) > abs(cxDelta)) ? bMoveDx = true : bMoveDy = true;
	//		}
	//		else
	//		{
	//			bMoveDx = false;
	//			bMoveDy = false;
	//		}
	//
	//		if (cxDelta || cyDelta)
	//		{
	//			if (GetKeyState(VK_RBUTTON) & 0xF0)
	//			{
	//				if (bMoveDx)
	//					pCamera->Rotate(cyDelta, 0.0f, 0.0f);
	//				else if (bMoveDy)
	//					pCamera->Rotate(0.0f, 0.0f, -cxDelta);
	//			}
	//			else if (GetKeyState(VK_LBUTTON) & 0xF0)
	//			{
	//				if (bMoveDx)
	//					pCamera->Rotate(cyDelta, 0.0f, 0.0f);
	//				else if (bMoveDy)
	//					pCamera->Rotate(0.0f, cxDelta, 0.0f);
	//			}
	//		}
	//
	//		if (dwDirection)
	//			m_pPlayer->Move(dwDirection, fSpeed * 150.0f  * TIMEMGR->GetTimeElapsed(), true);
	//
	//		m_pPlayer->CameraUpdate(TIMEMGR->GetTimeElapsed());
	//
	//	}
	//}
}

