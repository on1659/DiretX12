#include "stdafx.h"
#include "Player.h"
#include "Camera.h"

ID3D11Buffer *CCamera::m_pd3dcbCamera = nullptr;
ID3D11Buffer *CCamera::m_pd3dPScbCamera = nullptr;
ID3D11Buffer *CCamera::m_pd3dcShaodwbCamera = nullptr;
ID3D11Buffer *CCamera::m_pd3dcbCameraOrtho = nullptr;
//지오매투리!
ID3D11Buffer *CCamera::m_pd3dGScbCamera = nullptr;

CCamera::CCamera(std::shared_ptr<CCamera> pCamera)
{
	if (pCamera)
	{
		m_d3dxvPosition = pCamera->GetPositionXMFLOAT3();
		m_d3dxvRight = pCamera->GetRightVectorXMFLOAT3();
		m_d3dxvLook = pCamera->GetLookVectorXMFLOAT3();
		m_d3dxvUp = pCamera->GetUpVectorXMFLOAT3();
		m_fPitch = pCamera->GetPitch();
		m_fRoll = pCamera->GetRoll();
		m_fYaw = pCamera->GetYaw();
		m_d3dxmtxView = pCamera->GetViewMatrixXMFLOAT4X4();
		m_d3dxmtxProjection = pCamera->GetProjectionMatrixXMFLOAT4X4();
		m_d3dViewport = pCamera->GetViewport();
		m_d3dxvOffset = pCamera->GetOffsetXMFLOAT3();
		m_fTimeLag = pCamera->GetTimeLag();
		m_pPlayer = pCamera->GetPlayer();

		//Shadow
		m_d3dxmtxShadowView = pCamera->GetShadowViewMatrixXMFLOAT4X4();
		//m_d3dxmtxShadowProjection = pCamera->GetShadowProjectionMatrixXMFLOAT4X4();
	}

	else
	{
		m_d3dxvPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_d3dxvRight = XMFLOAT3(1.0f, 0.0f, 0.0f);
		m_d3dxvLook = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_d3dxvUp = XMFLOAT3(0.0f, 0.0f, 1.0f);
		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		m_fYaw = 0.0f;
		m_fTimeLag = 0.0f;
		m_d3dxvOffset = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_nMode = 0x00;
		m_pPlayer = nullptr;
		XMStoreFloat4x4(&m_d3dxmtxView, XMMatrixIdentity());
		XMStoreFloat4x4(&m_d3dxmtxProjection, XMMatrixIdentity());

		XMStoreFloat4x4(&m_d3dxmtxShadowView, XMMatrixIdentity());
		//XMStoreFloat4x4(&m_d3dxmtxShadowProjection, XMMatrixIdentity());

	}

	m_xmf4Shadows = nullptr;
	m_xmf4Shadows = new XMMATRIX[1];

	//imsi
	Helper::RuntimeDataModify(&xf3_RegenerateViewMatrix, "xf3_RegenerateViewMatrix");
}

CCamera::~CCamera()
{
	if (m_xmf4Shadows)delete m_xmf4Shadows;
	m_xmf4Shadows = nullptr;
}

void CCamera::SetViewport(ID3D11DeviceContext *pd3dDeviceContext, DWORD xTopLeft, DWORD yTopLeft, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ)
{
	m_d3dViewport.TopLeftX = float(xTopLeft);
	m_d3dViewport.TopLeftY = float(yTopLeft);
	m_d3dViewport.Width = float(nWidth);
	m_d3dViewport.Height = float(nHeight);
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;
	if (pd3dDeviceContext) pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);
}

void CCamera::SetViewport(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (pd3dDeviceContext) pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);
}

void CCamera::SetLookAt(XMVECTOR& d3dxvPosition, XMVECTOR& d3dxvLookAt, XMVECTOR& d3dxvUp)
{
	XMFLOAT4X4 mtxLookAt;
	XMStoreFloat4x4(&mtxLookAt, XMMatrixLookToLH(d3dxvPosition, d3dxvLookAt, d3dxvUp));
	m_d3dxvRight = XMFLOAT3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
	m_d3dxvUp = XMFLOAT3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
	m_d3dxvLook = XMFLOAT3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
}

