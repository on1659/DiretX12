#include "stdafx.h"
#include "ScreenShader.h"
#include "GameFramework.h"
//다중 렌더타겟

CScreenShader::CScreenShader(int nTextures, int nSamplers, int nTextureStartSlot, int nSamplerStartSlot)
{
	m_nTextures = nTextures;
	m_ppd3dsrvTextures = new ID3D11ShaderResourceView*[m_nTextures];
	for (int i = 0; i < m_nTextures; i++) m_ppd3dsrvTextures[i] = nullptr;
	m_nTextureStartSlot = nTextureStartSlot;

	m_nSamplers = nSamplers;
	m_ppd3dSamplerStates = new ID3D11SamplerState*[m_nSamplers];
	for (int i = 0; i < m_nSamplers; i++) m_ppd3dSamplerStates[i] = nullptr;
	m_nSamplerStartSlot = nSamplerStartSlot;

	//샘플러 초기화
	m_pnVertexStrides = nullptr;
	m_pnVertexOffsets = nullptr;
	m_pd3dRasterizerState = nullptr;

	m_vsScreen[0] = XMFLOAT4(-1, -1, 0, 1);
	m_vsScreen[1] = XMFLOAT4(-1, 1, 0, 1);
	m_vsScreen[2] = XMFLOAT4(1, -1, 0, 1);
	m_vsScreen[3] = XMFLOAT4(1, 1, 0, 1);

	m_strScreenPSShader = TEXT("../Assets/07.Shader/DefferedShader/PSScreenShader.cso");

	m_bRTVTexture = false;
}

CScreenShader::~CScreenShader()
{
}

void CScreenShader::ReleaseObject()
{
	for (int i = 0; i < m_nTextures; i++) if (m_ppd3dsrvTextures[i]) m_ppd3dsrvTextures[i]->Release();
	for (int i = 0; i < m_nSamplers; i++) if (m_ppd3dSamplerStates[i]) m_ppd3dSamplerStates[i]->Release();
	if (m_ppd3dsrvTextures) delete[] m_ppd3dsrvTextures;
	if (m_ppd3dSamplerStates) delete[] m_ppd3dSamplerStates;
	m_ppd3dsrvTextures = nullptr;
	m_ppd3dSamplerStates = nullptr;

	if (m_pd3dRasterizerState) m_pd3dRasterizerState->Release();
	m_pd3dRasterizerState = nullptr;
	m_pd3dComputTexture2D = nullptr;

	m_pd3dRTV_OffScreen = nullptr;
	m_pd3dSRV_OffScreen = nullptr;
	m_pd3dUAV_OffScreen = nullptr;

	if (m_pnVertexOffsets) delete m_pnVertexOffsets;
	m_pnVertexOffsets = nullptr;


	if (m_pnVertexStrides) delete m_pnVertexStrides;
	m_pnVertexStrides = nullptr;


}



void CScreenShader::SetTexture(int nIndex, ID3D11ShaderResourceView *pd3dsrvTexture)
{
	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->Release();
	m_ppd3dsrvTextures[nIndex] = pd3dsrvTexture;
	if (pd3dsrvTexture) pd3dsrvTexture->AddRef();
}

