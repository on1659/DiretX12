#include "stdafx.h"
#include "FirstPersonCamera.h"

// CFirstPersonCamera
CFirstPersonCamera::CFirstPersonCamera(std::shared_ptr<CCamera> pCamera) : CCamera(pCamera)
{
	m_nMode = FIRST_PERSON_CAMERA;
	if (pCamera)
	{
		if (pCamera->GetMode() == SPACESHIP_CAMERA)
		{
			//XMStoreFloat3(&m_d3dxvUp, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			m_d3dxvUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
			m_d3dxvRight.y = 0.0f;
			m_d3dxvLook.y = 0.0f;

			XMVECTOR rightNormal = XMVector3Normalize(XMLoadFloat3(&m_d3dxvRight));
			XMVECTOR lookNormal = XMVector3Normalize(XMLoadFloat3(&m_d3dxvLook));
			XMStoreFloat3(&m_d3dxvRight, rightNormal);
			XMStoreFloat3(&m_d3dxvLook, lookNormal);

		}
	}
}

void CFirstPersonCamera::Rotate(float x, float y, float z)
{
	XMMATRIX mtxRotate;
	XMVECTOR xmPosition;
	if (x != 0.0f)
	{
		mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_d3dxvRight), XMConvertToRadians(x));
		XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
		XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
	}
	if (m_pPlayer && (y != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetUpVector(), XMConvertToRadians(y));
		XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
		XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
	}
	if (m_pPlayer && (z != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetLookVector(), XMConvertToRadians(z));
		xmPosition = XMLoadFloat3(&m_d3dxvPosition);

		xmPosition -= m_pPlayer->GetPosition();
		XMVector3TransformCoord(xmPosition, mtxRotate);
		xmPosition += m_pPlayer->GetPosition();

		XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
		XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
	}
}


