//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Player.h"
#include "HeightMapTerrain.h"

#include "SpaceShipCamera.h"
#include "FirstPersonCamera.h"
#include "ThirdPersonCamera.h"
#include "FreeCamera.h"

#include "PlayerState.h"
#include "PlayerComponet.h"
#include "InstancedObjectsShader.h"

#include "QuadrangleMesh.h"
#include "ShadowMapShader.h"
#include "BulletShader.h"
#include "FBXSingleMesh.h"
#include "BulletinShader.h"
#include "Warp3DModelManager.h"

//Componet
#include "BulletComponent.h"
#include "PlayerHeadBangBang.h"

#include "ItemThrow.h"

#include "HeadBangBangShader.h"
#include "PlayerAnimation.h"
#include "CubeParticleShader.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPlayer

CPlayer::CPlayer(int nMeshes) : CGameObject(nMeshes)
{

	//m_d3dxvPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvPosition = XMFLOAT3(0.0f, BLOCK_SCALE, 0.0f);
	m_d3dxvRight = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_d3dxvUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_d3dxvLook = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_d3dxvVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvGravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	m_pPlayerUpdatedContext = nullptr;
	m_pCameraUpdatedContext = nullptr;

	m_bHovering = false;

	m_bcMeshBoundingCube.m_d3dxvMaximum = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
	m_bcMeshBoundingCube.m_d3dxvMinimum = { +FLT_MAX, +FLT_MAX, +FLT_MAX };

	m_pCubeShader.reset();


	//Default Component
	SetCompoent(move(std::make_unique<PlayerState>()));
	m_cPlayerState = static_cast<PlayerState*>(m_vComponent[0].get());


	//Head
	m_bHeadMove = true;
}

CPlayer::~CPlayer()
{
	m_cPlayerState = nullptr;

	if (nullptr != m_pPlayerUpdatedContext)
	{
		delete m_pPlayerUpdatedContext;
		m_pPlayerUpdatedContext = nullptr;
	}
	if (nullptr != m_pCameraUpdatedContext)
	{
		delete m_pCameraUpdatedContext;
		m_pCameraUpdatedContext = nullptr;
	}
}

void CPlayer::Release()
{
	if (m_pCubeShader)m_pCubeShader->ReleaseObjects();
	if (m_pShadowShader)m_pShadowShader->ReleaseObjects();
	if (m_pHeadBangBangShader)m_pHeadBangBangShader->ReleaseObjects();

}

void CPlayer::CreateConstBuffers(ID3D11Device *pd3dDevice){}

void CPlayer::UpdateConstBuffers(ID3D11DeviceContext *pd3dDeviceContext){}


//KYT '16.05.08
/*
	Player Cube View - Hide시 생기는 큐브
*/

void CPlayer::CreateHeadUpCube(ID3D11Device* pd3dDevice)
{

	// 깊이 스텐실 상태 설정
	//D3D11_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	//ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	//d3dDepthStencilDesc.DepthEnable			= true;
	//d3dDepthStencilDesc.DepthWriteMask		= D3D11_DEPTH_WRITE_MASK_ALL;
	//d3dDepthStencilDesc.DepthFunc			= D3D11_COMPARISON_ALWAYS;
	//d3dDepthStencilDesc.StencilEnable		= false;
	//pd3dDevice->CreateDepthStencilState(&d3dDepthStencilDesc, &m_pd3dDepthStencilState);
	

	//CubeShader
	m_pCubeShader = std::make_unique<CInstancedObjectsShader>();
	m_pCubeShader->CreateShader(pd3dDevice);
	m_pCubeShader->SetMesh(std::move(std::make_unique<Default_Mesh>(pd3dDevice, 0.2f, 0.2f, 0.2f)));
	//m_pCubeShader->SetMaterial(); //재질에 따라서 바뀜
	m_pCubeShader->BuildObjects(pd3dDevice);

	std::shared_ptr<CGameObject> pGameObject = std::make_shared<CGameObject>();
	XMFLOAT3 pos = GetPositionXMFLOAT3();
	pos.y += 0.5f;
	pGameObject->SetPosition(pos);
	m_pCubeShader->AddObject(pGameObject);


	//Shadow
	m_pShadowShader = std::make_unique <CShadowMapShader>();
	m_pShadowShader->CreateShader(pd3dDevice);
	m_pShadowShader->SetMesh(move(std::make_unique<CQuadrangleMesh>(pd3dDevice, 0.4, 0.4)));
	m_pShadowShader->SetMaterial(pd3dDevice, "../Assets/02_ModelData/04_Others/character_shadow_texture.png");

	pos = GetPositionXMFLOAT3();
	m_pShadowShader->SetFirstPosition(pos.x, 1, pos.z);
	m_pShadowShader->AddObject(pGameObject);
	m_pShadowShader->BuildObjects(pd3dDevice, nullptr);
	m_pShadowShader->SetBlendState("Default");

	//HeadBangBang
	m_pHeadBangBangShader = std::make_unique<CHeadBangBangShader>();
	m_pHeadBangBangShader->CreateShader(pd3dDevice);
	m_pHeadBangBangShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/01_MapData/Map/Star.warp3d")));
	m_pHeadBangBangShader->SetPlayer(this);
	m_pHeadBangBangShader->BuildObjects(pd3dDevice);

	std::shared_ptr<CGameObject> headObject = std::make_shared<CGameObject>();
	auto script = std::make_unique<PlayerHeadBangBang>();
	script->SetPlayer(this);
	script->SetGameObject(headObject);

	headObject->SetPosition(GetPositionXMFLOAT3());
	this->SetCompoent(move(script));
	m_pHeadBangBangShader->AddObject(headObject);

}

