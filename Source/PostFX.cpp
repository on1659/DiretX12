#include "stdafx.h"
#include "PostFX.h"

CPostFX::CPostFX() 
	: CShader()

	, m_pSampLinear(nullptr)
	, m_pSampPoint(nullptr)

	, m_fMiddleGrey(0.0025f)
	, m_fWhite(1.5f)
	, m_fBloomThreshold(2.0)
	, m_fBloomScale(0.1f)

	, m_pDownScaleRT(nullptr)
	, m_pDownScaleSRV(nullptr)
	, m_pDownScaleUAV(nullptr)

	, m_pBloomRT(nullptr)
	, m_pBloomSRV(nullptr)
	, m_pBloomUAV(nullptr)
	
	, m_pDownScale1DBuffer(nullptr)
	, m_pDownScale1DUAV(nullptr)
	, m_pDownScale1DSRV(nullptr),

	  m_pDownScaleCB(nullptr)
	, m_pFinalPassCB(nullptr)
	, m_pBlurCB(nullptr),

	  m_pAvgLumBuffer(nullptr)
	, m_pAvgLumUAV(nullptr)
	, m_pAvgLumSRV(nullptr),

	  m_pPrevAvgLumBuffer(nullptr)
	, m_pPrevAvgLumUAV(nullptr)
	, m_pPrevAvgLumSRV(nullptr),

	  m_pDownScaleFirstPassCS(nullptr)
	, m_pDownScaleSecondPassCS(nullptr)
	, m_pBloomRevealCS(nullptr)

	, m_HorizontalBlurCS(nullptr)
	, m_VerticalBlurCS(nullptr)
	, m_pFullScreenQuadVS(nullptr)
	, m_pFinalPassPS(nullptr)
{
	 m_pTempRT[0] = nullptr;
	 m_pTempRT[1] = nullptr;
	m_pTempSRV[0] = nullptr;
	m_pTempSRV[1] = nullptr;
	m_pTempUAV[0] = nullptr;
	m_pTempUAV[1] = nullptr;
}

CPostFX::~CPostFX()
{

}

