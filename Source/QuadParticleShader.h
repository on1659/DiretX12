#pragma once

#include "CubeParticleShader.h"



class CQuadParticleShader : public CCubeParticleShader
{

public:
	CQuadParticleShader();
	virtual ~CQuadParticleShader();
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateConstBuffer(ID3D11Device *pd3dDevice);
	virtual void UpdateConstBuffer(ID3D11DeviceContext* pd3dDeviceContext);

};