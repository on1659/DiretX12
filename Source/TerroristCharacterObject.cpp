#include "stdafx.h"
#include "TerroristCharacterObject.h"


CTerroristCharacterObject::CTerroristCharacterObject()
{
	m_tagMesh = MeshTag::eTerrorist;
}

CTerroristCharacterObject::~CTerroristCharacterObject()
{
}

void CTerroristCharacterObject::CreateMesh(ID3D11Device *pd3dDevice)
{
	CFbxModelSkinnedMesh* pCharacterMesh = new CFbxModelSkinnedMesh(pd3dDevice, MeshTag::eTerrorist);
	//	CFbxModelSkinnedMesh* pCharacterMesh = new CFbxModelSkinnedMesh(pd3dDevice, MeshTag::eTest);
	pCharacterMesh->Initialize(pd3dDevice);

	SetMesh(pCharacterMesh);
}

void CTerroristCharacterObject::CreateShader(ID3D11Device *pd3dDevice)
{
	m_pShader = new CCharacterNormalShader();
	m_pShader->CreateShader(pd3dDevice);
}

void CTerroristCharacterObject::CreateMaterial()
{
	CTexture *pCharacterTexture = new CTexture(2, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);

	m_pMaterial = new CMaterial();
	//	pCharacterTexture->SetTexture(0, TextureTag::eTest);
	pCharacterTexture->SetTexture(0, TextureTag::eTerroristD);
	pCharacterTexture->SetTexture(1, TextureTag::eTerroristND);
	pCharacterTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);

	m_pMaterial->SetTexture(pCharacterTexture);
}

void CTerroristCharacterObject::CreateAnimation()
{
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eIdle,				AnimationTrack("Idle"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalk,				AnimationTrack("Walk"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eRun,				AnimationTrack("Run"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eStandingFire,		AnimationTrack("StandingFire"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalkingFire,		AnimationTrack("WalkingFire"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eStandingReloading, AnimationTrack("StandingReloading"), AnimationData::Type::eOnce));
}

void CTerroristCharacterObject::CreateWeapon(ID3D11Device *pd3dDevice)
{
	m_pWeapon = new CRifleGunWeapon(this);

	m_pWeapon->CreateObjectData(pd3dDevice);
	m_pWeapon->CreateAxisObject(pd3dDevice);
}