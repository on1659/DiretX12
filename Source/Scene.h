//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------

#pragma once

#include "Camera.h"
#include "Shader.h"

class CHeightMapTerrain;
class CInstancedObjectsShader;
class CGeoMetryInstanceShader;
class CUIShader;
class CBillBoardShader;
class CAABBRenderShader;
class CGeoInstanceingCubeShader;

class CBlendShader;
class CTrapShader;

class CParticlePangShader;

class CMonsterHeadCubeShader;
class CGameSystem;
class MonsterState;

class CUIShader;
class CBillBoardSpriteShader;
class CBillBoardDisappearShader;

class CUIGauge;
class CAxisObjects;

class CScene : public CSingleTonBase<CScene>, CSceneState
{
public:
	CScene();

	virtual ~CScene();

	//Input Msg
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;

	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;

	virtual bool ProcessInput(POINT ptOldCursorPos);

	//Create
	virtual void BuildObjects(ID3D11Device *pd3dDevice);

	virtual void Load(ID3D11Device *pd3dDevice);

	virtual void LoadStaticFBX(ID3D11Device *pd3dDevice);

	virtual void LoadDynamicFBX(ID3D11Device *pd3dDevice);


	//Relase
	virtual void ReleaseObjects();

	//Udpate
	virtual void AnimateObjects(float fTimeElapsed);

	//OnCollision
	virtual void OnCollision(ID3D11DeviceContext *pd3dDeviceContext);

	//Render
	virtual void OnPreRender(ID3D11DeviceContext *pd3dDeviceContext);


	void SetViewport(ID3D11DeviceContext *pd3dDeviceContext);

	//Light, Camera..
	virtual void RenderAway(ID3D11DeviceContext *pd3DeviceContext);

	//Player, UI ...
	virtual void RenderBasic(ID3D11DeviceContext *pd3DeviceContext);

	//SkyBox, Terrain ...
	virtual void RenderEnvironment(ID3D11DeviceContext *pd3dDeviceContext);

	//Blend Shader
	virtual void RenderBlend(ID3D11DeviceContext *pd3dDeviceContext);

	//SpacePartition
	virtual void RenderSpacePartition(ID3D11DeviceContext *pd3dDeviceContext);

	//Non MTR
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

	//Picking
	CGameObject *PickObjectPointedByCursor(int xClient, int yClient);

	//SkyBox
	void OnChangeSkyBoxTextures(ID3D11Device *pd3dDevice, CMaterial *pMaterial, int nIndex = 0);

	//GetTerrain
	//CHeightMapTerrain *GetTerrain();


	//Player Set & Get
	void SetPlayer(std::shared_ptr<CPlayer> pPlayer) { m_pPlayer = pPlayer; }

	//Camera Set & Get
	void SetCamera(std::shared_ptr<CCamera> pCamera) { m_pCamera = pCamera; }


public:
	std::vector<std::shared_ptr<CGameObject>>		m_ppObjects;
	int												m_nObjects;

	std::shared_ptr<CAABBRenderShader>				m_aabbShader;

private:
	ID3D11Device								*m_pd3dDevice;


	CObjectsShader								**m_ppObjectShaders;
	int											m_nObjectShaders;

	CInstancedObjectsShader						**m_ppInstancingShaders;
	int											m_nInstancingShaders;

	std::shared_ptr<CPlayer>					m_pPlayer;
	CGameObject									*m_pSelectedObject;

	std::shared_ptr<CCamera>					m_pCamera;

	//텍스트블랜딩
	ID3D11ShaderResourceView*					m_pd3dLayerMapArray;

	ID3D11Buffer*								m_pd3dcbFog = nullptr;

public:

	//KYT '16.04.29
	/*
	Fog 파라메터 세팅을 HLSL로 넘김
	*/


	//Space Partitiong
private:
	std::vector<CShader*>						m_vRenderShader;
	std::vector<CShader*>						m_vBlendShader;
	std::list<CMatrixShader*>					m_vTerrainRenderShader;

	//Debug Mode
private:
	bool										m_bMouseClick;

	std::vector<std::string>					vPath;

	//Blend
	//std::shared_ptr<CBlendShader>	pBlendshader;
	//std::shared_ptr<CBlendShader>	pItemCubeShader;

	///Game System
private:
	CGameSystem*							m_pGameSystem;

public:
	void LoadSystem();

	//ParticlePangShader
private:


	//KYT '16.05.12
	/*
		총알 텍스쳐
	*/

private:

	std::vector<	//0-Apple / 1-햄버거 / 2-도넛 / 3-체리 / 4-블랜딩큐브
		std::shared_ptr<CInstancedObjectsShader>>	m_vItemShader;

	std::list<std::shared_ptr<CGameObject>>				m_lFakeItemList;

	std::shared_ptr<CTrapShader>						m_pTrapShader;

	std::shared_ptr<CBulletShader>						m_pBulletItemShader;

	std::shared_ptr<CBulletShader>						m_pSkillThrowShader;

	std::shared_ptr<CBulletShader>						m_pFlashThrowShader;

public:

	// - 플레이어가 가짜아이템을 먹으면 트랩이 나와야 하니깐 그거 추가해주는 함수
	void AddTrap(const XMFLOAT3& xmPosition, CPlayer* player, const int& playerId);

	// - 플레이어가 가짜아이템을 사용했을때 가짜아이템을 설치해주는 함수
	void AddFakeInstall(const XMFLOAT3& xmPosition);

	void AddBullet(XMFLOAT3 xmPosition, const XMVECTOR& x3Look, const int& playerId);

	void AddThrowSkill(XMFLOAT3 xmPosition, XMVECTOR& x3Look, const int& playerId);

	void AddFlashItem(XMFLOAT3 xmPosition, const XMVECTOR& x3Look, const int& playerId);

	CBulletShader* GetBulletShader() const { return m_pBulletItemShader.get(); }

	CBulletShader* GetSkillShader() const { return m_pSkillThrowShader.get(); }

	CBulletShader* GetFlashShader() const { return m_pFlashThrowShader.get(); }

	//몬스터 식별자
public:

	std::shared_ptr<CMonsterHeadCubeShader>		m_pMonsterHeadCubeShader[2];

	void AddMonsterHeadCube(const XMFLOAT3& xmPosition, const int& playerId);


private:
	std::vector
		<std::shared_ptr<CInstancedObjectsShader>>	m_vMonsterShader;

public:
	CInstancedObjectsShader* GetMonsterShader(int n) const { return m_vMonsterShader[n].get(); }

	std::vector
		<std::shared_ptr<CInstancedObjectsShader>> GetMonsterShader() const { return m_vMonsterShader; }

private:
	std::unique_ptr<CHeadBangBangShader>			m_pHeadBangBangShader;

public:
	void AddHeadBangBang(const XMFLOAT3& xmPosition, MonsterState* state);

	private: 
		bool bGamePlaying;

	public:
		bool IsGamePlaying() const { return bGamePlaying; }
		void SetGamePlaying(bool play) { bGamePlaying = play; }

	private:

		std::unique_ptr<CUIGauge>			 m_pGauge;
		bool								 m_bRespawnVisible;
		float								 m_fRespawnStartTime;
		float								 m_fRespawnDuringTime;

		std::vector<std::string>			m_vDefaultMaterial_path;

		std::map <std::string, bool>		m_mTerrainMap;
};