void CScreenShader::CreateShader(ID3D11Device *pd3dDevice)
{
	m_fFlashSpeed		 =  0.05f;
	m_fRespawnStartTime  =  0.0f;
	m_fRespawnDuringTime = 10.0f;
	Screen_BlurType* blur = new Screen_BlurType();
	blur->blurType = gBulrMode;

	//Create Bulr Const Buffer 
	D3D11_BUFFER_DESC blurbd;
	ZeroMemory(&blurbd, sizeof(blurbd));
	blurbd.Usage = D3D11_USAGE_DYNAMIC;
	blurbd.ByteWidth = sizeof(Screen_BlurType);
	blurbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	blurbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dFogBufferData;
	ZeroMemory(&d3dFogBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dFogBufferData.pSysMem = blur;
	pd3dDevice->CreateBuffer(&blurbd, &d3dFogBufferData, &m_pd3dBlurConstBuffer);


	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &m_ppd3dSamplerStates[0]);

	//화면 좌표계 4개 준다
	Screen_VS_INPUT v[4];
	v[0].d3dxPosition = m_vsScreen[0];
	v[1].d3dxPosition = m_vsScreen[1];
	v[2].d3dxPosition = m_vsScreen[2];
	v[3].d3dxPosition = m_vsScreen[3];

	v[0].d3dxTextCoord = XMFLOAT2(0, 1);
	v[1].d3dxTextCoord = XMFLOAT2(0, 0);
	v[2].d3dxTextCoord = XMFLOAT2(1, 1);
	v[3].d3dxTextCoord = XMFLOAT2(1, 0);

	//버텍스 버퍼 생성
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(Screen_VS_INPUT) * 4;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = v;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);


	m_pnVertexStrides = new UINT[1];
	m_pnVertexOffsets = new UINT[1];
	m_pnVertexStrides[0] = sizeof(Screen_VS_INPUT);
	m_pnVertexOffsets[0] = 0;


	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT nElements = ARRAYSIZE(d3dInputLayout);

	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/DefferedShader/VSScreenShader.cso", d3dInputLayout, nElements, &m_pd3dVertexShader);
	
	
	CreatePixelShaderFromCompiledFile(pd3dDevice, m_strScreenPSShader, &m_pd3dPixelShader);

	if (m_bRTVTexture)
	{
		std::wstring wstr = m_strScreenPSShader;

		std::string str;
		str.assign(wstr.begin(), wstr.end());
		Radar::DXUT_SetDebugName(m_pd3dPixelShader, str.c_str());
	}
	else
		Radar::DXUT_SetDebugName(m_pd3dPixelShader, ("Shadows PSScreenShader "));



	//CreateVertexShaderFromFile(pd3dDevice, L"ScreenShader.fx", "VSTextureToScreen", "vs_5_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	//CreatePixelShaderFromFile(pd3dDevice, L"ScreenShader.fx", "PSTextureToScreen", "ps_5_0", &m_pd3dPixelShader);

	//std::vector<LPCSTR> vShaderNameChar;
	//vShaderNameChar.push_back("HorzBlurCS");
	//vShaderNameChar.push_back("VerBlurCS");
	//
	//for (auto pChar : vShaderNameChar)
	//	CreateComputeShaderFromFile(pd3dDevice, L"ScreenShader.fx", pChar, "cs_5_0", &m_pd3dComputeShader);

}

void CScreenShader::CreateComputeShader(ID3D11Device * pd3dDevice)
{
	CreateComputeShaderFromFile(pd3dDevice, L"ScreenShader.fx", "CSMain", "cs_5_0", &m_pHorizontalBlurShader);
	//CreateComputeShaderFromFile(pd3dDevice, L"ScreenShader.fx", "VerBlurCS", "cs_5_0", &m_pVerticalBlurShader);
	//CreateComputeShaderFromFile(pd3dDevice, L"ComputeShader.fx", "CS", "cs_5_0", &m_pd3dComputeShader);
}

void CScreenShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &m_ppd3dSamplerStates[0]);

	//화면 좌표계 4개 준다
	Screen_VS_INPUT v[4];
	v[0].d3dxPosition = XMFLOAT4(-1, -1, 0, 1);
	v[1].d3dxPosition = XMFLOAT4(-1, 1, 0, 1);
	v[2].d3dxPosition = XMFLOAT4(1, -1, 0, 1);
	v[3].d3dxPosition = XMFLOAT4(1, 1, 0, 1);

	v[0].d3dxTextCoord = XMFLOAT2(0, 1);
	v[1].d3dxTextCoord = XMFLOAT2(0, 0);
	v[2].d3dxTextCoord = XMFLOAT2(1, 1);
	v[3].d3dxTextCoord = XMFLOAT2(1, 0);

	//버텍스 버퍼 생성
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(Screen_VS_INPUT) * 4;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = v;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);


	m_pnVertexStrides = new UINT[1];
	m_pnVertexOffsets = new UINT[1];
	m_pnVertexStrides[0] = sizeof(Screen_VS_INPUT);
	m_pnVertexOffsets[0] = 0;



}

void CScreenShader::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	//무엇인가 변화가 없었다!!
	if (gBulrMode.x == YT_BlurMode::BLUR_NOTHING)
		return;

	if (gBulrMode.w == YT_BlurMode::BLUR_RESPAWN)
	{
		gBulrMode.w = 20;
		m_fRespawnStartTime = TIMEMGR->GetGlobalTime();
	}

	gBulrMode.z -= m_fFlashSpeed;

	if (gBulrMode.z < 1.0f)
	{
		gBulrMode.x = YT_BlurMode::BLUR_NOTHING;
		gBulrMode.z = 1.0f;
	}

	if (TIMEMGR->GetGlobalTime() - m_fRespawnStartTime > m_fRespawnDuringTime)
	{
		gBulrMode.x = YT_BlurMode::BLUR_NOTHING;
		gBulrMode.w = 1.0f;
	}