void CCamera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	XMStoreFloat4x4(&m_d3dxmtxProjection,
		XMMatrixPerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance));

	XMStoreFloat4x4(&m_d3dxmtxShadowProjection,
		XMMatrixPerspectiveFovLH(XMConvertToRadians(60), WIDTH/ HEIGHT, 1, 1000));
}

void CCamera::GenerateViewMatrix()
{
	if (m_pPlayer) XMStoreFloat4x4(&m_d3dxmtxView, XMMatrixLookAtLH(XMLoadFloat3(&m_d3dxvPosition), m_pPlayer->GetPosition(), XMLoadFloat3(&m_d3dxvUp)));
	XMStoreFloat4x4(&m_d3dxmtxView, XMMatrixLookAtLH(XMLoadFloat3(&m_d3dxvPosition), XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMLoadFloat3(&m_d3dxvUp)));
}

void CCamera::RegenerateViewMatrix()
{
	//JJY '16.04.18
	/*
	카메라 수정(1.8)
	*/

	//RSH '16.07.16
	/*
	카메라 수정(2.0)
	*/


	float t = m_d3dxvPosition.y;
	m_d3dxvPosition.y *= 2.0f;



	XMStoreFloat3(&m_d3dxvLook, XMVector3Normalize(XMLoadFloat3(&m_d3dxvLook)));
	XMStoreFloat3(&m_d3dxvRight, XMVector3Cross(XMLoadFloat3(&m_d3dxvUp), XMLoadFloat3(&m_d3dxvLook)));
	XMStoreFloat3(&m_d3dxvRight, XMVector3Normalize(XMLoadFloat3(&m_d3dxvRight)));
	XMStoreFloat3(&m_d3dxvUp, XMVector3Cross(XMLoadFloat3(&m_d3dxvLook), XMLoadFloat3(&m_d3dxvRight)));
	XMStoreFloat3(&m_d3dxvUp, XMVector3Normalize(XMLoadFloat3(&m_d3dxvUp)));

	m_d3dxmtxView._11 = m_d3dxvRight.x;
	m_d3dxmtxView._12 = m_d3dxvUp.x;
	m_d3dxmtxView._13 = m_d3dxvLook.x;

	m_d3dxmtxView._21 = m_d3dxvRight.y;
	m_d3dxmtxView._22 = m_d3dxvUp.y;
	m_d3dxmtxView._23 = m_d3dxvLook.y;

	m_d3dxmtxView._31 = m_d3dxvRight.z;
	m_d3dxmtxView._32 = m_d3dxvUp.z;
	m_d3dxmtxView._33 = m_d3dxvLook.z;

	XMFLOAT2 temp_xmfloat3;
	XMStoreFloat2(&temp_xmfloat3, XMVector3Dot(XMLoadFloat3(&m_d3dxvPosition), XMLoadFloat3(&m_d3dxvRight)));
	m_d3dxmtxView._41 = -temp_xmfloat3.x;

	XMStoreFloat2(&temp_xmfloat3, XMVector3Dot(XMLoadFloat3(&m_d3dxvPosition), XMLoadFloat3(&m_d3dxvUp)));
	m_d3dxmtxView._42 = -temp_xmfloat3.x;

	XMStoreFloat2(&temp_xmfloat3, XMVector3Dot(XMLoadFloat3(&m_d3dxvPosition), XMLoadFloat3(&m_d3dxvLook)));
	m_d3dxmtxView._43 = -temp_xmfloat3.x;

	//KYT '16.04.18
	/*
	카메라 수정
	*/
	m_d3dxvPosition.y = t;
	CalculateFrustumPlanes();
}

