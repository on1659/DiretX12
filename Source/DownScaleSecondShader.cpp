#include "stdafx.h"
#include "DownScaleSecondShader.h"

CDownScaleSecondShader::CDownScaleSecondShader()
	: CComputeShaderBase()
	, m_nWidth(0)
	, m_nHeight(0)
	, m_nDownScaleGroup(0)
	, m_fAdaptation(0.0f)
	, m_pSecondPassBuffer(nullptr)
	, m_pSecondPassUAV(nullptr)
	, m_pSecondPassSRV(nullptr)
	, m_pSecondPassCB(nullptr)
	, m_fBloomThreshold(0.0f)
	, m_pd3dtxtDownCasting(nullptr)
	, m_pd3dsrvDownCasting(nullptr)
	, m_pd3duavDownCasting(nullptr)
{
}

CDownScaleSecondShader::~CDownScaleSecondShader()
{
}

void CDownScaleSecondShader::ReleaseShaderVariables()
{
	if (m_pSecondPassBuffer) m_pSecondPassBuffer->Release();
	if (m_pSecondPassUAV)	m_pSecondPassUAV->Release();
	if (m_pSecondPassSRV)	m_pSecondPassSRV->Release();
	if (m_pSecondPassCB)		m_pSecondPassCB->Release();

	if (m_pd3dtxtDownCasting) m_pd3dtxtDownCasting->Release();
	if (m_pd3dsrvDownCasting) m_pd3dsrvDownCasting->Release();
	if (m_pd3duavDownCasting) m_pd3duavDownCasting->Release();

}

void CDownScaleSecondShader::CreateShader(ID3D11Device *pd3dDevice)
{
	CShader::CreateComputeShaderFromCompiledFile(pd3dDevice, TEXT("CSDownScaleSecondPass.cso"), &m_pd3dComputeShader);// , d3dInputElements, nElements, &m_pd3dVertexShader);

	m_nDownScaleGroup = (UINT)ceil((float)(FRAME_BUFFER_WIDTH * FRAME_BUFFER_HEIGHT / 16) / 1024);
	m_nWidth = FRAME_BUFFER_WIDTH;
	m_nHeight = FRAME_BUFFER_HEIGHT;


	//Dispatch Thread Group set
	SetThreadGroup(m_nDownScaleGroup, 1, 1);

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.StructureByteStride = sizeof(float);
	bufferDesc.ByteWidth = m_nDownScaleGroup * bufferDesc.StructureByteStride;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	pd3dDevice->CreateBuffer(&bufferDesc, NULL, &m_pSecondPassBuffer);


	D3D11_UNORDERED_ACCESS_VIEW_DESC DescUAV;
	ZeroMemory(&DescUAV, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	DescUAV.Format = DXGI_FORMAT_UNKNOWN;
	DescUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	DescUAV.Buffer.NumElements = m_nDownScaleGroup;
	pd3dDevice->CreateUnorderedAccessView(m_pSecondPassBuffer, &DescUAV, &m_pSecondPassUAV);
	Radar::DXUT_SetDebugName(m_pSecondPassSRV, "PostFX - Luminance Down Scale 1D SRV");


	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate average luminance buffer
	D3D11_SHADER_RESOURCE_VIEW_DESC dsrvd;
	ZeroMemory(&dsrvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	dsrvd.Format = DXGI_FORMAT_UNKNOWN;
	dsrvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	dsrvd.Buffer.NumElements = m_nDownScaleGroup;
	(pd3dDevice->CreateShaderResourceView(m_pSecondPassBuffer, &dsrvd, &m_pSecondPassSRV));
	Radar::DXUT_SetDebugName(m_pSecondPassSRV, "PostFX - Down Scale 1D SRV");

}

void CDownScaleSecondShader::UpdateShader(ID3D11DeviceContext* pd3dDeviceContext)
{
	//적응값 있는 cb매 프레임 set
	//값 변경만 하면 매번 효과 적용됨 이게 더 직관적인것 같다.
	D3D11_MAPPED_SUBRESOURCE Data;
	pd3dDeviceContext->Map(m_pSecondPassCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);
	YT_SecondPassCB *pSecondPassCB = static_cast<YT_SecondPassCB*>(Data.pData);
	pSecondPassCB->nWidth = m_nWidth / 4;
	pSecondPassCB->nHeight = m_nHeight / 4;
	pSecondPassCB->nGroupSize = m_nDownScaleGroup;
	pSecondPassCB->fAdaptation = m_fAdaptation;
	pSecondPassCB->nTotalPixels = pSecondPassCB->nWidth * pSecondPassCB->nHeight;
	pSecondPassCB->fBloomThreshold = m_fBloomThreshold;
	pd3dDeviceContext->Unmap(m_pSecondPassCB, 0);


	CDownScaleSecondShader::SetComputeShader(pd3dDeviceContext);

	//uav
	//bloom
	//첫 패스에서 블룸 1/16scale texture를 set한다.
	ID3D11UnorderedAccessView* arrUAVs[2] = { m_pd3duavDownCasting, m_pSecondPassUAV };
	pd3dDeviceContext->CSSetUnorderedAccessViews(0, 2, arrUAVs, (UINT*)(&arrUAVs));

	//constant
	ID3D11Buffer* arrConstBuffers[1] = { m_pSecondPassCB };
	pd3dDeviceContext->CSSetConstantBuffers(0, 1, arrConstBuffers);

	CDownScaleSecondShader::Dispatch(pd3dDeviceContext);


	CDownScaleSecondShader::ClearComputeShader(pd3dDeviceContext);

	ID3D11UnorderedAccessView* pUAVs[2] = { nullptr, nullptr };
	pd3dDeviceContext->CSSetUnorderedAccessViews(0, 2, pUAVs, (UINT*)(&pUAVs));

	ID3D11ShaderResourceView* pSRVs[2] = { m_pd3dsrvDownCasting, m_pSecondPassSRV };
	pd3dDeviceContext->CSSetShaderResources(0, 2, pSRVs);

}
