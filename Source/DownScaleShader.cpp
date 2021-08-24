#include "stdafx.h"
#include "DownScaleShader.h"


CDownScaleShader::CDownScaleShader()
	: CComputeShaderBase()

{
}

CDownScaleShader::~CDownScaleShader()
{

}

void CDownScaleShader::ReleaseShaderVariables()
{

}

void CDownScaleShader::FirstCompute(ID3D11Device *pd3dDevice)
{
	
}


void CDownScaleShader::SecondCompute(ID3D11Device *pd3dDevice)
{
	}

void CDownScaleShader::CreateShader(ID3D11Device *pd3dDevice)
{

	//CDownScaleShader::FirstCompute(pd3dDevice);
	//CDownScaleShader::SecondCompute(pd3dDevice);

}

void CDownScaleShader::UpdateComputeShader(ID3D11DeviceContext* pd3dDeviceContext, ID3D11ShaderResourceView* pd3dsrvLDR)
{

}


void CDownScaleShader::First_UpdateComputeShader(ID3D11DeviceContext* pd3dDeviceContext, ID3D11ShaderResourceView* pd3dsrvLDR)
{
	ID3D11UnorderedAccessView* arrUAVs[2] = { m_pDownScale1DUAV, m_pDownScaleUAV };
	pd3dDeviceContext->CSSetUnorderedAccessViews(0, 2, arrUAVs, nullptr);

	ID3D11ShaderResourceView* arrViews[2] = { pd3dsrvLDR, nullptr };
	pd3dDeviceContext->CSSetShaderResources(0, 1, arrViews);

	pd3dDeviceContext->CSSetShader(m_pDownScaleFirstPassCS, nullptr, 0);

	pd3dDeviceContext->Dispatch(m_nDownScaleGroups, 1, 1);

}

void CDownScaleShader::Second_UpdateComputeShader(ID3D11DeviceContext* pd3dDeviceContext)
{
	ID3D11UnorderedAccessView* arrUAVs[2]; 
	ID3D11ShaderResourceView* arrViews[2];
										 
	//ZeroMemory(arrUAVs, sizeof(arrUAVs));
	//arrUAVs[0] = m_pAvgLumUAV;
	//pd3dDeviceContext->CSSetUnorderedAccessViews(0, 2, arrUAVs, nullptr);
	//
	//// Input
	//arrViews[0] = m_pDownScale1DSRV;
	//arrViews[1] = m_pPrevAvgLumSRV;
	//pd3dDeviceContext->CSSetShaderResources(0, 2, arrViews);
	//
	//pd3dDeviceContext->CSSetShader(m_pDownScaleSecondPassCS, nullptr, 0);
	//
	//pd3dDeviceContext->Dispatch(1, 1, 1);
	//
	//// Cleanup
	//pd3dDeviceContext->CSSetShader(nullptr, nullptr, 0);
	//ZeroMemory(arrViews, sizeof(arrViews));
	//pd3dDeviceContext->CSSetShaderResources(0, 2, arrViews);
	//ZeroMemory(arrUAVs, sizeof(arrUAVs));
	//pd3dDeviceContext->CSSetUnorderedAccessViews(0, 2, arrUAVs, (UINT*)(&arrUAVs));

}