void CCamera::CreateConstBuffers(ID3D11Device* pd3dDevice)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_CAMERA);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bd, nullptr, &m_pd3dcbCamera);
	pd3dDevice->CreateBuffer(&bd, nullptr, &m_pd3dGScbCamera);
	


	pd3dDevice->CreateBuffer(&bd, nullptr, &m_pd3dcShaodwbCamera);

	Radar::DXUT_SetDebugName(m_pd3dcbCamera, ("VS_CAMERA_VIEW_PROJ"));
	Radar::DXUT_SetDebugName(m_pd3dGScbCamera, ("GS_CAMERA_VIEW_PROJ"));
	Radar::DXUT_SetDebugName(m_pd3dcShaodwbCamera, ("SHADOW_CAMERA_VIEW_PROJ"));

}

void CCamera::ReleaseConstBuffers()
{
	if (m_pd3dcbCamera) m_pd3dcbCamera->Release();
	if (m_pd3dGScbCamera) m_pd3dGScbCamera->Release();
	if (m_pd3dPScbCamera) m_pd3dPScbCamera->Release();
	if (m_pd3dcbCameraOrtho) m_pd3dcbCameraOrtho->Release();
	if (m_pd3dcShaodwbCamera)m_pd3dcShaodwbCamera->Release();
}

void CCamera::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX& pd3dxmtxView, XMMATRIX& pd3dxmtxProjection)
{
	{
		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
		pd3dDeviceContext->Map(m_pd3dcbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		VS_CB_CAMERA *pcbViewProjection = (VS_CB_CAMERA *)d3dMappedResource.pData;

		XMStoreFloat4x4(&(pcbViewProjection->m_d3dmtxViewProjection), XMMatrixTranspose(XMMatrixMultiply(pd3dxmtxView, pd3dxmtxProjection)));
		pd3dDeviceContext->Unmap(m_pd3dcbCamera, 0);

		pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_CAMERA, 1, &m_pd3dcbCamera);
		pd3dDeviceContext->GSSetConstantBuffers(GS_CB_SLOT_CAMERA, 1, &m_pd3dcbCamera);//카메라 슬롯에 슬롯 1개만큼, m_pd3dcdCamera로 갱신.
		pd3dDeviceContext->DSSetConstantBuffers(DS_CB_SLOT_CAMERA, 1, &m_pd3dcbCamera);//카메라 슬롯에 슬롯 1개만큼, m_pd3dcdCamera로 갱신.
	}
}

void CCamera::UpdateConstBuffers(ID3D11DeviceContext *pd3dDeviceContext)
{
	UpdateConstBuffer(pd3dDeviceContext, XMLoadFloat4x4(&m_d3dxmtxView), XMLoadFloat4x4(&m_d3dxmtxProjection));

	//UpdateShadowConstBufferUpdate(pd3dDeviceContext);
}