HRESULT CPostFX::Init(ID3D11Device *pd3dDevice, UINT width, UINT height)
{
	Deinit();

	HRESULT hr;

	// Find the amount of thread groups needed for the downscale operation
	m_nWidth = width;
	m_nHeight = height;
	m_nDownScaleGroups = (UINT)ceil((float)(m_nWidth * m_nHeight / 16) / 1024.0f);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate the downscaled target
	D3D11_TEXTURE2D_DESC dtd = 
	{
		m_nWidth / 4, //UINT Width;
		m_nHeight / 4, //UINT Height;
		1, //UINT MipLevels;
		1, //UINT ArraySize;
		DXGI_FORMAT_R16G16B16A16_TYPELESS, //DXGI_FORMAT Format;
		1, //DXGI_SAMPLE_DESC SampleDesc;
		0,
		D3D11_USAGE_DEFAULT,//D3D11_USAGE Usage;
		D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_UNORDERED_ACCESS,//UINT BindFlags;
		0,//UINT CPUAccessFlags;
		0//UINT MiscFlags;    
        };
	V_RETURN(pd3dDevice->CreateTexture2D( &dtd, NULL, &m_pDownScaleRT ) );
	Radar::DXUT_SetDebugName( m_pDownScaleRT, "PostFX - Down Scaled RT" );

		// Create the resource views
	D3D11_SHADER_RESOURCE_VIEW_DESC dsrvd;
	ZeroMemory(&dsrvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	dsrvd.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	dsrvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	dsrvd.Texture2D.MipLevels = 1;
	V_RETURN( pd3dDevice->CreateShaderResourceView( m_pDownScaleRT, &dsrvd, &m_pDownScaleSRV ) );
	Radar::DXUT_SetDebugName( m_pDownScaleSRV, "PostFX - Down Scaled SRV" );

	// Create the UAVs
	D3D11_UNORDERED_ACCESS_VIEW_DESC DescUAV;
	ZeroMemory( &DescUAV, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC) );
	DescUAV.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	DescUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	DescUAV.Buffer.FirstElement = 0;
	DescUAV.Buffer.NumElements = m_nWidth * m_nHeight / 16;
	V_RETURN( pd3dDevice->CreateUnorderedAccessView( m_pDownScaleRT, &DescUAV, &m_pDownScaleUAV ) );
	Radar::DXUT_SetDebugName( m_pDownScaleUAV, "PostFX - Down Scaled UAV" );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate temporary target
	V_RETURN( pd3dDevice->CreateTexture2D( &dtd, NULL, &m_pTempRT[0] ) );
	Radar::DXUT_SetDebugName( m_pTempRT[0], "PostFX - Temp 0 RT" );

	V_RETURN( pd3dDevice->CreateShaderResourceView( m_pTempRT[0], &dsrvd, &m_pTempSRV[0] ) );
	Radar::DXUT_SetDebugName( m_pTempSRV[0], "PostFX - Temp 0 SRV" );

	V_RETURN( pd3dDevice->CreateUnorderedAccessView( m_pTempRT[0], &DescUAV, &m_pTempUAV[0] ) );
	Radar::DXUT_SetDebugName( m_pTempUAV[0], "PostFX - Temp 0 UAV" );

	V_RETURN( pd3dDevice->CreateTexture2D( &dtd, NULL, &m_pTempRT[1] ) );
	Radar::DXUT_SetDebugName( m_pTempRT[1], "PostFX - Temp 1 RT" );

	V_RETURN( pd3dDevice->CreateShaderResourceView( m_pTempRT[1], &dsrvd, &m_pTempSRV[1] ) );
	Radar::DXUT_SetDebugName( m_pTempSRV[1], "PostFX - Temp 1 SRV" );

	V_RETURN( pd3dDevice->CreateUnorderedAccessView( m_pTempRT[1], &DescUAV, &m_pTempUAV[1] ) );
	Radar::DXUT_SetDebugName( m_pTempUAV[1], "PostFX - Temp 1 UAV" );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate bloom target
	V_RETURN( pd3dDevice->CreateTexture2D( &dtd, NULL, &m_pBloomRT ) );
	Radar::DXUT_SetDebugName( m_pBloomRT, "PostFX - Bloom RT" );

	V_RETURN( pd3dDevice->CreateShaderResourceView( m_pBloomRT, &dsrvd, &m_pBloomSRV ) );
	Radar::DXUT_SetDebugName( m_pBloomSRV, "PostFX - Bloom SRV" );

	V_RETURN( pd3dDevice->CreateUnorderedAccessView( m_pBloomRT, &DescUAV, &m_pBloomUAV ) );
	Radar::DXUT_SetDebugName( m_pBloomUAV, "PostFX - Bloom UAV" );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate down scaled luminance buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.StructureByteStride = sizeof(float);
	bufferDesc.ByteWidth = m_nDownScaleGroups * bufferDesc.StructureByteStride;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	V_RETURN( pd3dDevice->CreateBuffer( &bufferDesc, NULL, &m_pDownScale1DBuffer ) );
	Radar::DXUT_SetDebugName( m_pDownScale1DBuffer, "P_m_pDownScale1DBuffer" );

	ZeroMemory( &DescUAV, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC) );
	DescUAV.Format = DXGI_FORMAT_UNKNOWN;
	DescUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	DescUAV.Buffer.FirstElement = 0;
	DescUAV.Buffer.NumElements = m_nDownScaleGroups;
	V_RETURN( pd3dDevice->CreateUnorderedAccessView( m_pDownScale1DBuffer, &DescUAV, &m_pDownScale1DUAV ) );
	Radar::DXUT_SetDebugName( m_pDownScale1DUAV, "P_m_pDownScale1DUAV" );

	dsrvd.Format = DXGI_FORMAT_UNKNOWN;
	dsrvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	dsrvd.Buffer.FirstElement = 0;
	dsrvd.Buffer.NumElements = m_nDownScaleGroups;
	V_RETURN( pd3dDevice->CreateShaderResourceView( m_pDownScale1DBuffer, &dsrvd, &m_pDownScale1DSRV ) );
	Radar::DXUT_SetDebugName( m_pDownScale1DSRV, "P_m_pDownScale1DSRV" );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate average luminance buffer
	bufferDesc.ByteWidth = sizeof(float);
	V_RETURN( pd3dDevice->CreateBuffer( &bufferDesc, NULL, &m_pAvgLumBuffer ) );
	Radar::DXUT_SetDebugName( m_pAvgLumBuffer, "P_m_pAvgLumBuffer" );
	
	DescUAV.Buffer.NumElements = 1;
	V_RETURN( pd3dDevice->CreateUnorderedAccessView( m_pAvgLumBuffer, &DescUAV, &m_pAvgLumUAV ) );
	Radar::DXUT_SetDebugName( m_pAvgLumUAV, "P_m_pAvgLumUAV" );

	dsrvd.Buffer.NumElements = 1;
	V_RETURN( pd3dDevice->CreateShaderResourceView( m_pAvgLumBuffer, &dsrvd, &m_pAvgLumSRV ) );
	Radar::DXUT_SetDebugName( m_pAvgLumSRV, "P_m_pAvgLumSRV" );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate previous frame average luminance buffer
	V_RETURN( pd3dDevice->CreateBuffer( &bufferDesc, NULL, &m_pPrevAvgLumBuffer ) );
	Radar::DXUT_SetDebugName( m_pPrevAvgLumBuffer, "P_m_pPrevAvgLumBuffer" );

	V_RETURN( pd3dDevice->CreateUnorderedAccessView( m_pPrevAvgLumBuffer, &DescUAV, &m_pPrevAvgLumUAV ) );
	Radar::DXUT_SetDebugName( m_pPrevAvgLumUAV, "P_m_pPrevAvgLumUAV" );

	V_RETURN( pd3dDevice->CreateShaderResourceView( m_pPrevAvgLumBuffer, &dsrvd, &m_pPrevAvgLumSRV ) );
	Radar::DXUT_SetDebugName( m_pPrevAvgLumSRV, "P_m_pPrevAvgLumSRV" );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate constant buffers
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(TDownScaleCB);
	V_RETURN( pd3dDevice->CreateBuffer( &bufferDesc, NULL, &m_pDownScaleCB ) );
	Radar::DXUT_SetDebugName( m_pDownScaleCB, "Pm_pDownScaleCB" );

	bufferDesc.ByteWidth = sizeof(TFinalPassCB);
	V_RETURN( pd3dDevice->CreateBuffer( &bufferDesc, NULL, &m_pFinalPassCB ) );
	Radar::DXUT_SetDebugName( m_pFinalPassCB, "P_m_pFinalPassCB" );

	bufferDesc.ByteWidth = sizeof(TBlurCB);
	V_RETURN( pd3dDevice->CreateBuffer( &bufferDesc, NULL, &m_pBlurCB ) );
	Radar::DXUT_SetDebugName( m_pBlurCB, "P_m_pBlurCB" );

	// Compile the shaders
    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;// | D3DCOMPILE_WARNINGS_ARE_ERRORS;
	#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;
	#endif

	// Create the two samplers
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;
	V_RETURN(pd3dDevice->CreateSamplerState(&samDesc, &m_pSampLinear));
	Radar::DXUT_SetDebugName(m_pSampLinear, "Linear");

	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	V_RETURN(pd3dDevice->CreateSamplerState(&samDesc, &m_pSampPoint));
	Radar::DXUT_SetDebugName(m_pSampPoint, "Point");

	Read_CSO_Shader(pd3dDevice);

	return S_OK;
}