void CPlayer::UnMove(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	fDistance *= -1;

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

void CPlayer::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
		m_d3dxvLook  = XMFLOAT3(0, 0, 1);
		m_d3dxvRight = XMFLOAT3(1, 0, 0);
		m_d3dxvUp    = XMFLOAT3(0, 1, 0);

		XMVECTOR d3dxvShift = XMVectorZero();
		if (dwDirection & DIR_FORWARD)	d3dxvShift += XMLoadFloat3(&m_d3dxvLook) * fDistance;
		if (dwDirection & DIR_BACKWARD) d3dxvShift -= XMLoadFloat3(&m_d3dxvLook) * fDistance;
		if (dwDirection & DIR_RIGHT)	d3dxvShift += XMLoadFloat3(&m_d3dxvRight) * fDistance;
		if (dwDirection & DIR_LEFT)		d3dxvShift -= XMLoadFloat3(&m_d3dxvRight) * fDistance;
		if (dwDirection & DIR_UP)		d3dxvShift += XMLoadFloat3(&m_d3dxvUp) * fDistance;
		if (dwDirection & DIR_DOWN)		d3dxvShift -= XMLoadFloat3(&m_d3dxvUp) * fDistance;

		d3dxvShift = XMVectorZero();
		if (dwDirection & DIR_FORWARD)	d3dxvShift += XMVectorSet(0,0,1,1) * fDistance;
		if (dwDirection & DIR_BACKWARD) d3dxvShift -= XMVectorSet(0,0,1,1) * fDistance;
		if (dwDirection & DIR_RIGHT)	d3dxvShift += XMVectorSet(1,0,0,1) * fDistance;
		if (dwDirection & DIR_LEFT)		d3dxvShift -= XMVectorSet(1,0,0,1) * fDistance;
		if (dwDirection & DIR_UP)		d3dxvShift += XMVectorSet(0,1,0,1) * fDistance;
		if (dwDirection & DIR_DOWN)		d3dxvShift -= XMVectorSet(0,1,0,1) * fDistance;


		Move(d3dxvShift, bUpdateVelocity);

	}
}

void CPlayer::Move(float fxOffset, float fyOffset, float fzOffset)
{
}

void CPlayer::Move(const XMVECTOR & d3dxvShift, bool bVelocity)
{
	XMVECTOR d3dxvPosition = XMLoadFloat3(&m_d3dxvPosition);
	d3dxvPosition = d3dxvPosition + d3dxvShift;
	XMStoreFloat3(&m_d3dxvPosition, d3dxvPosition);
}

void CPlayer::OnPrepareRender()
{
	m_d3dxmtxWorld._11 = m_d3dxvRight.x;
	m_d3dxmtxWorld._12 = m_d3dxvRight.y;
	m_d3dxmtxWorld._13 = m_d3dxvRight.z;


	m_d3dxmtxWorld._21 = m_d3dxvUp.x;
	m_d3dxmtxWorld._22 = m_d3dxvUp.y;
	m_d3dxmtxWorld._23 = m_d3dxvUp.z;


	m_d3dxmtxWorld._31 = m_d3dxvLook.x;
	m_d3dxmtxWorld._32 = m_d3dxvLook.y;
	m_d3dxmtxWorld._33 = m_d3dxvLook.z;


	m_d3dxmtxWorld._41 = m_d3dxvPosition.x;
	m_d3dxmtxWorld._42 = m_d3dxvPosition.y;
	m_d3dxmtxWorld._43 = m_d3dxvPosition.z;

	CGameObject::Update(nullptr);

}