void CCamera::CalculateFrustumPlanes()
{
	XMMATRIX mtxViewProjection = XMLoadFloat4x4(&m_d3dxmtxView) * XMLoadFloat4x4(&m_d3dxmtxProjection);

	XMFLOAT4X4 mtx;
	XMStoreFloat4x4(&mtx, mtxViewProjection);

	m_pd3dxFrustumPlanes[0].x = -(mtx._14 + mtx._11);
	m_pd3dxFrustumPlanes[0].y = -(mtx._24 + mtx._21);
	m_pd3dxFrustumPlanes[0].z = -(mtx._34 + mtx._31);
	m_pd3dxFrustumPlanes[0].w = -(mtx._44 + mtx._41);

	m_pd3dxFrustumPlanes[1].x = -(mtx._14 - mtx._11);
	m_pd3dxFrustumPlanes[1].y = -(mtx._24 - mtx._21);
	m_pd3dxFrustumPlanes[1].z = -(mtx._34 - mtx._31);
	m_pd3dxFrustumPlanes[1].w = -(mtx._44 - mtx._41);

	m_pd3dxFrustumPlanes[2].x = -(mtx._14 - mtx._12);
	m_pd3dxFrustumPlanes[2].y = -(mtx._24 - mtx._22);
	m_pd3dxFrustumPlanes[2].z = -(mtx._34 - mtx._32);
	m_pd3dxFrustumPlanes[2].w = -(mtx._44 - mtx._42);

	m_pd3dxFrustumPlanes[3].x = -(mtx._14 + mtx._12);
	m_pd3dxFrustumPlanes[3].y = -(mtx._24 + mtx._22);
	m_pd3dxFrustumPlanes[3].z = -(mtx._34 + mtx._32);
	m_pd3dxFrustumPlanes[3].w = -(mtx._44 + mtx._42);

	m_pd3dxFrustumPlanes[4].x = -(mtx._13);
	m_pd3dxFrustumPlanes[4].y = -(mtx._23);
	m_pd3dxFrustumPlanes[4].z = -(mtx._33);
	m_pd3dxFrustumPlanes[4].w = -(mtx._43);


	//D3DXVECTOR3 normal;
	//D3DXVec3Normalize(&normal, &(D3DXVECTOR3(mtx._31, mtx._32, mtx._33)));
	//::gotoxy(0, 3);
	//std::cout << "                                     ";
	//::gotoxy(0, 3);
	//std::cout << normal.x << "," << normal.y << "," << normal.z << std::endl;

	m_pd3dxFrustumPlanes[5].x = -(mtx._14 - mtx._13);
	m_pd3dxFrustumPlanes[5].y = -(mtx._24 - mtx._23);
	m_pd3dxFrustumPlanes[5].z = -(mtx._34 - mtx._33);
	m_pd3dxFrustumPlanes[5].w = -(mtx._44 - mtx._43);

	for (int i = 0; i < 6; i++)
		XMPlaneNormalize(XMLoadFloat4(&m_pd3dxFrustumPlanes[i]));
}

#ifdef _AABB_
bool CCamera::IsInFrustum(XMFLOAT3& d3dxvMinimum, XMFLOAT3& d3dxvMaximum)
{
	XMFLOAT3 d3dxvNearPoint, d3dxvFarPoint, d3dxvNormal;
	XMFLOAT3 d3dxvTempMax, d3dxvTempMin;

	d3dxvTempMax = d3dxvMaximum;
	d3dxvTempMin = d3dxvMinimum;

	for (int i = 0; i < 6; i++)
	{
		XMStoreFloat3(&d3dxvNormal, XMVectorSet(m_pd3dxFrustumPlanes[i].x, m_pd3dxFrustumPlanes[i].y, m_pd3dxFrustumPlanes[i].z, 0.0f));

		if (d3dxvNormal.x >= 0.0f)
		{
			if (d3dxvNormal.y >= 0.0f)
			{
				if (d3dxvNormal.z >= 0.0f)
				{
					d3dxvNearPoint.x = d3dxvTempMin.x; d3dxvNearPoint.y = d3dxvTempMin.y; d3dxvNearPoint.z = d3dxvTempMin.z;
				}
				else
				{
					d3dxvNearPoint.x = d3dxvTempMin.x; d3dxvNearPoint.y = d3dxvTempMin.y; d3dxvNearPoint.z = d3dxvTempMax.z;
				}
			}
			else
			{
				if (d3dxvNormal.z >= 0.0f)
				{
					d3dxvNearPoint.x = d3dxvTempMin.x; d3dxvNearPoint.y = d3dxvTempMax.y; d3dxvNearPoint.z = d3dxvTempMin.z;
				}
				else
				{
					d3dxvNearPoint.x = d3dxvTempMin.x; d3dxvNearPoint.y = d3dxvTempMax.y; d3dxvNearPoint.z = d3dxvTempMax.z;
				}
			}
		}
		else
		{
			if (d3dxvNormal.y >= 0.0f)
			{
				if (d3dxvNormal.z >= 0.0f)
				{
					d3dxvNearPoint.x = d3dxvTempMax.x; d3dxvNearPoint.y = d3dxvTempMin.y; d3dxvNearPoint.z = d3dxvTempMin.z;
				}
				else
				{
					d3dxvNearPoint.x = d3dxvTempMax.x; d3dxvNearPoint.y = d3dxvTempMin.y; d3dxvNearPoint.z = d3dxvTempMax.z;
				}
			}
			else
			{
				if (d3dxvNormal.z >= 0.0f)
				{
					d3dxvNearPoint.x = d3dxvTempMax.x; d3dxvNearPoint.y = d3dxvTempMax.y; d3dxvNearPoint.z = d3dxvTempMin.z;
				}
				else
				{
					d3dxvNearPoint.x = d3dxvTempMax.x; d3dxvNearPoint.y = d3dxvTempMax.y; d3dxvNearPoint.z = d3dxvTempMax.z;
				}
			}
		}

		XMFLOAT4 plane;//	palne.y = 0.0f;
		XMVECTOR dot = XMPlaneDotCoord(XMLoadFloat4(&m_pd3dxFrustumPlanes[i]), XMLoadFloat3(&d3dxvNearPoint));
		XMStoreFloat4(&plane, dot);
		if (plane.x > 0.0f) return false;
	}
	return true;
}

