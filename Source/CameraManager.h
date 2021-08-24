#pragma once

class CPlayer;
class CCamera;
class CFirstPersonCamera;
class CSpaceShipCamera;
class CThirdPersonCamera;

class CCameraManager : public CSingleTonBase<CCameraManager>
{
private:
	//std::shared_ptr<CCamera>					m_pMainCamera;
	std::vector<std::shared_ptr<CCamera>>		m_vCamera;

public:
	CCameraManager();

	~CCameraManager();
	
	void Load(ID3D11Device *pd3dDevice);

	std::shared_ptr<CCamera> GetCameara(int index = 0) const { return m_vCamera[index]; }

	void ChangeCamera(std::shared_ptr<CCamera> pCamera, int index = 0);

	void SetData(CPlayer* pPlayer);

	void CreateCamera(ID3D11Device *pd3dDevice, DWORD nCameraMode = THIRD_PERSON_CAMERA);

	std::shared_ptr<CCamera> ChnageCamera(DWORD nCameraMode, int index = 0);

	std::shared_ptr<CCamera> CreateCamera
	(
		ID3D11Device *pd3dDevice,
		DWORD nNewCameraMode, 
		DWORD nCurrentCameraMode,
		XMFLOAT3 vRight,
		XMFLOAT3 vLook,
		std::shared_ptr<CCamera> pCamera = nullptr
	);
	
	template<typename T>
	std::shared_ptr<CCamera> ChangeCamera(std::shared_ptr<CCamera> pOldCamera, bool isMainCamera = false);

	void OnProcessingMouseMessage(UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void OnProcessingKeyboardMessage(UINT nMessageID, WPARAM wParam);

	void ProcessInput(DWORD dwDirection, POINT ptOldCursorPos);

	std::shared_ptr<CCamera> GetMainCamera(){ return m_vCamera[0]; }

};

