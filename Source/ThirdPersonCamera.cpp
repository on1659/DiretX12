#include "stdafx.h"
#include "ThirdPersonCamera.h"

// CThirdPersonCamera

CThirdPersonCamera::CThirdPersonCamera(std::shared_ptr<CCamera> pCamera) : CCamera(pCamera)
{
	m_nMode = THIRD_PERSON_CAMERA;
	if (pCamera)
	{
		if (pCamera->GetMode() == SPACESHIP_CAMERA)
		{
			m_d3dxvUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
			//XMStoreFloat3(&m_d3dxvUp, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			m_d3dxvRight.y = 0.0f;
			m_d3dxvLook.y = 0.0f;
			XMVECTOR rightNormal = XMVector3Normalize(XMLoadFloat3(&m_d3dxvRight));
			XMVECTOR lookNormal = XMVector3Normalize(XMLoadFloat3(&m_d3dxvLook));
			XMStoreFloat3(&m_d3dxvRight, rightNormal);
			XMStoreFloat3(&m_d3dxvLook, lookNormal);
		}
	}

}

void CThirdPersonCamera::Update(XMVECTOR& d3dxvLookAt, float fTimeElapsed)
{
	if (m_pPlayer)
	{
		XMFLOAT4X4 mtxRotate;
		XMStoreFloat4x4(&mtxRotate, XMMatrixIdentity());
		XMFLOAT3 d3dxvRight = m_pPlayer->GetRightVectorXMFLOAT3();
		XMFLOAT3 d3dxvUp = m_pPlayer->GetUpVectorXMFLOAT3();
		XMFLOAT3 d3dxvLook = m_pPlayer->GetLookVectorXMFLOAT3();

		mtxRotate._11 = d3dxvRight.x;
		mtxRotate._21 = d3dxvUp.x;
		mtxRotate._31 = d3dxvLook.x;

		mtxRotate._12 = d3dxvRight.y;
		mtxRotate._22 = d3dxvUp.y;
		mtxRotate._32 = d3dxvLook.y;

		mtxRotate._13 = d3dxvRight.z;
		mtxRotate._23 = d3dxvUp.z;
		mtxRotate._33 = d3dxvLook.z;

		XMVECTOR d3dxvOffset = XMLoadFloat3(&m_d3dxvOffset);
		XMMATRIX xmMatrix = XMLoadFloat4x4(&mtxRotate);
		XMVECTOR position = XMLoadFloat3(&m_d3dxvPosition);

		d3dxvOffset = XMVector3TransformCoord(d3dxvOffset, xmMatrix);
		XMVECTOR d3dxvPosition = m_pPlayer->GetPosition() + d3dxvOffset;
		XMVECTOR d3dxvDirection = d3dxvPosition - position;

		//최적화를 위해서 변수 하나를 사용하였지만 여기서는 플레이어와 카메라의 거리임
		D3DXVECTOR3 v3 = XMLoadD3DXVECTOR3(d3dxvDirection);
		XMFLOAT3 xm3Legth;
		XMVECTOR xmLength = XMVector3Length(d3dxvDirection);
		XMStoreFloat3(&xm3Legth, xmLength);
		float fLength = xm3Legth.x;

		d3dxvDirection = XMVector3Normalize(d3dxvDirection);
		float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;
		float fDistance = fLength * fTimeLagScale;

		//KYT '16.02.15 
		/*
		어찌 되든 같은 값임
		*/
		//if (fDistance > fLength) fDistance = fLength;
		//if (fLength < 0.01f) 	fDistance = fLength;
		fDistance = fLength;
		if (fDistance > 0)
		{
			//position = XMVectorAdd(position, (d3dxvDirection * fDistance));
			XMStoreFloat3(&m_d3dxvPosition, position + (d3dxvDirection * fDistance));
			SetLookAt(d3dxvLookAt);
		}
	}
}