bool CCamera::IsInFrustum(AABB *pAABB)
{
	return (IsInFrustum(pAABB->m_d3dxvMinimum, pAABB->m_d3dxvMaximum));
}

bool CCamera::IsInFrustumQuad(AABB *pAABB)
{
	return (IsInFrustumQuad(pAABB->m_d3dxvMinimum, pAABB->m_d3dxvMaximum));
}

bool CCamera::IsInFrustum2D(XMFLOAT3 fCenter)
{
	int xNum = (int)(fCenter.x / m_xmSpaceSize.x);
	int zNum = (int)(fCenter.z / m_xmSpaceSize.z);

	if (xNum >= m_nDepth)xNum = m_nDepth - 1;
	if (zNum >= m_nDepth)zNum = m_nDepth - 1;
	if (xNum  < 0)xNum = 0;
	if (zNum  < 0)zNum = 0;


	return true;
}


bool CCamera::IsInFrustumQuad(XMFLOAT3& d3dxvMinimum, XMFLOAT3& d3dxvMaximum)
{
	int x = ((d3dxvMinimum.x + d3dxvMaximum.x) * 0.5f);
	int z = ((d3dxvMinimum.z + d3dxvMaximum.z) * 0.5f);

	if (abs(m_d3dxvPosition.x - x) > CULL_CAMERA_DISTANCE * 1.5f)return false;
	if (abs(m_d3dxvPosition.z - z) > CULL_CAMERA_DISTANCE * 1.5f)return false;

	XMFLOAT3 d3dxvNearPoint, d3dxvFarPoint, d3dxvNormal;
	XMFLOAT3 d3dxvTempMax, d3dxvTempMin;

	d3dxvTempMax = d3dxvMaximum;
	d3dxvTempMin = d3dxvMinimum;

	for (int i = 0; i < 6; i++)
	{
		XMStoreFloat3(&d3dxvNormal, XMVectorSet(m_pd3dxFrustumPlanes[i].x, m_pd3dxFrustumPlanes[i].y, m_pd3dxFrustumPlanes[i].z, 0.0f));

		if (d3dxvNormal.x >= 0.0f)
		{
			if (d3dxvNormal.z >= 0.0f)
			{
				d3dxvNearPoint.x = d3dxvTempMin.x;
				d3dxvNearPoint.z = d3dxvTempMin.z;
			}
			else
			{
				d3dxvNearPoint.x = d3dxvTempMin.x;
				d3dxvNearPoint.z = d3dxvTempMax.z;
			}
		}
		else
		{
			{
				if (d3dxvNormal.z >= 0.0f)
				{
					d3dxvNearPoint.x = d3dxvTempMax.x;
					d3dxvNearPoint.z = d3dxvTempMin.z;
				}
				else
				{
					d3dxvNearPoint.x = d3dxvTempMax.x;
					d3dxvNearPoint.z = d3dxvTempMax.z;
				}
			}
		}

		//d3dxvNearPoint.y = 0;						//기존의 코드
		d3dxvNearPoint.y = GetPositionXMFLOAT3().y; //향상된 코드
		XMFLOAT4 plane = m_pd3dxFrustumPlanes[i];
		XMVECTOR dot = XMPlaneDotCoord(XMLoadFloat4(&plane), XMLoadFloat3(&d3dxvNearPoint));
		XMStoreFloat4(&plane, dot);
		if (plane.x > 0.0f) return false;
	}
	return true;
}