void CPostFX::Deinit()
{
	SAFE_RELEASE( m_pDownScaleRT );
	SAFE_RELEASE( m_pDownScaleSRV );
	SAFE_RELEASE( m_pDownScaleUAV );
	SAFE_RELEASE( m_pTempRT[0] );
	SAFE_RELEASE( m_pTempSRV[0] );
	SAFE_RELEASE( m_pTempUAV[0] );
	SAFE_RELEASE( m_pTempRT[1] );
	SAFE_RELEASE( m_pTempSRV[1] );
	SAFE_RELEASE( m_pTempUAV[1] );
	SAFE_RELEASE( m_pBloomRT );
	SAFE_RELEASE( m_pBloomSRV );
	SAFE_RELEASE( m_pBloomUAV );
	SAFE_RELEASE( m_pDownScale1DBuffer );
	SAFE_RELEASE( m_pDownScale1DUAV );
	SAFE_RELEASE( m_pDownScale1DSRV );
	SAFE_RELEASE( m_pDownScaleCB );
	SAFE_RELEASE( m_pFinalPassCB );
	SAFE_RELEASE( m_pBlurCB );
	SAFE_RELEASE( m_pAvgLumBuffer );
	SAFE_RELEASE( m_pAvgLumUAV );
	SAFE_RELEASE( m_pAvgLumSRV );
	SAFE_RELEASE( m_pPrevAvgLumBuffer );
	SAFE_RELEASE( m_pPrevAvgLumUAV );
	SAFE_RELEASE( m_pPrevAvgLumSRV );
	SAFE_RELEASE( m_pDownScaleFirstPassCS );
	SAFE_RELEASE( m_pDownScaleSecondPassCS );
	SAFE_RELEASE( m_pBloomRevealCS );
	SAFE_RELEASE( m_HorizontalBlurCS );
	SAFE_RELEASE( m_VerticalBlurCS );
	SAFE_RELEASE( m_pFullScreenQuadVS );
	SAFE_RELEASE( m_pFinalPassPS );
}