void CThirdPersonCamera::Update(XMVECTOR& d3dxvLookAt, CPlayer* player, float fTimeElapsed)
{
	if (player)
	{
		XMFLOAT4X4 mtxRotate;
		XMStoreFloat4x4(&mtxRotate, XMMatrixIdentity());
		XMFLOAT3 d3dxvRight = player->GetRightVectorXMFLOAT3();
		XMFLOAT3 d3dxvUp = player->GetUpVectorXMFLOAT3();
		XMFLOAT3 d3dxvLook = player->GetLookVectorXMFLOAT3();

		mtxRotate._11 = d3dxvRight.x;
		mtxRotate._21 = d3dxvUp.x;
		mtxRotate._31 = d3dxvLook.x;

		mtxRotate._12 = d3dxvRight.y;
		mtxRotate._22 = d3dxvUp.y;
		mtxRotate._32 = d3dxvLook.y;

		mtxRotate._13 = d3dxvRight.z;
		mtxRotate._23 = d3dxvUp.z;
		mtxRotate._33 = d3dxvLook.z;

		XMVECTOR d3dxvOffset = XMLoadFloat3(&m_d3dxvOffset);
		XMMATRIX xmMatrix = XMLoadFloat4x4(&mtxRotate);
		XMVECTOR position = XMLoadFloat3(&m_d3dxvPosition);

		d3dxvOffset = XMVector3TransformCoord(d3dxvOffset, xmMatrix);
		XMVECTOR d3dxvPosition = player->GetPosition() + d3dxvOffset;
		XMVECTOR d3dxvDirection = d3dxvPosition - position;

		//최적화를 위해서 변수 하나를 사용하였지만 여기서는 플레이어와 카메라의 거리임
		D3DXVECTOR3 v3 = XMLoadD3DXVECTOR3(d3dxvDirection);
		XMFLOAT3 xm3Legth;
		XMVECTOR xmLength = XMVector3Length(d3dxvDirection);
		XMStoreFloat3(&xm3Legth, xmLength);
		float fLength = xm3Legth.x;

		d3dxvDirection = XMVector3Normalize(d3dxvDirection);
		float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;
		float fDistance = fLength * fTimeLagScale;

		//KYT '16.02.15 
		/*
		어찌 되든 같은 값임
		*/
		//if (fDistance > fLength) fDistance = fLength;
		//if (fLength < 0.01f) 	fDistance = fLength;
		fDistance = fLength;
		if (fDistance > 0)
		{
			//position = XMVectorAdd(position, (d3dxvDirection * fDistance));
			XMStoreFloat3(&m_d3dxvPosition, position + (d3dxvDirection * fDistance));
			SetLookAt(d3dxvLookAt);
		}
	}
}

void CThirdPersonCamera::Update(CPlayer* player, float fTimeElapsed)
{
	if (player)
	{
		XMFLOAT4X4 mtxRotate;
		XMStoreFloat4x4(&mtxRotate, XMMatrixIdentity());
		XMFLOAT3 d3dxvRight = player->GetRightVectorXMFLOAT3();
		XMFLOAT3 d3dxvUp = player->GetUpVectorXMFLOAT3();
		XMFLOAT3 d3dxvLook = player->GetLookVectorXMFLOAT3();

		mtxRotate._11 = d3dxvRight.x;
		mtxRotate._21 = d3dxvUp.x;
		mtxRotate._31 = d3dxvLook.x;

		mtxRotate._12 = d3dxvRight.y;
		mtxRotate._22 = d3dxvUp.y;
		mtxRotate._32 = d3dxvLook.y;

		mtxRotate._13 = d3dxvRight.z;
		mtxRotate._23 = d3dxvUp.z;
		mtxRotate._33 = d3dxvLook.z;

		XMVECTOR d3dxvOffset = XMLoadFloat3(&m_d3dxvOffset);
		XMMATRIX xmMatrix = XMLoadFloat4x4(&mtxRotate);
		XMVECTOR position = XMLoadFloat3(&m_d3dxvPosition);

		d3dxvOffset = XMVector3TransformCoord(d3dxvOffset, xmMatrix);
		XMVECTOR d3dxvPosition = player->GetPosition() + d3dxvOffset;
		XMVECTOR d3dxvDirection = d3dxvPosition - position;

		//최적화를 위해서 변수 하나를 사용하였지만 여기서는 플레이어와 카메라의 거리임
		D3DXVECTOR3 v3 = XMLoadD3DXVECTOR3(d3dxvDirection);
		XMFLOAT3 xm3Legth;
		XMVECTOR xmLength = XMVector3Length(d3dxvDirection);
		XMStoreFloat3(&xm3Legth, xmLength);
		float fLength = xm3Legth.x;

		d3dxvDirection = XMVector3Normalize(d3dxvDirection);
		float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;
		float fDistance = fLength * fTimeLagScale;

		//KYT '16.02.15 
		/*
		어찌 되든 같은 값임
		*/
		//if (fDistance > fLength) fDistance = fLength;
		//if (fLength < 0.01f) 	fDistance = fLength;
		fDistance = fLength;
		if (fDistance > 0)
		{
			//position = XMVectorAdd(position, (d3dxvDirection * fDistance));
			XMStoreFloat3(&m_d3dxvPosition, position + (d3dxvDirection * fDistance));
			SetLookAt(player->GetPosition());
		}
	}
}