#else

bool CCamera::IsInFrustum(XMVECTOR& xCenter, XMVECTOR& xExtern)
{
	XMFLOAT3 xmVectorNearPoint, xmVectorFarPoint, xmVectorNormal;
	XMFLOAT3 xmVectorTempMax, xmVectorTempMin;

	XMStoreFloat3(&xmVectorTempMin, XMVectorSubtract(xCenter, xExtern));
	XMStoreFloat3(&xmVectorTempMax, XMVectorAdd(xCenter, xExtern));

	for (int i = 0; i < 6; i++)
	{
		XMStoreFloat3(&xmVectorNormal, XMVectorSet(m_pd3dxFrustumPlanes[i].x, m_pd3dxFrustumPlanes[i].y, m_pd3dxFrustumPlanes[i].z, 0.0f));

		if (xmVectorNormal.x >= 0.0f)
		{
			if (xmVectorNormal.y >= 0.0f)
			{
				if (xmVectorNormal.z >= 0.0f)
				{
					xmVectorNearPoint.x = xmVectorTempMin.x; xmVectorNearPoint.y = xmVectorTempMin.y; xmVectorNearPoint.z = xmVectorTempMin.z;
				}
				else
				{
					xmVectorNearPoint.x = xmVectorTempMin.x; xmVectorNearPoint.y = xmVectorTempMin.y; xmVectorNearPoint.z = xmVectorTempMax.z;
				}
			}
			else
			{
				if (xmVectorNormal.z >= 0.0f)
				{
					xmVectorNearPoint.x = xmVectorTempMin.x; xmVectorNearPoint.y = xmVectorTempMax.y; xmVectorNearPoint.z = xmVectorTempMin.z;
				}
				else
				{
					xmVectorNearPoint.x = xmVectorTempMin.x; xmVectorNearPoint.y = xmVectorTempMax.y; xmVectorNearPoint.z = xmVectorTempMax.z;
				}
			}
		}
		else
		{
			if (xmVectorNormal.y >= 0.0f)
			{
				if (xmVectorNormal.z >= 0.0f)
				{
					xmVectorNearPoint.x = xmVectorTempMax.x; xmVectorNearPoint.y = xmVectorTempMin.y; xmVectorNearPoint.z = xmVectorTempMin.z;
				}
				else
				{
					xmVectorNearPoint.x = xmVectorTempMax.x; xmVectorNearPoint.y = xmVectorTempMin.y; xmVectorNearPoint.z = xmVectorTempMax.z;
				}
			}
			else
			{
				if (xmVectorNormal.z >= 0.0f)
				{
					xmVectorNearPoint.x = xmVectorTempMax.x; xmVectorNearPoint.y = xmVectorTempMax.y; xmVectorNearPoint.z = xmVectorTempMin.z;
				}
				else
				{
					xmVectorNearPoint.x = xmVectorTempMax.x; xmVectorNearPoint.y = xmVectorTempMax.y; xmVectorNearPoint.z = xmVectorTempMax.z;
				}
			}
		}
		//KYT '16.02.07 memo
		/*XMVectorGetX 극혐*/
		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_pd3dxFrustumPlanes[i]), XMLoadFloat3(&xmVectorNearPoint))) > 0.0f) return false;
	}
	return true;
}

bool CCamera::IsInFrustum(BoundingBox *boundingbox)
{
	return (IsInFrustum(XMLoadFloat3(&boundingbox->Center), XMLoadFloat3(&boundingbox->Extents)));
}
#endif

