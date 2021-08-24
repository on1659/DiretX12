#include "stdafx.h"
#include "SkinnedObject.h"


CSkinnedObject::CSkinnedObject()
{
	m_pAnimController = new CAnimationController();
}

CSkinnedObject::~CSkinnedObject()
{
	SafeDelete(m_pAnimController);
}

void CSkinnedObject::CreateObjectData(ID3D11Device *pd3dDevice)
{
	CGameObject::CreateObjectData(pd3dDevice);
	CreateAnimation();

	// 기본 애니메이션 세팅
	m_pAnimController->SetAnimation(AnimationData::CharacterAnim::eIdle);
}

void CSkinnedObject::SetMesh(CFbxModelSkinnedMesh* mesh) 
{
	CGameObject::SetMesh(mesh);
	m_pAnimController->SetMesh(mesh);
	m_pSkinnedMesh = mesh;
}

void CSkinnedObject::SetAnimation(AnimationData::CharacterAnim anim, float speed)
{
	m_pAnimController->SetAnimation(anim, speed);
}

void CSkinnedObject::Update(float fTimeElapsed)
{
	m_pAnimController->Update(fTimeElapsed);
}

void CSkinnedObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	m_pAnimController->UpdateConstantBuffer(pd3dDeviceContext);
	CGameObject::Render(pd3dDeviceContext, pCamera);
}