void CPlayer::SetPosition(float x, float z)
{
	m_d3dxvPosition.x = x;
	m_d3dxvPosition.z = z;
}

void CPlayer::SetPosition(const XMVECTOR& d3dxvPosition)
{
	XMStoreFloat3(&m_d3dxvPosition, d3dxvPosition);
}

void CPlayer::SetPosition(const XMFLOAT3& d3dxvPosition)
{
	m_d3dxvPosition = d3dxvPosition;
}

void CPlayer::OffsetPosition(const XMFLOAT3& xmf3Offset)
{
	m_d3dxvPosition.x += xmf3Offset.x;
	m_d3dxvPosition.y += xmf3Offset.y;
	m_d3dxvPosition.z += xmf3Offset.z;

}

void CPlayer::OffsetPosition(const XMVECTOR& xmf3Offset)
{	
	XMVECTOR pos = XMLoadFloat3(&m_d3dxvPosition);
	pos = XMVectorAdd(pos, xmf3Offset);
	XMStoreFloat3(&m_d3dxvPosition, pos);
}

void CPlayer::SetRotation(float x, float y, float z)
{
	m_d3dxvRight = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_d3dxvUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_d3dxvLook = XMFLOAT3(0.0f, 0.0f, 1.0f);
	OnPrepareRender();
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z));
	XMStoreFloat4x4(&m_d3dxmtxWorld, mtxRotate * XMLoadFloat4x4(&m_d3dxmtxWorld));

	//if (m_pCamera)m_pCamera->SetRotation(x, y, z);
}

void CPlayer::ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed)
{
	
}

void CPlayer::OnPlayerUpdated(float fTimeElapsed)
{
}

void CPlayer::OnCameraUpdated(float fTimeElapsed)
{
}

void CPlayer::Render(ID3D11DeviceContext* pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	CGameObject::Render(pd3dDeviceContext, pCamera);

	if (m_pShadowShader)
	{
		m_pShadowShader->SetPosition(GetPositionXMFLOAT3());
		m_pShadowShader->Render(pd3dDeviceContext, pCamera);
	}

	//if (m_cPlayerState->GetVisible())
	//{
	//	XMFLOAT3 xmPosition = GetPositionXMFLOAT3(); xmPosition.y += 0.5f;
	//	m_pCubeShader->GetvRenderObject()[0]->SetPosition(xmPosition);
	//	m_pCubeShader->RenderPlayer(pd3dDeviceContext, pCamera);
	//}

	if (m_cPlayerState->IsHeadBang())
	{
		UIMGR->UseShader("Alpha");
		m_pHeadBangBangShader->RenderPlayer(pd3dDeviceContext);
	}
	else
		UIMGR->EndUseShader("Alpha");

}

void CPlayer::Animate(float fTimeElapsed, XMMATRIX *pd3dxmtxParent)
{
	CGameObject::Animate(fTimeElapsed);
	CPlayer::OnPrepareRender();
	CGameObject::Update(pd3dxmtxParent);
}

void CPlayer::StartHeadBangBang()
{ 
	m_cPlayerState->StartHeadBangBang(); 
}

void CPlayer::StopHeadBangBang() 
{ 
	m_cPlayerState->StopHeadBangBang(); 
}


void CPlayer::Animate(float fTimeElapsed)
{
	CGameObject::Animate(fTimeElapsed);

	auto n = m_vComponent.size();

	for (int i = 0; i < n; ++i)
		if( i < m_vComponent.size()) m_vComponent[i]->OnPlayerUpdate(fTimeElapsed);

	CPlayer::OnPrepareRender();
	CGameObject::Update(nullptr);
}

void CPlayer::Rotate(float x, float y, float z)
{
	CGameObject::Rotate(x, y, z);

	m_d3dxvRight.x = m_d3dxmtxWorld._11;
	m_d3dxvRight.y = m_d3dxmtxWorld._12;
	m_d3dxvRight.z = m_d3dxmtxWorld._13;


	m_d3dxvUp.x = m_d3dxmtxWorld._21;
	m_d3dxvUp.y = m_d3dxmtxWorld._22;
	m_d3dxvUp.z = m_d3dxmtxWorld._23;


	m_d3dxvLook.x = m_d3dxmtxWorld._31;
	m_d3dxvLook.y = m_d3dxmtxWorld._32;
	m_d3dxvLook.z = m_d3dxmtxWorld._33;
}

void CPlayer::Respawn()
{
	m_cPlayerState->SetHp(1);
	static_cast<PlayerAnimation*>(m_vComponent[2].get())->Respawn();

}