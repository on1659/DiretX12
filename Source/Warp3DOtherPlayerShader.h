#pragma once

//RSH 16.05.01
/*
상대 플레이어를 그리는 쉐이더
*/
#include "stdafx.h"
#include "Warp3DSkinningShader.h"

class CInstancedObjectsShader;
class CShadowMapShader;
class CWarpAnimation;
class PlayerState;

class CWarp3DOtherPlayerShader : public CWarp3DSkinningShader
{
public:
	CWarp3DOtherPlayerShader(std::unique_ptr<CMesh> pMesh);
	CWarp3DOtherPlayerShader(int nObjects = 1);
	~CWarp3DOtherPlayerShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);

	virtual void BuildObjects(ID3D11Device *pd3dDevice, void *pContext = nullptr);
	virtual void RenderPlayer(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	virtual void AnimateObjects(float fTimeElapsed);
	virtual void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

protected:
	std::vector<CWarpAnimation*>				m_pAnimations;
	AnimState									m_currnetAnimationState;

protected:
	std::unique_ptr<CInstancedObjectsShader>	m_pCubeShader;

	//KYT '16.05.10
	/*
		그림자 텍스쳐
	*/
	std::unique_ptr<CShadowMapShader>			m_pShadowShader;


	//KYT '16.07.21
	/*
		헤드뱅뱅
	*/
	bool										m_bHeadMove;
	std::unique_ptr<CHeadBangBangShader>		m_pHeadBangBangShader;

	PlayerState*								m_sPlayerState;

public: 
	void CreateTeamMarkMesh(ID3D11Device* pd3dDevice, UINT team_line);

};

class CQueryChanOtherPlayerShader : public CWarp3DOtherPlayerShader
{
public:
	CQueryChanOtherPlayerShader(int nObjects = 1);
};