void CThirdPersonCamera::Update(float fTimeElapsed)
{
	if (m_pPlayer)
	{
		XMFLOAT4X4 mtxRotate;
		XMStoreFloat4x4(&mtxRotate, XMMatrixIdentity());
		XMFLOAT3 d3dxvRight = m_pPlayer->GetRightVectorXMFLOAT3();
		XMFLOAT3 d3dxvUp = m_pPlayer->GetUpVectorXMFLOAT3();
		XMFLOAT3 d3dxvLook = m_pPlayer->GetLookVectorXMFLOAT3();

		mtxRotate._11 = d3dxvRight.x;
		mtxRotate._21 = d3dxvUp.x;
		mtxRotate._31 = d3dxvLook.x;

		mtxRotate._12 = d3dxvRight.y;
		mtxRotate._22 = d3dxvUp.y;
		mtxRotate._32 = d3dxvLook.y;

		mtxRotate._13 = d3dxvRight.z;
		mtxRotate._23 = d3dxvUp.z;
		mtxRotate._33 = d3dxvLook.z;

		XMVECTOR d3dxvOffset = XMLoadFloat3(&m_d3dxvOffset);
		XMMATRIX xmMatrix = XMLoadFloat4x4(&mtxRotate);
		XMVECTOR position = XMLoadFloat3(&m_d3dxvPosition);

		d3dxvOffset = XMVector3TransformCoord(d3dxvOffset, xmMatrix);
		XMVECTOR d3dxvPosition = m_pPlayer->GetPosition() + d3dxvOffset;
		XMVECTOR d3dxvDirection = d3dxvPosition - position;

		//최적화를 위해서 변수 하나를 사용하였지만 여기서는 플레이어와 카메라의 거리임
		D3DXVECTOR3 v3 = XMLoadD3DXVECTOR3(d3dxvDirection);
		XMFLOAT3 xm3Legth;
		XMVECTOR xmLength = XMVector3Length(d3dxvDirection);
		XMStoreFloat3(&xm3Legth, xmLength);
		float fLength = xm3Legth.x;

		d3dxvDirection = XMVector3Normalize(d3dxvDirection);
		float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;
		float fDistance = fLength * fTimeLagScale;

		//KYT '16.02.15 
		/*
		어찌 되든 같은 값임
		*/
		//if (fDistance > fLength) fDistance = fLength;
		//if (fLength < 0.01f) 	fDistance = fLength;
		fDistance = fLength;
		if (fDistance > 0)
		{
			//position = XMVectorAdd(position, (d3dxvDirection * fDistance));
			XMStoreFloat3(&m_d3dxvPosition, position + (d3dxvDirection * fDistance));
			SetLookAt(m_pPlayer->GetPosition());
		}
	}
}


//üũ
void CThirdPersonCamera::SetLookAt(XMVECTOR& d3dxvLookAt)
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

void CThirdPersonCamera::SetLookAt()
{
	XMVECTOR& d3dxvLookAt = m_pPlayer->GetPosition();

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

