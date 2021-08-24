#pragma once

#include "InstancedObjectsShader.h"

class CBillBoardShader;
class MonsterState;

class CHeadBangBangShader : public CInstancedObjectsShader
{

protected:
	std::unique_ptr<CBillBoardShader>		m_pBillBoardShader;
	CPlayer*								m_pPlayer;
	std::list
		<std::shared_ptr<CGameObject>>		m_lRenderObject;

public:

	CHeadBangBangShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CHeadBangBangShader(std::unique_ptr<CMesh> pMesh);

	CHeadBangBangShader(int nObjects = 1);

	virtual ~CHeadBangBangShader();

	void SetPlayer(CPlayer* player);

	virtual void CreateShader(ID3D11Device* pd3dDeivce);

	virtual void RenderPlayer(ID3D11DeviceContext* pd3dDeviceContext);

	virtual void RenderNoClear(ID3D11DeviceContext* pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	virtual void RenderMonster(ID3D11DeviceContext* pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	virtual void AddHeadBangBang(XMFLOAT3 xmPivotPosition, MonsterState* state);

	virtual void AnimateHeadBangBang(float fTimeElapsed);

	virtual void BuildObjects(ID3D11Device* pd3dDeivce);

};

