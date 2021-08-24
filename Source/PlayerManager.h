#pragma once


class CHeightMapTerrain;
class CTerrainPlayer;
class COtherPlayer;

#ifndef _CONNECT_SERVER_
	#define GAME_SEVER_TEST
#endif 

class CPlayerManager : public CSingleTonBase<CPlayerManager>
{
private:
	std::shared_ptr<CInstancedObjectsShader>				m_pPlayerShader;
	std::shared_ptr<CTerrainPlayer>							m_pPlayer;
	std::shared_ptr<CCamera>								m_pCamera;
	ID3D11Device*											m_pd3dDevice;

	std::vector<std::shared_ptr<CInstancedObjectsShader>>	m_pOtherPlayerShader;
	std::vector<std::shared_ptr<COtherPlayer>>				m_pOtherPlayer;

	int														m_mapData[252][160];

	float													m_fSpeed;

	float													m_fPlayerSize;

	//
	bool													m_firstkeydown;

	std::shared_ptr<CCamera>								m_pShadowCamera;



public:

	CPlayerManager();

	~CPlayerManager();

	void ReleseInstance();

	void Load(ID3D11Device* pd3dDevice);

	void Render(ID3D11DeviceContext *pd3dDeviceContext);

	void Animate(float fTimeElapsed);

	void OnProcessingKeyboardMessage(UINT nMessageID, WPARAM wParam);

	void OrthoCameraLoad(ID3D11Device *pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext = nullptr);


	//kYT '16.05.02 
	/*
		Player Move -> Componet 이사
	*/
	//void ProcessInput(DWORD dwDirection, POINT ptOldCursorPos);
	//
	//void ProcessInput();

	std::shared_ptr<CCamera> GetCamera() { return m_pCamera; }

	std::shared_ptr<CTerrainPlayer> GetPlayer() { return m_pPlayer; }

	std::shared_ptr<CInstancedObjectsShader> GetPlayerShader() { return m_pPlayerShader; }

	std::shared_ptr<CPlayer> GetOtherPlayer(int index);

	std::vector<std::shared_ptr<COtherPlayer>> GetOtherPlayer(){ return m_pOtherPlayer;}


	//KYT '16.06.28
	/*
		GetOtherPlayer -> GetOtherPlayerShader명칭 변경;
	*/
	std::vector<std::shared_ptr<CInstancedObjectsShader>>
		GetOtherPlayerShader() { return m_pOtherPlayerShader; }

	bool OnCollision();

	bool OnCollision(std::shared_ptr<CGameObject> object);

	//JJY '16.04.24
	/*
	데드레커닝
	*/

	void ServerReturn_Pos_Move(int obj_idx, float pos_x, float pos_z);
	void ServerReturn_Rotate(int obj_idx, float cxDelta, float cyDelta);

	void Client_DeadReckoning();

private:
		

public:
	//void SetTerrain(std::shared_ptr<CHeightMapTerrain> pTerrain){ m_pTerrain = pTerrain; }
};

