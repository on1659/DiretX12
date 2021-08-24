#pragma once


//후에 OrthView로도 가능할듯
struct Screen_VS_INPUT
{
	XMFLOAT4 d3dxPosition;
	XMFLOAT2 d3dxTextCoord;
};

struct Screen_BlurType { XMFLOAT4 blurType; };


//다중 렌더링
class CScreenShader : public CShader
{
private:

	//CTexture로 하니 반대바얗ㅇ으로나옴
	int									m_nTextures;
	ID3D11ShaderResourceView			**m_ppd3dsrvTextures;
	int									m_nTextureStartSlot;

	int									m_nSamplers;
	ID3D11SamplerState					**m_ppd3dSamplerStates;
	int									m_nSamplerStartSlot;

	ID3D11Buffer						*m_pd3dVertexBuffer;
	UINT								*m_pnVertexStrides;
	UINT								*m_pnVertexOffsets;

	ID3D11Buffer						*m_pd3dBlurConstBuffer;
	float								 m_fFlashSpeed;
	float								 m_fRespawnStartTime;
	float								 m_fRespawnDuringTime;

	XMFLOAT4							m_vsScreen[4];
	WCHAR*								m_strScreenPSShader;
	bool								m_bRTVTexture;

public:
	//KYT '16.03.20
	/*
		CSShader를 통한 Deffered Shader
	*/
	//KYT '16.03.20
	ID3D11RenderTargetView				*m_pd3dRTV_OffScreen;

	ID3D11ShaderResourceView			*m_pHorizonSRV;
	ID3D11UnorderedAccessView			*m_pHorizonUAV;
	ID3D11Texture2D						*m_pHorizonTexture2D;

	ID3D11ShaderResourceView			*m_pd3dSRV_OffScreen;
	ID3D11UnorderedAccessView			*m_pd3dUAV_OffScreen;
	ID3D11Texture2D						*m_pd3dComputTexture2D;

	//ID3D11ComputeShader*				m_pCSMainShader;			//Compute   - Shader
	ID3D11ComputeShader*				m_pHorizontalBlurShader;	//Compute   - Shader
	///ID3D11ComputeShader*				m_pVerticalBlurShader;		//Compute   - Shader



	//KYT '16.03.08
	/*
		CSShader를 통한 Deffered Shader
	*/
	ID3D11ShaderResourceView		*m_pd3dsrvDefferedTexture;
	ID3D11UnorderedAccessView		*m_pd3duavDefferedTexture;


public:
	CScreenShader(int nTextures, int nSamplers, int nTextureStartSlot, int nSamplerStartSlot);
	virtual ~CScreenShader();
	virtual void ReleaseObject();
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateComputeShader(ID3D11Device *pd3dDevice);
	virtual void BuildObjects(ID3D11Device *pd3dDevice, void *pContext = nullptr);
	virtual void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);
	void ComputeLoad(ID3D11Device *pd3dDevice);
	void ReSetShader(ID3D11Device *pd3dDevice);


	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera = nullptr);
	virtual void ComputeRender(ID3D11DeviceContext *pd3dDeviceContext, ID3D11ShaderResourceView* pd3dsrvDiffuse);
	void SetTexture(int nIndex, ID3D11ShaderResourceView *pd3dsrvTexture);

	ID3D11ShaderResourceView** GetSRV()
	{
		return m_ppd3dsrvTextures;
	}
	ID3D11ShaderResourceView* GetSRV(const int& i)
	{
		return m_ppd3dsrvTextures[i];
	}
	void SetScreenPos(XMFLOAT4* f4)
	{
		for (int i = 0; i < 4; ++i)
			m_vsScreen[i] = f4[i];
	}
	void SetPSShaderName(WCHAR* msg) 
	{
		m_strScreenPSShader = msg;
		m_bRTVTexture = true;
	}
};
//다중 렌더링
