#pragma once

class CCubeParticleShader;
class CQuadParticleShader;
class CWaterFallShader;

class CCamera;

class CBillBoardShader;
class CFlowerParticleShader;
class CParticleManager : public CSingleTonBase<CParticleManager>
{

private:

	struct CameraPosition
	{
		XMFLOAT4 gf4Position;
	};

	ID3D11Device*																m_pd3dDevice;


	//------------------------------------------------------------------------------------------------------------
	std::multimap<std::string,
		std::vector<std::shared_ptr<CCubeParticleShader>>>						m_mParticleShader;
	
	std::vector<std::shared_ptr<CQuadParticleShader>>							m_vWaterFallShader;

	float																		m_fSpeed;
	float																		m_fSize;
	float																		m_fDuringTime;
	float																		m_fEmitCount;
	std::vector<std::shared_ptr<CWaterFallShader>>								m_vWaterParticle;

	int																			m_nWaterFallMaxEmitCount;
	int																			m_nWarerFallLoop;
	int																			m_nWaterFallBottom;

	ID3D11Buffer*																m_pd3dcbCameraPosition;
	std::shared_ptr<CCamera>													m_pCamera;
		
	//------------------------------------------------------------------------------------------------------------
	std::map<std::string, std::shared_ptr<CBillBoardShader>>				  m_mBilBoardEffectShader;

public:

	CParticleManager();

	~CParticleManager();

	void ReleseInstance();

private:

	void PushParticleContainer(std::string name, std::shared_ptr<CCubeParticleShader> pParticle);
	std::vector<std::shared_ptr<CCubeParticleShader>>& FindParticleContainer(std::string name);

	void AddCubePartcielShader(std::string name, int nCount);
	void AddCubePartcielShader(std::string name, int nCount, float fSpeed, float fSize, float fAge, int nMaxPariticle, XMFLOAT3 fRandPosition = XMFLOAT3(0.0f, 0.0f, 0.0f), TCHAR* path = L"../Assets/03_Image/Particle/Smash.png");

	void AddQuadPartcieShader(std::string name, int nCount);
	void AddQuadPartcieShader(std::string name, int nCount, float fSpeed, float fSize, float fAge, int nMaxPariticle, XMFLOAT3 fRandPosition = XMFLOAT3(0.0f, 0.0f, 0.0f), TCHAR* path = L"../Assets/03_Image/Particle/Smash.png");

	void AddWaterFallPartcielhader(std::string name, int nCount, float fSpeed, float fSize, float fAge, int nMaxPariticle, XMFLOAT3 fRandPosition = XMFLOAT3(0.0f, 0.0f, 0.0f), TCHAR* path = L"../Assets/03_Image/Particle/Smash.png");


	//Animatino Sprite
	std::shared_ptr<CBillBoardShader> FindSpriteAnimationShader(std::string name);

	void AddAnimationSpriteShader(std::string name, std::string path, std::string blending, XMFLOAT2 fSize, float frameFPS, float frameCount);

	void AddBillBoardEffectShader(std::string name, std::string path, XMFLOAT2 fSize);


public:

	//KYT '16.05.12
	/*
	총알의 종류를 증가 시킬꺼면 여기서 쉐이더를 추가하자.
	*/
	void Load(ID3D11Device* pd3dDevice);
	void CreateConstBuffer(ID3D11Device* pd3dDevice);
	void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	void StartParticle(const std::string& name, XMFLOAT3 xmPosition, int loop = 1);
	void StartParticle(const std::string& name, const float& x, const float& y, const float& z, int loop = 1);
	void StartSprtieAnimation(const std::string& name, const float& x, const float& y, const float& z, int loop = 1);
	void StartSprtieAnimation(const std::string& name, const XMFLOAT3& position, int loop = 1);

	void Render(ID3D11DeviceContext* pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
	void Animate(float fTimeElasped);

	void SetCamera(std::shared_ptr<CCamera> pCamera) { m_pCamera = pCamera; }


};