//--------------------------------------직교투영 JJY
void CCamera::OrthoCameraLoad(ID3D11Device* pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext)
{
	XMStoreFloat4x4(&m_d3dxmtxOrthoView, XMMatrixIdentity());
	XMStoreFloat4x4(&m_d3dxmtxOrtho, XMMatrixIdentity());


	//CCamera::OrthoCreateShaderVariables(pd3dDevice);
	CCamera::OrthoGenerateProjectionMatrix();

	if (pd3dDeviceContext)
		CCamera::UpdateShaderVariablesOrtho(pd3dDeviceContext);
	else
	{
		ID3D11DeviceContext* pd3dDeviceContext;
		pd3dDevice->GetImmediateContext(&pd3dDeviceContext);
		CCamera::UpdateShaderVariablesOrtho(pd3dDeviceContext);
	}
}

void CCamera::OrthoCreateConstBuffer(ID3D11Device* pd3dDevice)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_CAMERA_ORTHO);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bd, nullptr, &m_pd3dcbCameraOrtho);
}

void CCamera::OrthoGenerateProjectionMatrix()
{
	XMStoreFloat4x4(&m_d3dxmtxOrtho, XMMatrixOrthographicLH(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f));
}

void CCamera::UpdateShaderVariableOrtho(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX& pd3dxmtxView, XMMATRIX& pd3dxmtxProjection)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbCameraOrtho, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_CAMERA_ORTHO *pcbViewProjection = (VS_CB_CAMERA_ORTHO *)d3dMappedResource.pData;
	XMStoreFloat4x4(&(pcbViewProjection->m_d3dxmtxOrthoViewProjection), XMMatrixTranspose(XMMatrixMultiply(pd3dxmtxView, pd3dxmtxProjection)));
	pd3dDeviceContext->Unmap(m_pd3dcbCameraOrtho, 0);

	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_CAMERA_ORTHO, 1, &m_pd3dcbCameraOrtho);

#ifdef _WITH_GEOMETRY_SHADER_SHADOW
	pd3dDeviceContext->GSSetConstantBuffers(VS_CB_SLOT_CAMERA_ORTHO, 1, &m_pd3dcbCameraOrtho);
#endif

}

void CCamera::UpdateShaderVariablesOrtho(ID3D11DeviceContext *pd3dDeviceContext)
{
	UpdateShaderVariableOrtho(pd3dDeviceContext, XMLoadFloat4x4(&m_d3dxmtxOrthoView), XMLoadFloat4x4(&m_d3dxmtxOrtho));
}


