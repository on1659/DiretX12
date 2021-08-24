#pragma once

#include "Shader.h"
#include "SkyBox.h"

class CSkyBoxShader : public CShader
{
public:
	CSkyBoxShader();
	virtual ~CSkyBoxShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
};