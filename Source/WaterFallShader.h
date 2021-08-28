#pragma once

#include "CubeParticleShader.h"

class CWaterFallShader : public CCubeParticleShader
{
protected:
	struct SpriteAnimation
	{
		float fSpriteAnimation_FPS;
		float fSpriteAnimation_Count;
		float fSpriteAnimation_Width;
		float fSpriteAnimation_Left;
	};

private: 
	float m_fPauseTime;
	float m_fMaxPauseTime;
	int	  m_iParticle;

	float						m_frameFPS;
	int							m_frameCount;
	float						m_frameWidth;
	float						m_frameLeft;

	float						m_imageWidth;

	float						m_fFPSCoount;
	int							m_nCurrentFrame;

	ID3D11Buffer*				m_pd3dcbAnimationSprite;

	bool						m_bSoundPlaying;

public:
	CWaterFallShader();

	virtual ~CWaterFallShader();

	void SetNumber(int n) { m_iParticle = n; }

	void SetRandomTexture(ID3D11ShaderResourceView* texture1D)
	{
		if (m_pd3dsrvRandomTexture)m_pd3dsrvRandomTexture->Release();
		m_pd3dsrvRandomTexture = texture1D;
	}

	virtual void Initialize(ID3D11Device* pd3dDevice, TCHAR *pszFileName, XMFLOAT3 d3dxvPosition, UINT nMaxParticles, float frameFPS, float frameCount);

	virtual void AfterCreateShader(ID3D11Device* pd3dDevice);

	virtual void AfterAuraCreateShader(ID3D11Device* pd3dDevice);

	virtual void PauseTime(const float& fPause) { m_fMaxPauseTime = fPause; }

	virtual bool Update(float fTimeElapsed);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	virtual void CreateConstBuffer(ID3D11Device* pd3dDevice);

	virtual void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

};