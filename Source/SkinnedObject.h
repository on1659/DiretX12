#pragma once
#include "Object.h"
#include "AnimationController.h"
#include "FbxModelSkinnedMesh.h"

class CSkinnedObject : public CGameObject
{
protected:
	CFbxModelSkinnedMesh*	m_pSkinnedMesh = nullptr;
	CAnimationController*	m_pAnimController = nullptr;

public:
	CSkinnedObject();
	virtual ~CSkinnedObject();

protected:
	virtual void CreateAnimation() = 0;

public:
	virtual void CreateObjectData(ID3D11Device *pd3dDevice);

	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	// ------ Get, Setter ------ //
	CFbxModelSkinnedMesh* GetSkinnedMesh() const { return m_pSkinnedMesh; }
	AnimationData::CharacterAnim GetAnimation() const { return m_pAnimController->GetAnimEnum(); }

	void SetMesh(CFbxModelSkinnedMesh* mesh);
	void SetAnimation(AnimationData::CharacterAnim anim, float speed = 1.0f);
};