#ifdef DEBUG_MODE_UI
	FONTMGR->MessagePush("기본", 920, 270, 25, YT_Color(250, 25, 25), L"(%1.f %1.f %1.f %1.f)", gBulrMode.x, gBulrMode.y, gBulrMode.z, gBulrMode.w);
#endif
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dBlurConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	Screen_BlurType *cbBlurType = (Screen_BlurType*)d3dMappedResource.pData;
	cbBlurType->blurType = gBulrMode;
	pd3dDeviceContext->Unmap(m_pd3dBlurConstBuffer, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_BLUR_MODE, 1, &m_pd3dBlurConstBuffer);
}

void CScreenShader::ComputeLoad(ID3D11Device *pd3dDevice)
{
	//
	//KYT '16.03.08
	/*
	UnorderedAccessView
	*/

	{
		m_pHorizonTexture2D = nullptr;

		////----------------------------------------TextUre Desc-----------------------------------------//
		D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
		::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
		d3dTexture2DDesc.Width = FRAME_BUFFER_WIDTH;
		d3dTexture2DDesc.Height = FRAME_BUFFER_HEIGHT;
		d3dTexture2DDesc.MipLevels = 1;
		d3dTexture2DDesc.ArraySize = 1;
		d3dTexture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //RW
		d3dTexture2DDesc.SampleDesc.Count = 1;
		d3dTexture2DDesc.SampleDesc.Quality = 0;
		d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
		d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS; //RW
		d3dTexture2DDesc.CPUAccessFlags = 0;
		d3dTexture2DDesc.MiscFlags = 0;
		pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, 0, &m_pHorizonTexture2D);
		//----------------------------------------TextUre Desc-----------------------------------------//

		//----------------------------------------Resource Desc-----------------------------------------//
		D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRViewDesc;
		::ZeroMemory(&d3dSRViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		d3dSRViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		d3dSRViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		d3dSRViewDesc.Texture2D.MostDetailedMip = 0;
		d3dSRViewDesc.Texture2D.MipLevels = 1;
		pd3dDevice->CreateShaderResourceView(m_pHorizonTexture2D, &d3dSRViewDesc, &m_pHorizonSRV);
		//----------------------------------------Resource Desc-----------------------------------------//

		//----------------------------------------UnorderedAccessView----------------------------------------//
		D3D11_UNORDERED_ACCESS_VIEW_DESC d3dUAViewDesc;
		d3dUAViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		d3dUAViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		d3dUAViewDesc.Texture2D.MipSlice = 0;
		pd3dDevice->CreateUnorderedAccessView(m_pHorizonTexture2D, &d3dUAViewDesc, &m_pHorizonUAV);
		//----------------------------------------UnorderedAccessView----------------------------------------//
	}

	{
		////----------------------------------------TextUre Desc-----------------------------------------//
		m_pd3dComputTexture2D = nullptr;
		D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
		::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
		d3dTexture2DDesc.Width = FRAME_BUFFER_WIDTH;
		d3dTexture2DDesc.Height = FRAME_BUFFER_HEIGHT;
		d3dTexture2DDesc.MipLevels = 1;
		d3dTexture2DDesc.ArraySize = 1;
		d3dTexture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //RW
		d3dTexture2DDesc.SampleDesc.Count = 1;
		d3dTexture2DDesc.SampleDesc.Quality = 0;
		d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
		d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS; //RW
		d3dTexture2DDesc.CPUAccessFlags = 0;
		d3dTexture2DDesc.MiscFlags = 0;
		pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, 0, &m_pd3dComputTexture2D);
		pd3dDevice->CreateRenderTargetView(m_pd3dComputTexture2D, nullptr, &m_pd3dRTV_OffScreen);
		pd3dDevice->CreateShaderResourceView(m_pd3dComputTexture2D, nullptr, &m_pd3dSRV_OffScreen);
		pd3dDevice->CreateUnorderedAccessView(m_pd3dComputTexture2D, nullptr, &m_pd3dUAV_OffScreen);
		//----------------------------------------TextUre Desc-----------------------------------------//
	}

	CreateComputeShader(pd3dDevice);
}

void CScreenShader::ReSetShader(ID3D11Device * pd3dDevice)
{
	if (m_pd3dPixelShader)m_pd3dPixelShader->Release();
	m_pd3dPixelShader = nullptr;
	CreatePixelShaderFromFile(pd3dDevice, L"ScreenShaderFreeCamera.hlsl", "main", "ps_5_0", &m_pd3dPixelShader);
}

void CScreenShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{

	#ifdef _USE_COMPUTE_SHADER
		pd3dDeviceContext->PSSetShaderResources(0, 1, &m_pd3dSRV_OffScreen);
		//------------------------------------- 지훈 -----------------------------------------------
		//pd3dDeviceContext->PSSetShaderResources(m_nTextureStartSlot + 1, m_nTextures - 1, &m_ppd3dsrvTextures[1]);
		//------------------------------------- 지훈 -----------------------------------------------
		//pd3dDeviceContext->PSSetShaderResources(m_nTextures + 1, 1, &(m_pHorizonSRV));
		pd3dDeviceContext->PSSetSamplers(m_nSamplerStartSlot, m_nSamplers, m_ppd3dSamplerStates);
	#else

	//KYT '12.08.10
	/*
	Light 업데이트는 한번만 하면됩니다.
	*/
	LIGHTMGR->UpdateConstBuffer(pd3dDeviceContext);
	CScreenShader::UpdateConstBuffer(pd3dDeviceContext);

	pd3dDeviceContext->PSSetShaderResources(m_nTextureStartSlot, m_nTextures, &m_ppd3dsrvTextures[0]);
	pd3dDeviceContext->PSSetSamplers(m_nSamplerStartSlot, m_nSamplers, m_ppd3dSamplerStates);

	#endif


	//
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
	//

	pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, m_pnVertexStrides, m_pnVertexOffsets);
	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//CONTENTMGR->DebugginBlendingState(pd3dDeviceContext);
	pd3dDeviceContext->Draw(4, 0);
	//CONTENTMGR->DebugginBlendingState(pd3dDeviceContext, false);

	//

	pd3dDeviceContext->IASetInputLayout(nullptr);
	pd3dDeviceContext->VSSetShader(nullptr, nullptr, 0);
	pd3dDeviceContext->PSSetShader(nullptr, nullptr, 0);

	//pd3dDeviceContext->CSSetShaderResources(m_nSamplerStartSlot, 1, &m_ppd3dsrvTextures[0]);
	//pd3dDeviceContext->CSSetSamplers(m_nSamplerStartSlot, m_nSamplers, m_ppd3dSamplerStates);


}

void CScreenShader::ComputeRender(ID3D11DeviceContext * pd3dDeviceContext, ID3D11ShaderResourceView* pd3dsrvDiffuse)
{
	//KYT '12.02.26
	/*
	Light 업데이트는 한번만 하면됩니다.
	*/
	LIGHTMGR->UpdateConstBuffer(pd3dDeviceContext);
	CScreenShader::UpdateConstBuffer(pd3dDeviceContext);

	UINT cxGroup = (UINT)ceilf(FRAME_BUFFER_WIDTH / 256.0f);
	UINT cyGroup = (UINT)ceilf(FRAME_BUFFER_HEIGHT / 256.0f);

	ID3D11ShaderResourceView  *pd3dNullSRV[1] = { nullptr };
	ID3D11UnorderedAccessView *pd3dNullUAV[1] = { nullptr };

	int nBlurCount = 1;
	for (int i = 0; i < nBlurCount; ++i)
	{
		//---------------------------------------지훈 -----------------------
		/*
		현재 상태 브리핑
		- 1. 조명 계산에 필요한 모든 자원은 이미 cs에 set 되어 있다.
		*/
		//-------------------------- cs조명 계산 ----------------------------------

		//1. 그림 그릴 uav set
		pd3dDeviceContext->CSSetUnorderedAccessViews(0, 1, &m_pd3dUAV_OffScreen, nullptr);

		//조명 계산할 cs set.
		pd3dDeviceContext->CSSetShader(m_pHorizontalBlurShader, nullptr, 0);

		//게임 화면 만큼의 pixel이 있을 꺼니까 그만큼 dispatch
		pd3dDeviceContext->Dispatch(cxGroup, FRAME_BUFFER_HEIGHT, 1);
		//-------------------------- cs조명 계산 ----------------------------------
		//------------------------------------- 지훈 -----------------------------------------------

		//--------------------------- nullptr -------------------------------
		pd3dDeviceContext->CSSetShaderResources(0, 1, pd3dNullSRV);
		pd3dDeviceContext->CSSetUnorderedAccessViews(0, 1, pd3dNullUAV, nullptr);
		pd3dDeviceContext->CSSetShader(nullptr, nullptr, 0);
		//--------------------------- nullptr -------------------------------

	}
}
//다중 렌더 타겟