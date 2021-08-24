#include "stdafx.h"
#include "SpaceShipCamera.h"


CSpaceShipCamera::CSpaceShipCamera(std::shared_ptr<CCamera> pCamera) : CCamera(pCamera)
{
	m_nMode = SPACESHIP_CAMERA;
}

void CSpaceShipCamera::Rotate(float x, float y, float z)
{
	XMMATRIX mtxRotate;
	XMFLOAT3 xmPosition;
	if (m_pPlayer && (x != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetRightVector(), XMConvertToRadians(x));
		XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
		XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));

		XMStoreFloat3(&xmPosition, m_pPlayer->GetPosition());
		m_d3dxvPosition.x -= xmPosition.x;
		m_d3dxvPosition.y -= xmPosition.y;
		m_d3dxvPosition.z -= xmPosition.z;
		XMStoreFloat3(&m_d3dxvPosition, XMVector3TransformCoord(XMLoadFloat3(&m_d3dxvPosition), mtxRotate));
		m_d3dxvPosition.x += xmPosition.x;
		m_d3dxvPosition.y += xmPosition.y;
		m_d3dxvPosition.z += xmPosition.z;
	}
	if (m_pPlayer && (y != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetUpVector(), XMConvertToRadians(y));
		XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
		XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));

		XMStoreFloat3(&xmPosition, m_pPlayer->GetPosition());
		m_d3dxvPosition.x -= xmPosition.x;
		m_d3dxvPosition.y -= xmPosition.y;
		m_d3dxvPosition.z -= xmPosition.z;
		XMStoreFloat3(&m_d3dxvPosition, XMVector3TransformCoord(XMLoadFloat3(&m_d3dxvPosition), mtxRotate));
		m_d3dxvPosition.x += xmPosition.x;
		m_d3dxvPosition.y += xmPosition.y;
		m_d3dxvPosition.z += xmPosition.z;
	}
	if (m_pPlayer && (z != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetLookVector(), XMConvertToRadians(z));
		XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
		XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));

		XMStoreFloat3(&xmPosition, m_pPlayer->GetPosition());
		m_d3dxvPosition.x -= xmPosition.x;
		m_d3dxvPosition.y -= xmPosition.y;
		m_d3dxvPosition.z -= xmPosition.z;
		XMStoreFloat3(&m_d3dxvPosition, XMVector3TransformCoord(XMLoadFloat3(&m_d3dxvPosition), mtxRotate));
		m_d3dxvPosition.x += xmPosition.x;
		m_d3dxvPosition.y += xmPosition.y;
		m_d3dxvPosition.z += xmPosition.z;
	}
}
