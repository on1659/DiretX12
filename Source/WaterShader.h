#pragma once




class CWaterShader : public CShader
{
public:
	CWaterShader();

	virtual ~CWaterShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);

};