void CPostFX::PostProcessing
(
	  ID3D11DeviceContext*		pd3dImmediateContext
	, ID3D11ShaderResourceView* pd3dsrvLDR
	, ID3D11RenderTargetView*	pd3dRenderTargetView
)

{
	// Constants
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	pd3dImmediateContext->Map(m_pDownScaleCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	TDownScaleCB* pDownScale = (TDownScaleCB*)MappedResource.pData; 
	pDownScale->nWidth = m_nWidth  / 4;
	pDownScale->nHeight = m_nHeight / 4;
	pDownScale->nTotalPixels = pDownScale->nWidth * pDownScale->nHeight;
	pDownScale->nGroupSize = m_nDownScaleGroups;
	pDownScale->fAdaptation = m_fAdaptation;
	pDownScale->fBloomThreshold = m_fBloomThreshold;
	pd3dImmediateContext->Unmap(m_pDownScaleCB, 0);
	ID3D11Buffer* arrConstBuffers[1] = { m_pDownScaleCB };
	pd3dImmediateContext->CSSetConstantBuffers(0, 1, arrConstBuffers);

	// Down scale the HDR image
	ID3D11RenderTargetView* rt[1] = { NULL };
	pd3dImmediateContext->OMSetRenderTargets( 1, rt, NULL );
	DownScale(pd3dImmediateContext, pd3dsrvLDR);

	// Bloom
	Bloom(pd3dImmediateContext);

	// Blur the bloom values
	Blur(pd3dImmediateContext, m_pTempSRV[0], m_pBloomUAV);

	// Cleanup
	ZeroMemory( &arrConstBuffers, sizeof(arrConstBuffers) );
	pd3dImmediateContext->CSSetConstantBuffers(0, 1, arrConstBuffers);

	// Do the final pass
	pd3dImmediateContext->OMSetRenderTargets( 1, &pd3dRenderTargetView, nullptr );
	FinalPass(pd3dImmediateContext, pd3dsrvLDR);

	// Swap the previous frame average luminance
	ID3D11Buffer* pTempBuffer = m_pAvgLumBuffer;
	ID3D11UnorderedAccessView* pTempUAV = m_pAvgLumUAV;
	ID3D11ShaderResourceView* p_TempSRV = m_pAvgLumSRV;
	m_pAvgLumBuffer = m_pPrevAvgLumBuffer;
	m_pAvgLumUAV = m_pPrevAvgLumUAV;
	m_pAvgLumSRV = m_pPrevAvgLumSRV;
	m_pPrevAvgLumBuffer = pTempBuffer;
	m_pPrevAvgLumUAV = pTempUAV;
	m_pPrevAvgLumSRV = p_TempSRV;
}

void CPostFX::Read_CSO_Shader(ID3D11Device *pd3dDevice)
{
	//-------------------------- 
	#define _USE_CSO
	#ifdef _USE_CSO

		CShader::CreateComputeShaderFromCompiledFile(pd3dDevice, TEXT("CSDownScaleFirstPass.cso"), &m_pDownScaleFirstPassCS);
		Radar::DXUT_SetDebugName(m_pDownScaleFirstPassCS, "Post FX - Down Scale First Pass CS");

		CShader::CreateComputeShaderFromCompiledFile(pd3dDevice, TEXT("CSDownScaleSecondPass.cso"), &m_pDownScaleSecondPassCS);
		Radar::DXUT_SetDebugName(m_pDownScaleSecondPassCS, "Post FX - Down Scale Second Pass CS");

		CShader::CreateComputeShaderFromCompiledFile(pd3dDevice, TEXT("CSBloom.cso"), &m_pBloomRevealCS);
		Radar::DXUT_SetDebugName(m_pBloomRevealCS, "Post FX - Bloom Reveal CS");

		CShader::CreateComputeShaderFromCompiledFile(pd3dDevice, TEXT("CSHorizontalBlur.cso"), &m_HorizontalBlurCS);
		Radar::DXUT_SetDebugName(m_HorizontalBlurCS, "Post FX - Horizontal Blur CS");

		CShader::CreateComputeShaderFromCompiledFile(pd3dDevice, TEXT("CSVerticalBlur.cso"), &m_VerticalBlurCS);
		Radar::DXUT_SetDebugName(m_VerticalBlurCS, "Post FX - Vertical Blur CS");


		UINT nSlot = 0;
		D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		UINT nElements = ARRAYSIZE(d3dInputElements);

		CShader::CreateVertexShaderFromCompiledFile(pd3dDevice, TEXT("VSPostProcessing.cso"), d3dInputElements, nElements, &m_pFullScreenQuadVS);
		Radar::DXUT_SetDebugName(m_pFullScreenQuadVS, "Post FX - Vertical Blur CS");

		CShader::CreatePixelShaderFromCompiledFile(pd3dDevice, TEXT("PSPostProcessing.cso"), &m_pFinalPassPS);
		Radar::DXUT_SetDebugName(m_pFinalPassPS, "Post FX - Final Pass PS");

	#else

	CShader::CreateComputeShaderFromFile(pd3dDevice, L"PostDownScaleFX.hlsl", "DownScaleFirstPass", "cs_5_0", &m_pDownScaleFirstPassCS);
	Radar::DXUT_SetDebugName(m_pDownScaleFirstPassCS, "m_pDownScaleFirstPassCS");

	CShader::CreateComputeShaderFromFile(pd3dDevice, L"PostDownScaleFX.hlsl", "DownScaleSecondPass", "cs_5_0", &m_pDownScaleSecondPassCS);
	Radar::DXUT_SetDebugName(m_pDownScaleSecondPassCS, "m_pDownScaleSecondPassCS");

	CShader::CreateComputeShaderFromFile(pd3dDevice, L"PostDownScaleFX.hlsl", "BloomReveal", "cs_5_0", &m_pBloomRevealCS);
	Radar::DXUT_SetDebugName(m_pBloomRevealCS, "m_pBloomRevealCS");

	//--------------
	CShader::CreateComputeShaderFromFile(pd3dDevice, L"Blur.hlsl", "VerticalFilter", "cs_5_0", &m_HorizontalBlurCS);
	Radar::DXUT_SetDebugName(m_HorizontalBlurCS, "m_HorizontalBlurCS");

	CShader::CreateComputeShaderFromFile(pd3dDevice, L"Blur.hlsl", "HorizFilter", "cs_5_0", &m_VerticalBlurCS);
	Radar::DXUT_SetDebugName(m_VerticalBlurCS, "m_VerticalBlurCS");


	UINT nSlot = 0;
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT nElements = ARRAYSIZE(d3dInputElements);

	CShader::CreateVertexShaderFromFile(pd3dDevice, L"PostFX.hlsl", "FullScreenQuadVS", "vs_5_0", &m_pFullScreenQuadVS, d3dInputElements, nElements, &m_pd3dVertexLayout);
	Radar::DXUT_SetDebugName(m_pFullScreenQuadVS, "m_pFullScreenQuadVS");

	//CShader::CreatePixelShaderFromCompiledFile(pd3dDevice, TEXT("PSPostProcessing.cso"), &m_pFinalPassPS);
	CShader::CreatePixelShaderFromFile(pd3dDevice, L"PostFX.hlsl", "FinalPassPS", "ps_5_0", &m_pFinalPassPS);
	Radar::DXUT_SetDebugName(m_pFinalPassPS, "m_pFinalPassPS");

	//----------------------------------


#endif
}

void CPostFX::DownScale(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ShaderResourceView* pLDRSRV)
{
	// Output
	ID3D11UnorderedAccessView* arrUAVs[2] = { m_pDownScale1DUAV, m_pDownScaleUAV };
	pd3dImmediateContext->CSSetUnorderedAccessViews( 0, 2, arrUAVs, NULL );

	// Input
	ID3D11ShaderResourceView* arrViews[2] = { pLDRSRV, NULL };
	pd3dImmediateContext->CSSetShaderResources(0, 1, arrViews);

	// Shader
	pd3dImmediateContext->CSSetShader( m_pDownScaleFirstPassCS, NULL, 0 );

	// Execute the downscales first pass with enough groups to cover the entire full res HDR buffer
	pd3dImmediateContext->Dispatch(m_nDownScaleGroups, 1, 1);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Second pass - reduce to a single pixel

	// Outoput
	ZeroMemory(arrUAVs, sizeof(arrUAVs));
	arrUAVs[0] = m_pAvgLumUAV;
	pd3dImmediateContext->CSSetUnorderedAccessViews( 0, 2, arrUAVs, NULL );

	// Input
	arrViews[0] = m_pDownScale1DSRV;
	arrViews[1] = m_pPrevAvgLumSRV;
	pd3dImmediateContext->CSSetShaderResources(0, 2, arrViews);

	// Shader
	pd3dImmediateContext->CSSetShader( m_pDownScaleSecondPassCS, NULL, 0 );

	// Excute with a single group - this group has enough threads to process all the pixels
	pd3dImmediateContext->Dispatch(1, 1, 1);

	// Cleanup
	pd3dImmediateContext->CSSetShader( NULL, NULL, 0 );
	ZeroMemory(arrViews, sizeof(arrViews));
	pd3dImmediateContext->CSSetShaderResources(0, 2, arrViews);
	ZeroMemory(arrUAVs, sizeof(arrUAVs));
	pd3dImmediateContext->CSSetUnorderedAccessViews( 0, 2, arrUAVs, (UINT*)(&arrUAVs) );
}

void CPostFX::Bloom(ID3D11DeviceContext* pd3dImmediateContext)
{
	// Input
	ID3D11ShaderResourceView* arrViews[2] = { m_pDownScaleSRV, m_pAvgLumSRV };
	pd3dImmediateContext->CSSetShaderResources(0, 2, arrViews);

	// Output
	ID3D11UnorderedAccessView* arrUAVs[1] = { m_pTempUAV[0] };
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 1, arrUAVs, NULL);

	// Shader
	pd3dImmediateContext->CSSetShader( m_pBloomRevealCS, NULL, 0 );

	// Execute the downscales first pass with enough groups to cover the entire full res HDR buffer
	pd3dImmediateContext->Dispatch(m_nDownScaleGroups, 1, 1);

	// Cleanup
	pd3dImmediateContext->CSSetShader( NULL, NULL, 0 );
	ZeroMemory(arrViews, sizeof(arrViews));
	pd3dImmediateContext->CSSetShaderResources(0, 2, arrViews);
	ZeroMemory(arrUAVs, sizeof(arrUAVs));
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 1, arrUAVs, NULL);
}