void CCamera::UpdateShadowConstBufferUpdate(ID3D11DeviceContext * pd3dDeviceContext)
{
	//*m_xmf4Shadows = XMMatrixTranspose(XMMatrixMultiply(XMLoadFloat4x4(&m_d3dxmtxShadowView), XMLoadFloat4x4(&m_d3dxmtxShadowProjection)));
	//pd3dDeviceContext->UpdateSubresource(m_pd3dcShaodwbCamera, 0, NULL, m_xmf4Shadows, 0, 0);

	//return;
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	/*상수 버퍼의 메모리 주소를 가져와서 카메라 변환 행렬과 투영 변환 행렬을 복사한다. 쉐이더에서 행렬의 행과 열이 바뀌는 것에 주의하라.*/
	pd3dDeviceContext->Map(m_pd3dcShaodwbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_CAMERA *pcbViewProjection = (VS_CB_CAMERA *)d3dMappedResource.pData;
	
	XMStoreFloat4x4(&(pcbViewProjection->m_d3dmtxViewProjection), XMMatrixTranspose(XMMatrixMultiply(XMLoadFloat4x4(&m_d3dxmtxShadowView), XMLoadFloat4x4(&m_d3dxmtxOrtho))));
	
	pd3dDeviceContext->Unmap(m_pd3dcShaodwbCamera, 0);
	
	//상수 버퍼를 슬롯(VS_SLOT_CAMERA)에 설정한다.
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_SHADOW, 1, &m_pd3dcShaodwbCamera);
	pd3dDeviceContext->VSSetConstantBuffers(PS_CB_SLOT_SHADOW, 1, &m_pd3dcShaodwbCamera);


	return;
	XMFLOAT4X4 xf44;
	
	::gotoxy(0, 0);
	XMStoreFloat4x4(&xf44, XMMatrixMultiply(XMLoadFloat4x4(&m_d3dxmtxShadowView), XMLoadFloat4x4(&m_d3dxmtxOrtho)));
	//xf44 = pcbViewProjection->m_d3dmtxViewProjection;
	std::cout << "---------------------Shadows View * Proj ------------------------------" << std::endl;
	std::cout << "( " << (float)xf44._11 << "," << (float)xf44._12 << "," << (float)xf44._13 << "," << (float)xf44._14 << ")" << std::endl;
	std::cout << "( " << (float)xf44._21 << "," << (float)xf44._22 << "," << (float)xf44._23 << "," << (float)xf44._24 << ")" << std::endl;
	std::cout << "( " << (float)xf44._31 << "," << (float)xf44._32 << "," << (float)xf44._33 << "," << (float)xf44._34 << ")" << std::endl;
	std::cout << "( " << (float)xf44._41 << "," << (float)xf44._42 << "," << (float)xf44._43 << "," << (float)xf44._44 << ")" << std::endl;
	std::cout << "-------------------------------------------------------------" << std::endl;

	xf44 = m_d3dxmtxShadowView;
	std::cout << "---------------------Shadows View------------------------------" << std::endl;
	std::cout << "( " << (float)xf44._11 << "," << (float)xf44._12 << "," << (float)xf44._13 << "," << (float)xf44._14 << ")" << std::endl;
	std::cout << "( " << (float)xf44._21 << "," << (float)xf44._22 << "," << (float)xf44._23 << "," << (float)xf44._24 << ")" << std::endl;
	std::cout << "( " << (float)xf44._31 << "," << (float)xf44._32 << "," << (float)xf44._33 << "," << (float)xf44._34 << ")" << std::endl;
	std::cout << "( " << (float)xf44._41 << "," << (float)xf44._42 << "," << (float)xf44._43 << "," << (float)xf44._44 << ")" << std::endl;
	std::cout << "-------------------------------------------------------------" << std::endl;

	xf44 = m_d3dxmtxView;
	std::cout << "---------------------Camera View------------------------------" << std::endl;
	std::cout << "( " << (float)xf44._11 << "," << (float)xf44._12 << "," << (float)xf44._13 << "," << (float)xf44._14 << ")" << std::endl;
	std::cout << "( " << (float)xf44._21 << "," << (float)xf44._22 << "," << (float)xf44._23 << "," << (float)xf44._24 << ")" << std::endl;
	std::cout << "( " << (float)xf44._31 << "," << (float)xf44._32 << "," << (float)xf44._33 << "," << (float)xf44._34 << ")" << std::endl;
	std::cout << "( " << (float)xf44._41 << "," << (float)xf44._42 << "," << (float)xf44._43 << "," << (float)xf44._44 << ")" << std::endl;
	std::cout << "-------------------------------------------------------------" << std::endl;


	xf44 = m_d3dxmtxProjection;
	std::cout << "---------------------Projection ------------------------------" << std::endl;
	std::cout << "( " << (float)xf44._11 << "," << (float)xf44._12 << "," << (float)xf44._13 << "," << (float)xf44._14 << ")" << std::endl;
	std::cout << "( " << (float)xf44._21 << "," << (float)xf44._22 << "," << (float)xf44._23 << "," << (float)xf44._24 << ")" << std::endl;
	std::cout << "( " << (float)xf44._31 << "," << (float)xf44._32 << "," << (float)xf44._33 << "," << (float)xf44._34 << ")" << std::endl;
	std::cout << "( " << (float)xf44._41 << "," << (float)xf44._42 << "," << (float)xf44._43 << "," << (float)xf44._44 << ")" << std::endl;
	std::cout << "-------------------------------------------------------------" << std::endl;


}

void CCamera::CreateShadowBuffer(const XMFLOAT3& position, const XMFLOAT3& up)
{
	XMStoreFloat4x4(&m_d3dxmtxShadowView, XMMatrixLookAtLH(XMVectorSet(position.x, position.y, position.z, 1.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(up.x, up.y, up.z, 1.0f)));
}

//--------------------------------------직교투영 JJY
