#pragma once

#include "ComputeShaderBase.h"

class CBloomShader : public CComputeShaderBase
{
public:
	CBloomShader();

	~CBloomShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;

};