void CPostFX::Blur(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ShaderResourceView* pInput, ID3D11UnorderedAccessView* pOutput)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Second pass - horizontal gaussian filter

	// Output
	ID3D11UnorderedAccessView* arrUAVs[1] = { m_pTempUAV[1] };
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 1, arrUAVs, NULL);

	// Input
	ID3D11ShaderResourceView* arrViews[1] = { pInput };
	pd3dImmediateContext->CSSetShaderResources(0, 1, arrViews);

	// Shader
	pd3dImmediateContext->CSSetShader(m_HorizontalBlurCS, NULL, 0);

	// Execute the horizontal filter
	pd3dImmediateContext->Dispatch((UINT)ceil((m_nWidth / 4.0f) / (128.0f - 12.0f)), (UINT)ceil(m_nHeight / 4.0f), 1);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// First pass - vertical gaussian filter

	// Output
	arrUAVs[0] = pOutput;
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 1, arrUAVs, NULL );

	// Input
	arrViews[0] = m_pTempSRV[1];
	pd3dImmediateContext->CSSetShaderResources(0, 1, arrViews);

	// Shader
	pd3dImmediateContext->CSSetShader(m_VerticalBlurCS, NULL, 0);

	// Execute the vertical filter
	pd3dImmediateContext->Dispatch((UINT)ceil(m_nWidth / 4.0f), (UINT)ceil((m_nHeight / 4.0f) / (128.0f - 12.0f)), 1);

	// Cleanup
	pd3dImmediateContext->CSSetShader(NULL, NULL, 0);
	ZeroMemory(arrViews, sizeof(arrViews));
	pd3dImmediateContext->CSSetShaderResources(0, 1, arrViews);
	ZeroMemory(arrUAVs, sizeof(arrUAVs));
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 1, arrUAVs, NULL);
}

