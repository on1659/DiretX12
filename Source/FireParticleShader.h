#pragma once

//KYT '16.03.05 plus 
/*
	CFireParticleShader 제작
	VS,PS만을 이용한 Shader임
*/

#include "ParticleShader.h"

//use VS
struct NoiseBufferType
{
	float frameTime;
	XMFLOAT3 scrollSpeeds;
	XMFLOAT3 scales;
	float padding; 
};

//use PS
struct Distortion
{
	XMFLOAT2 distortion1;
	XMFLOAT2 distortion2;
	XMFLOAT2 distortion3;
	float    distortionScale;
	float    distortionBias;
};

//----------------------------------------------------------------
class CFireParticleShader : public CParticleShader
{
private:
	
	bool							 bInit;

	//VS
	ID3D11Buffer					*m_pd3dcbNoiseBufferType;
	NoiseBufferType					m_NoiseBufferType;

	//PS
	ID3D11Buffer					*m_pd3dcbDistortion;
	Distortion						m_Distortion;


public:

	CFireParticleShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CFireParticleShader(std::unique_ptr<CMesh> pMesh);

	CFireParticleShader(int nObjects = 1);

	virtual ~CFireParticleShader();

	void CreateShader(ID3D11Device *pd3dDevice);

	void BuildObjects(ID3D11Device *pd3dDevice, void *pContext = nullptr) override;

	void CreateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	void AnimateObjects(float fTimeElapsed);

	void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	XMMATRIX SetLookAt(XMFLOAT4X4 &xmMatrix, XMFLOAT3 d3dxvTarget);

};