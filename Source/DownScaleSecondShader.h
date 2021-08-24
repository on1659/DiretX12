#pragma once

#include "ComputeShaderBase.h"

class CDownScaleSecondShader : public CComputeShaderBase
{

private:
	ID3D11Buffer*					m_pSecondPassBuffer;
	ID3D11UnorderedAccessView*		m_pSecondPassUAV;
	ID3D11ShaderResourceView*		m_pSecondPassSRV;
	ID3D11Buffer*					m_pSecondPassCB;


	//bloom
	float							 m_fBloomThreshold;
	//1
	ID3D11Texture2D					*m_pd3dtxtDownCasting;
	ID3D11ShaderResourceView		*m_pd3dsrvDownCasting;
	ID3D11UnorderedAccessView		*m_pd3duavDownCasting;


public:
	CDownScaleSecondShader();

	~CDownScaleSecondShader();

	void ReleaseShaderVariables() override;

	void CreateShader(ID3D11Device *pd3dDevice) override;

	void UpdateShader(ID3D11DeviceContext *pd3dDeviceContext) override;

	void SetAdaptation(const float& fAdaptation) { m_fAdaptation = fAdaptation; }

	void SetBloomThreshold(const float& fBloomThreshold) { m_fBloomThreshold = fBloomThreshold; }


};