void CPostFX::FinalPass(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ShaderResourceView* pLDRSRV)
{
	ID3D11ShaderResourceView* arrViews[3] = { pLDRSRV, m_pAvgLumSRV, m_pBloomSRV};
	pd3dImmediateContext->PSSetShaderResources(0, 3, arrViews);

	// Constants
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	pd3dImmediateContext->Map(m_pFinalPassCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	TFinalPassCB* pFinalPass = (TFinalPassCB*)MappedResource.pData; 
	pFinalPass->fMiddleGrey = m_fMiddleGrey;
	pFinalPass->fLumWhiteSqr = m_fWhite;
	pFinalPass->fLumWhiteSqr *= pFinalPass->fMiddleGrey; // Scale by the middle grey value
	pFinalPass->fLumWhiteSqr *= pFinalPass->fLumWhiteSqr; // Square
	pFinalPass->fBloomScale = m_fBloomScale;
	pd3dImmediateContext->Unmap(m_pFinalPassCB, 0);
	ID3D11Buffer* arrConstBuffers[1] = { m_pFinalPassCB };
	pd3dImmediateContext->PSSetConstantBuffers(PS_HDR_CB_SLOT_FINAL_PASS_CONTANTS, 1, arrConstBuffers);
	

	ID3D11SamplerState* arrSamplers[2] = { m_pSampPoint, m_pSampLinear };
	pd3dImmediateContext->PSSetSamplers( 0, 2, arrSamplers );
	
	// Set the shaders
	pd3dImmediateContext->VSSetShader(m_pFullScreenQuadVS, NULL, 0);
	pd3dImmediateContext->PSSetShader(m_pFinalPassPS, NULL, 0);

	pd3dImmediateContext->Draw(4, 0);

	// Cleanup
	ZeroMemory(arrViews, sizeof(arrViews));
	pd3dImmediateContext->PSSetShaderResources(0, 3, arrViews);
	ZeroMemory(arrConstBuffers, sizeof(arrConstBuffers));
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, arrConstBuffers);
	pd3dImmediateContext->VSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->PSSetShader(NULL, NULL, 0);
}



void Test()
{
	for (int i = 0; i < 10; ++i)
		printf("%d", i);

	for (auto i = 0; i < 10; ++i)
		printf("%d", i);


	int k = 10;
	auto p = 10;


	/*
	  여기서
		 a는 lvaule; (왼쪽값이라고도 해요)
	     k은 rvaule; (오른값이라고도 해요)
	*/
	int a = k;

	// 그래서 auto는 rvaule를 가지고 lvaule type을 유추합니다.

}