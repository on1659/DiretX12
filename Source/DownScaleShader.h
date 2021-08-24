#pragma once
#include "ComputeShaderBase.h"

class CDownScaleShader : public CComputeShaderBase
{

public:
	
	CDownScaleShader();

	~CDownScaleShader();

	void ReleaseShaderVariables() override;

	// Create
	void CreateShader(ID3D11Device *pd3dDevice) override;

	void FirstCompute(ID3D11Device *pd3dDevice);
	void SecondCompute(ID3D11Device *pd3dDevice);

	// Update
	void UpdateComputeShader(ID3D11DeviceContext *pd3dDeviceContext, ID3D11ShaderResourceView* pd3dsrvLDR);

	void First_UpdateComputeShader(ID3D11DeviceContext* pd3dDeviceContext, ID3D11ShaderResourceView* pd3dsrvLDR);
	void Second_UpdateComputeShader(ID3D11DeviceContext* pd3dDeviceContext);

private:

	UINT							m_nDownScaleGroups;

	// Downscaled scene texture - First
	ID3D11Texture2D					*m_pDownScaleRT;
	ID3D11ShaderResourceView		*m_pDownScaleSRV;
	ID3D11UnorderedAccessView		*m_pDownScaleUAV;

	// 1D intermediate storage for the down scale operation _ Second
	ID3D11Buffer					*m_pDownScale1DBuffer;
	ID3D11UnorderedAccessView		*m_pDownScale1DUAV;
	ID3D11ShaderResourceView		*m_pDownScale1DSRV;


	// Shaders
	ID3D11ComputeShader				*m_pDownScaleFirstPassCS;
	ID3D11ComputeShader				*m_pDownScaleSecondPassCS;

};

