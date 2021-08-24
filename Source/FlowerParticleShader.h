#pragma once

#include "QuadParticleShader.h"

class CFlowerParticleShader : public CQuadParticleShader
{
private:
	float m_fPauseTime;
	int m_fMaxPauseTime;
public:
	CFlowerParticleShader();

	virtual ~CFlowerParticleShader();

	virtual void Initialize(ID3D11Device *pd3dDevice, TCHAR *pszFileName, XMFLOAT3 d3dxvPosition, UINT nMaxParticles);

	virtual void PauseTime(const int& fPause) { m_fMaxPauseTime = fPause; }

	virtual bool Update(float fTimeElapsed);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

};