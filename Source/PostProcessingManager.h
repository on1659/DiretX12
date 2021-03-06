#pragma once

class CPostProcessingManager : public CShader
{
public:

	CPostProcessingManager();

	~CPostProcessingManager();

	bool Load(ID3D11Device* pd3dDevice, const int& width, const int& height);

	void ReleaseShaderVariables() override;

	// Entry point for post processing
	void PostProcessing
	(
		ID3D11DeviceContext*		pd3DeviceContext
		, ID3D11ShaderResourceView* pd3dsrvLDR
		, ID3D11RenderTargetView*	pd3dRenderTargetView
	);

	void Read_CSO_Shader(ID3D11Device* pd3dDevice);

	void SetParameters(const float& fMiddleGrey, const float& fWhite, const float& fAdaptation, const float& fBloomThreshold, const float& fBloomScale) { m_fMiddleGrey = fMiddleGrey; m_fWhite = fWhite; m_fAdaptation = fAdaptation; m_fBloomThreshold = fBloomThreshold; m_fBloomScale = fBloomScale; }

private:

	// Down scale the full size HDR image
	void DownScale(ID3D11DeviceContext* pd3DeviceContext, ID3D11ShaderResourceView* pLDRSRV);

	// Extract the bloom values from the downscaled image
	void Bloom(ID3D11DeviceContext* pd3DeviceContext);

	// Apply a gaussian blur to the input and store it in the output
	void Blur(ID3D11DeviceContext* pd3DeviceContext, ID3D11ShaderResourceView* pInput, ID3D11UnorderedAccessView* pOutput);

	// Final pass that composites all the post processing calculations
	void FinalPass(ID3D11DeviceContext* pd3DeviceContext, ID3D11ShaderResourceView* pLDRSRV);

	// Downscaled scene texture
	ID3D11Texture2D* m_pDownScaleRT;
	ID3D11ShaderResourceView* m_pDownScaleSRV;
	ID3D11UnorderedAccessView* m_pDownScaleUAV;

	// 1D intermediate storage for the down scale operation
	ID3D11Buffer* m_pDownScale1DBuffer;
	ID3D11UnorderedAccessView* m_pDownScale1DUAV;
	ID3D11ShaderResourceView* m_pDownScale1DSRV;

	// Temporary texture
	ID3D11Texture2D* m_pTempRT[2];
	ID3D11ShaderResourceView* m_pTempSRV[2];
	ID3D11UnorderedAccessView* m_pTempUAV[2];

	// Bloom texture
	ID3D11Texture2D* m_pBloomRT;
	ID3D11ShaderResourceView* m_pBloomSRV;
	ID3D11UnorderedAccessView* m_pBloomUAV;

	// Average luminance
	ID3D11Buffer* m_pAvgLumBuffer;
	ID3D11UnorderedAccessView* m_pAvgLumUAV;
	ID3D11ShaderResourceView* m_pAvgLumSRV;

	// Previous average luminance for adaptation
	ID3D11Buffer* m_pPrevAvgLumBuffer;
	ID3D11UnorderedAccessView* m_pPrevAvgLumUAV;
	ID3D11ShaderResourceView* m_pPrevAvgLumSRV;

	UINT m_nWidth;
	UINT m_nHeight;
	UINT m_nDownScaleGroups;
	float m_fMiddleGrey;
	float m_fWhite;
	float m_fAdaptation;
	float m_fBloomThreshold;
	float m_fBloomScale;

	typedef struct
	{
		UINT nWidth;
		UINT nHeight;
		UINT nTotalPixels;
		UINT nGroupSize;
		float fAdaptation;
		float fBloomThreshold;
		UINT pad[2];
	} TDownScaleCB;
	ID3D11Buffer* m_pDownScaleCB;

	typedef struct
	{
		float fMiddleGrey;
		float fLumWhiteSqr;
		float fBloomScale;
		UINT padding;
	} TFinalPassCB;
	ID3D11Buffer* m_pFinalPassCB;

	typedef struct
	{
		UINT numApproxPasses;
		float fHalfBoxFilterWidth;			// w/2
		float fFracHalfBoxFilterWidth;		// frac(w/2+0.5)
		float fInvFracHalfBoxFilterWidth;	// 1-frac(w/2+0.5)
		float fRcpBoxFilterWidth;			// 1/w
		UINT pad[3];
	} TBlurCB;
	ID3D11Buffer* m_pBlurCB;

	// Shaders
	ID3D11ComputeShader* m_pDownScaleFirstPassCS;
	ID3D11ComputeShader* m_pDownScaleSecondPassCS;
	ID3D11ComputeShader* m_pBloomRevealCS;
	ID3D11ComputeShader* m_HorizontalBlurCS;
	ID3D11ComputeShader* m_VerticalBlurCS;
	ID3D11VertexShader*  m_pFullScreenQuadVS;
	ID3D11PixelShader*   m_pFinalPassPS;

	// Direct3D 11 resources
	ID3D11SamplerState*         m_pSampLinear = NULL;
	ID3D11SamplerState*         m_pSampPoint = NULL;


	void BokehLoad(ID3D11Device* pd3dDevice);

	// Bokeh
	ID3D11ComputeShader*	m_BokehHighlightSearchCS;
	ID3D11VertexShader*		m_pBokehVS;
	ID3D11GeometryShader*	m_pBokehGS;
	ID3D11PixelShader*		m_pBokehPS;

	ID3D11Buffer* m_pBokehBuffer;
	ID3D11UnorderedAccessView* m_pBokehUAV;
	ID3D11ShaderResourceView* m_pBokehSRV;

	typedef struct
	{
		UINT nWidth;
		UINT nHeight;
		float ProjectionValues[2];
		float fDOFFarStart;
		float fDOFFarRangeRcp;
		float fMiddleGrey;
		float fLumWhiteSqr;
		float fBokehBlurThreshold;
		float fBokehLumThreshold;
		float fRadiusScale;
		float fColorScale;
	} TBokehHightlightScanCB;
	ID3D11Buffer* m_pBokehHightlightScanCB;
	
};




