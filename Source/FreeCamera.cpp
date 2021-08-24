#include "stdafx.h"
#include "FreeCamera.h"
#include "PlayerState.h"
CFreeCamera::CFreeCamera(std::shared_ptr<CCamera> pCamera) : CCamera(pCamera)
{
	m_nMode = SPACESHIP_CAMERA;
}

void CFreeCamera::Update(XMVECTOR& d3dxvLookAt, float fTimeElapsed)
{
}
/*
void CFreeCamera::Rotate(float x, float y, float z)
{
XMMATRIX mtxRotate;
XMFLOAT3 xmPosition;
if (x != 0.0f)
{
mtxRotate = XMMatrixRotationAxis(GetRightVector(), XMConvertToRadians(x));
XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
//XMStoreFloat3(&m_d3dxvPosition, XMVector3TransformCoord(GetPosition(), mtxRotate));
}

else if (y != 0.0f)
{
mtxRotate = XMMatrixRotationAxis(GetUpVector(), XMConvertToRadians(y));
XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
//XMStoreFloat3(&m_d3dxvPosition, XMVector3TransformCoord(GetPosition(), mtxRotate));

}
else if(z != 0.0f)
{
mtxRotate = XMMatrixRotationAxis(GetLookVector(), XMConvertToRadians(z));
XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
//XMStoreFloat3(&m_d3dxvPosition, XMVector3TransformCoord(GetPosition(), mtxRotate));
}
SetLookAt(GetPosition());
}
*/
void CFreeCamera::SetRotation(float x, float y, float z)
{
	//m_d3dxvRight = XMFLOAT3(1.0f, 0.0f, 0.0f);
	//m_d3dxvUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//m_d3dxvLook = XMFLOAT3(0.0f, 0.0f, 1.0f);
}

void CFreeCamera::SetLookAt(XMVECTOR& d3dxvLookAt)
{
	XMFLOAT4X4 mtxLookAt;
	XMVECTOR xmPosition = XMLoadFloat3(&m_d3dxvPosition);
	XMStoreFloat4x4(&mtxLookAt, XMMatrixLookAtLH(xmPosition, d3dxvLookAt, m_pPlayer->GetUpVector()));

	m_d3dxvRight.x = mtxLookAt._11;
	m_d3dxvRight.y = mtxLookAt._21;
	m_d3dxvRight.z = mtxLookAt._31;

	m_d3dxvUp.x = mtxLookAt._12;
	m_d3dxvUp.y = mtxLookAt._22;
	m_d3dxvUp.z = mtxLookAt._32;

	m_d3dxvLook.x = mtxLookAt._13;
	m_d3dxvLook.y = mtxLookAt._23;
	m_d3dxvLook.z = mtxLookAt._33;

}
