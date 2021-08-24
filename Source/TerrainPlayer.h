#pragma once

#include "Player.h"

class CTerrainPlayer : public CPlayer
{
public:
	CTerrainPlayer(int nMeshes = 1);
	virtual ~CTerrainPlayer();
	virtual void Release();

	virtual void ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed);
	virtual void OnPlayerUpdated(float fTimeElapsed);
	virtual void OnCameraUpdated(float fTimeElapsed);


	std::shared_ptr<CCamera> GetCamera() { return(m_pCamera); }
	virtual void SetCamera(std::shared_ptr<CCamera> pCamera) { m_pCamera = pCamera; }

	virtual void UpdateConstBuffers(ID3D11DeviceContext *pd3dDeviceContext);

	virtual void CamearMove(const XMVECTOR& d3dxvShift){ if (m_pCamera)m_pCamera->Move(d3dxvShift);}

	virtual void Move(DWORD nDirection, float fDistance, bool bVelocity = false);
	virtual void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	virtual void Move(const XMVECTOR& d3dxvShift, bool bVelocity = false);


	void Rotate(float x, float y, float z);
		 
	void Update(float fTimeElapsed);

	void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	std::shared_ptr<CCamera>  OnChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, DWORD nCurrentCameraMode);

	void SetPosition(float x, float z);

	void SetPosition(const XMVECTOR& d3dxvPosition);

	void SetPosition(const XMFLOAT3& d3dxvPosition);

	void SetPositionNotMove(const XMFLOAT3& d3dxvPosition);

protected:
	std::shared_ptr<CCamera>	m_pCamera;

};