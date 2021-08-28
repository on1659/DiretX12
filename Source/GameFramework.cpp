//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------s

#include "stdafx.h"
#include "GameFramework.h"
#include "InputManager.h"
#include "Warp3DModelManager.h"
#include "CubeTerrainManager.h"
#include "Shader.h"

#include "InstancedObjectsShader.h"

#include <fstream>

#ifdef _MULIT_THREAD_RENDER_
	#include "DefferedThreadProc.h"
	#include "RenderingThreadInfo.h"
#endif

#ifdef _MULIT_RENDER_TARGET_
#include "ScreenShader.h"
#endif

//#include "CTerrainPlayer.h"

#include "ContentManager.h"

CGameFramework::CGameFramework()
{
	m_name = "GameFrameWork";
	m_pd3dDevice = nullptr;
	m_pDXGISwapChain = nullptr;
	m_pd3dRenderTargetView = nullptr;
	m_pd3dDeviceContext = nullptr;

	m_pd3dDepthStencilBuffer = nullptr;
	m_pd3dDepthStencilView = nullptr;

	m_pScene = nullptr;
	m_pPlayer = nullptr;
	m_pCamera = nullptr;


	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	_tcscpy_s(m_pszBuffer, _T("SweetCrush ("));

	srand(timeGetTime());

#ifdef _MULIT_RENDER_TARGET_


	m_pd3dtxtPosition = nullptr;
	m_pd3dtxtNormal = nullptr;
	m_pd3dtxtDiffuse = nullptr;

	m_pd3dsrvPosition = nullptr;
	m_pd3dsrvNormal = nullptr;
	m_pd3dsrvDiffuse = nullptr;

	m_pd3drtvPosition = nullptr;
	m_pd3drtvNormal = nullptr;
	m_pd3drtvDiffuse = nullptr;

#endif

	m_vThread.clear();
	isPlaying = true;

	m_bLoad = false;
	bControlCV = false;

	m_bHDR = true;
	m_fMiddleGrey = 3.16f;
	m_fWhite = 1.53f;
	m_fBloomThreshold = 1.1f;
	m_fBloomScale = 0.34f;

	m_bUseTwBar = false;
	m_bUseRenderTargetView = false;


	//
	m_stLim = nullptr;
	m_bUseTwBar = true;

	m_bSaveButton = false;
	m_bLoadButton = false;

	m_pd3dLimBuffer = nullptr;
	m_pd3dScreenCaptureResourceView = nullptr;
}

CGameFramework::~CGameFramework()
{
}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	//JJY '16.04.16

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	if (!CreateDirect3DDisplay()) 
		return(false);

	//2. Ÿ�̸�
	TIMEMGR;

	//3. ��ǲ�Ŵ���
	INPUT->Load();

	//4. ���� �Ŵ���
#ifdef __DEBOUG__SOUND__
	SOUNDMGR->LoadSound();
#endif


	//Light
	LIGHTMGR->Load(m_pd3dDevice);

	//TextWapper Load
	FONTMGR->Load(m_pd3dDevice);


	//5. �ؽ��� �Ŵ���
	//TEXTUREMGR->BuildObjects(m_pd3dDevice);

#ifdef _SPACEPARTITIONING
#ifdef _QURD_ARRARY
	SPACE->Load(WORLDSIZE, WORLD_DEPTH, NODE_DEPTH_LEVEL);
#else
	SPACE->Load(WORLDSIZE, 2000, WORLDSIZE, NODE_DEPTH_LEVEL);
#endif
#endif

	CGameFramework::CreateConstBuffers();


#ifdef _MULIT_THREAD_LOADING_

	SceneLoad();

#else

#ifdef _MULIT_RENDER_TARGET_
	//���̴� ����
	m_pTextureToScreenShader = std::make_shared<CScreenShader>(MULITE_RENDER_NUMBER, 1, 0, 0);

	::GTimeRecord();
	m_pTextureToScreenShader->BuildObjects(m_pd3dDevice);
	::GTimeRecord(false, "ScreenShader BuildObjects : ");

	::GTimeRecord();
	m_pTextureToScreenShader->CreateShader(m_pd3dDevice);
	::GTimeRecord(false, "ScreenShader CreateShader : ");
	//m_pTextureToScreenShader->ComputeLoad(m_pd3dDevice);

#endif



	//KYT '16.01.20 plus
	/*����Ʈ ������Ʈ ����*/
	::GTimeRecord();
	CONTENTMGR->Load(m_pd3dDevice);
	::GTimeRecord(false, "Content Load  : ");

	//BULLETMGR->Load(m_pd3dDevice);

	//BulidObject
	CGameFramework::BuildObjects();
	//KYT '16.01.20 plus
	/*����Ʈ ������Ʈ ����*/
	//Cuz. BulidObject�ϱ� ���ؼ��� GameObject�� ������ �˾ƾ� �ϱ� ������

	::GTimeRecord();
	CONTENTMGR->FinalLoad(m_pd3dDevice);
	SPACE->FinalLoad();
	UIMGR->FinalLoad();
	::GTimeRecord(false, "FinalLoad  : ");

#endif

	timeTick = false;

	//PostProcessing
	m_pPostManager = std::make_unique<CPostProcessingManager>();
	m_pPostManager->Load(m_pd3dDevice, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);

	//TwBar
	m_wTwBar.Load(m_pd3dDevice);

	m_stLim = new LIMLIGHT();
	m_stOutLine = new OUTLINE_DATA();

	ReadTwBarData();

	int pos[2] = { 30, 150 };
	int size[2] = { 320, 120 };
	int color[3] = { 237, 180, 237 };

	TwBar* twBar = TwNewBar("HDR");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	TwSetParam(twBar, NULL, "size", TW_PARAM_INT32, 2, size);
	TwSetParam(twBar, NULL, "color", TW_PARAM_INT32, 3, color);
	TwSetParam(twBar, NULL, "position", TW_PARAM_INT32, 2, pos);

	TwAddVarRW(twBar, "SaveButton", TW_TYPE_BOOL8, &m_bSaveButton, "keyincr=F9 keydecr=F10 help='Scale the object (1=original size).'"); // bar
	TwAddVarRW(twBar, "LoadButton", TW_TYPE_BOOL8, &m_bLoadButton, "keyincr=F9 keydecr=F10 help='Scale the object (1=original size).'"); // bar

	TwAddVarRW(twBar, "MiddleGrey", TW_TYPE_FLOAT, &m_fMiddleGrey, "min=-1.5 max=10 step=0.05 keyincr=F9 keydecr=F10 help='Scale the object (1=original size).'"); // bar
	TwAddVarRW(twBar, "BloomScale", TW_TYPE_FLOAT, &m_fBloomScale, "min=-5 max=5 step=0.05 keyincr=F7 keydecr=F8 help='Scale the object (1=original size).'"); // bar
	TwAddVarRW(twBar, "HDR_fWhite", TW_TYPE_FLOAT, &m_fWhite, "min=-5 max=5 step=0.05 keyincr=F7 keydecr=F8 help='Scale the object (1=original size).'"); // bar
	TwAddVarRW(twBar, "HDR_BloomThreshold", TW_TYPE_FLOAT, &m_fBloomThreshold, "min=-5 max=5 step=0.05 keyincr=F7 keydecr=F8 help='Scale the object (1=original size).'"); // bar

	TwAddVarRW(twBar, "HDR", TW_TYPE_BOOL8, &m_bHDR, "key=l");
	TwAddVarRW(twBar, "RenderTarget", TW_TYPE_BOOL8, &m_bUseRenderTargetView, "key=p");
	TwAddVarRW(twBar, "UseTwBar", TW_TYPE_BOOL8, &m_bUseTwBar, "key=F11");

	TwBar* Lim = TwNewBar("PS_Shader");
	pos[1] += 135;

	TwSetParam(Lim, NULL, "size", TW_PARAM_INT32, 2, size);
	TwSetParam(Lim, NULL, "color", TW_PARAM_INT32, 3, color);
	TwSetParam(Lim, NULL, "position", TW_PARAM_INT32, 2, pos);

	TwAddVarRW(Lim, "UseLim", TW_TYPE_BOOL8, &m_bLimLight, "key=F12");
	TwAddVarRW(Lim, "LimColor", TW_TYPE_COLOR3F, &m_stLim->color, "");
	TwAddVarRW(Lim, "LimWidth", TW_TYPE_FLOAT, &m_stLim->fRimWidth, "min = 0 max = 1 step = 0.01");
	TwAddVarRW(Lim, "LimPower", TW_TYPE_FLOAT, &m_stLim->fPower, "min = 0 max = 1 step = 0.01");

	TwAddVarRW(Lim, "OutLine_Color", TW_TYPE_COLOR3F, &m_stOutLine->color, "");
	TwAddVarRW(Lim, "OutLine_Width", TW_TYPE_FLOAT, &m_stOutLine->fWidth, "min = 0 max = 0.02 step = 0.001");


	if (m_pd3dLimBuffer)
		m_pd3dLimBuffer->Release();
	m_pd3dLimBuffer = nullptr;

	//Create Bulr Const Buffer 
	D3D11_BUFFER_DESC blurbd;
	ZeroMemory(&blurbd, sizeof(blurbd));
	blurbd.Usage = D3D11_USAGE_DYNAMIC;
	blurbd.ByteWidth = sizeof(LIMLIGHT);
	blurbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	blurbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dFogBufferData;
	ZeroMemory(&d3dFogBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dFogBufferData.pSysMem = m_stLim;
	auto hr = m_pd3dDevice->CreateBuffer(&blurbd, &d3dFogBufferData, &m_pd3dLimBuffer);
	m_pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_LIMLIGHT, 1, &m_pd3dLimBuffer);


	blurbd.ByteWidth = sizeof(OUTLINE_DATA);
	ZeroMemory(&d3dFogBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dFogBufferData.pSysMem = m_stOutLine;
	hr = m_pd3dDevice->CreateBuffer(&blurbd, &d3dFogBufferData, &m_pd3dOutLineBuffer);
	m_pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_OUTLINE, 1, &m_pd3dOutLineBuffer);

	return(true);
}

bool CGameFramework::OnCreate()
{
	if (!CreateDirect3DDisplay()) 
		return false;


	//2. Ÿ�̸�
	TIMEMGR;


	//3. ��ǲ�Ŵ���
	INPUT->Load();

	//4. ���� �Ŵ���
	#ifdef __DEBOUG__SOUND__
		SOUNDMGR->LoadSound();
	#endif


	//Light
	LIGHTMGR->Load(m_pd3dDevice);

	//TextWapper Load
	FONTMGR->Load(m_pd3dDevice);

	//TW
	//TWBAR->Load(m_pd3dDevice, " GLOBAL help='test ui death' ");

	//5. �ؽ��� �Ŵ���
	//TEXTUREMGR->BuildObjects(m_pd3dDevice);

#ifdef _SPACEPARTITIONING
#ifdef _QURD_ARRARY
	SPACE->Load(WORLDSIZE, WORLD_DEPTH, NODE_DEPTH_LEVEL);
#else
	SPACE->Load(WORLDSIZE, 2000, WORLDSIZE, NODE_DEPTH_LEVEL);
#endif
#endif


	CGameFramework::CreateConstBuffers();


#ifdef _MULIT_THREAD_LOADING_

	m_pScene = CScene::GetInstance();

#else

#ifdef _MULIT_RENDER_TARGET_
	//���̴� ����
	m_pTextureToScreenShader = std::make_shared<CScreenShader>(MULITE_RENDER_NUMBER, 1, 0, 0);

	::GTimeRecord();
	m_pTextureToScreenShader->BuildObjects(m_pd3dDevice);
	::GTimeRecord(false, "ScreenShader BuildObjects : ");

	::GTimeRecord();
	m_pTextureToScreenShader->CreateShader(m_pd3dDevice);
	::GTimeRecord(false, "ScreenShader CreateShader : ");
	//m_pTextureToScreenShader->ComputeLoad(m_pd3dDevice);

#endif



	//KYT '16.01.20 plus
	/*����Ʈ ������Ʈ ����*/
	::GTimeRecord();
	CONTENTMGR->Load(m_pd3dDevice);
	::GTimeRecord(false, "Content Load  : ");

	//BULLETMGR->Load(m_pd3dDevice);

	//BulidObject
	CGameFramework::BuildObjects();
	//KYT '16.01.20 plus
	/*����Ʈ ������Ʈ ����*/
	//Cuz. BulidObject�ϱ� ���ؼ��� GameObject�� ������ �˾ƾ� �ϱ� ������

	::GTimeRecord();
	CONTENTMGR->FinalLoad(m_pd3dDevice);
	SPACE->FinalLoad();
	UIMGR->FinalLoad();
	::GTimeRecord(false, "FinalLoad  : ");

#endif

	timeTick = false;
	return(true);
}

void CGameFramework::ThreadCreate()
{
	m_vThread.push_back(new std::thread([&]() {OnCollision(); }));
	m_vThread.push_back(new std::thread([&]() {AnimateObjects(); }));
}

bool CGameFramework::CreateDirect3DDisplay()
{
	if (m_bLoad) return true;
	m_bLoad = true;

	::GetClientRect(m_hWnd, &m_rcClient);
	m_nWndClientWidth = m_rcClient.right - m_rcClient.left;
	m_nWndClientHeight = m_rcClient.bottom - m_rcClient.top;

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	//KYT '16.01.25 plus
	/*��ũ�������*/
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
	dxgiSwapChainDesc.Windowed = TRUE;
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH/*0*/;

	UINT dwCreateDeviceFlags = 0;

	#ifdef _DEBUG
		//dwCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	// https://docs.microsoft.com/ko-kr/windows/win32/direct3d11/overviews-direct3d-11-devices-downlevel-intro?redirectedfrom=MSDN#Overview
	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		// �ϵ���� ����̹�, Direct3D�� ����� �ִ��� �ϵ����� ���� 
		// �ϵ���������� �������� �ʴ� ����� ����Ʈ��� ���� ����
		//'16.12.06
		/*
		D3D_DRIVER_TYPE_HARDWARE���� D3D_DRIVER_TYPE_UNKNOWN�� ����
		GPU���� ����(EnumAdapters)
		*/
		//D3D_DRIVER_TYPE_HARDWARE,
		//���� �˼�����
		D3D_DRIVER_TYPE_UNKNOWN,

		// DirectX SDK���� �����ϴ� ������ ����Ʈ���� ����̹�
		// Ư�� ���� 9_1���� Ư�� ���� 10.1���� ����
		// - ������ ����Ʈ���� ����̹�(�����Ͷ�����)�� �ʿ����
		// - ����Ʈ���� ����̹��̸鼭 �׷��� �ϵ������ �ִ� ������ ����
		//   Direct3D 11�� �������� �ʴ� �׷��� �ϵ������� ���
		D3D_DRIVER_TYPE_WARP,

		// ���� ����̹�(��� Direct3D Ư���� �����ϴ� ����Ʈ���� ����̹�)
		// DirectX SDK���� �ӵ����ٴ� ��Ȯ���� ���� ����
		// CPU�� ����Ͽ� �����Ͷ����� ����	
		// Direct3D 11�� ��� Ư�� �������� �� ����̹��� ����� �� ����
		// ���� ���α׷��� �׽�Ʈ�� ������� ���� �ַ� ���
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D_DRIVER_TYPE);

	D3D_FEATURE_LEVEL pd3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	UINT nFeatureLevels = sizeof(pd3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	D3D_DRIVER_TYPE nd3dDriverType = (D3D_DRIVER_TYPE)D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL nd3dFeatureLevel = (D3D_FEATURE_LEVEL)D3D_FEATURE_LEVEL_11_0;
	HRESULT hResult = S_OK;

#ifdef _WITH_MSAA4_MULTISAMPLING
			dxgiSwapChainDesc.SampleDesc.Count = 4;
			dxgiSwapChainDesc.SampleDesc.Quality = m_n4xMSAAQualities - 1;
#else // _WITH_MSAA4_MULTISAMPLING
			dxgiSwapChainDesc.SampleDesc.Count = 1;
			dxgiSwapChainDesc.SampleDesc.Quality = 0;
#endif //_WITH_MSAA4_MULTISAMPLING
		IDXGIFactory1*pdxgiFactory = nullptr;
		IDXGIDevice *pdxgiDevice = nullptr;
		if (FAILED(hResult = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **)&pdxgiFactory)))	// DX11 DX12 ����
			return false;

		IDXGIAdapter* pAdapter;

#ifdef _SELECT_GPU_DRIVE_170904
		size_t ui64VideoMemory;
		DXGI_ADAPTER_DESC adapterDesc;

		// DirectX �׷��� �������̽� ���丮�� ����ϴ�. 
		//auto result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);

		if (FAILED(pdxgiFactory->EnumAdapters(0, (IDXGIAdapter**)&pAdapter)))
		{
			MessageBox(m_hWnd, TEXT("��û�� �׷��� ī�� �������̽��� ���� ����Ͱ� �����ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		}

		pAdapter->GetDesc(&adapterDesc);
		ui64VideoMemory = (std::size_t)(adapterDesc.DedicatedVideoMemory + adapterDesc.SharedSystemMemory);

		//���� �޸� ���ؼ� �� ���� GPU ã��
		int gpu_idx = 0;
		int select = 0;
		std::size_t comparison_videoMemory;
		while (pdxgiFactory->EnumAdapters(gpu_idx, &pAdapter) != DXGI_ERROR_NOT_FOUND)
		{
			pAdapter->GetDesc(&adapterDesc);
			comparison_videoMemory = (std::size_t)(adapterDesc.DedicatedVideoMemory + adapterDesc.SharedSystemMemory);

			auto memory = comparison_videoMemory / 1024 / 1024;
			wcout << L"GPU " << adapterDesc.Description << L" - [" << gpu_idx << L"] : " << memory << L"Mb" << std::endl;

			if (comparison_videoMemory > ui64VideoMemory)
			{
				ui64VideoMemory = comparison_videoMemory;
				select = gpu_idx;
			}
			++gpu_idx;
		}
		cout << "bye" << endl;
		
		pdxgiFactory->EnumAdapters(select, &pAdapter);
#endif // _SELECT_GPU_DRIVE_170904

		for (UINT i = 0; i < nDriverTypes; i++)
		{
			hResult = D3D11CreateDevice(pAdapter
					, d3dDriverTypes[i]
					, nullptr
					, dwCreateDeviceFlags
					, pd3dFeatureLevels
					, nFeatureLevels
					, D3D11_SDK_VERSION
					, &m_pd3dDevice
					, &nd3dFeatureLevel
				    , &m_pd3dDeviceContext);
				
				if (SUCCEEDED(hResult))
					break;
		}
		if (m_pd3dDevice == nullptr)
			return false;

		Radar::DXUT_SetDebugName(m_pd3dDevice, ("FrameWork Device"));

		if (FAILED(hResult = m_pd3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_n4xMSAAQualities))) return(false);


		if (FAILED(hResult = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pdxgiDevice))) return(false);
		if (FAILED(hResult = pdxgiFactory->CreateSwapChain(pdxgiDevice, &dxgiSwapChainDesc, &m_pDXGISwapChain))) return(false);
		if (pdxgiDevice) pdxgiDevice->Release();
		if (pdxgiFactory) pdxgiFactory->Release();
		if (pAdapter) pAdapter->Release();


	if (!CreateRenderTargetDepthStencilView())
		return false;

	return(true);
}

bool CGameFramework::CreateRenderTargetDepthStencilView()
{
	HRESULT hResult = S_OK;

	//ID3D11Texture2D *pd3dBackBuffer;
	//������ ����
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&m_pd3dMainBackBufferTextrue))) return(false);

	//����Ÿ�ٺ並 ������
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(m_pd3dMainBackBufferTextrue, nullptr, &m_pd3dRenderTargetView))) return(false);
	Radar::DXUT_SetDebugName(m_pd3dRenderTargetView, ("FrameWork Device m_pd3dRenderTargetView"));

	m_pd3dDevice->CreateShaderResourceView(m_pd3dMainBackBufferTextrue, nullptr, &m_pd3dMainShaderResourceView);
	Radar::DXUT_SetDebugName(m_pd3dMainShaderResourceView, ("FrameWork m_pd3dMainShaderResourceView "));

	//KYT '16.01.25 plus
	/* ��ũ������� �߰�*/
	//if (FAILED(hResult = m_pd3dDevice->CreateShaderResourceView(m_pd3dMainBackBufferTextrue, nullptr, &m_pd3dScreenCaptureResourceView)))return (false);
	//Radar::DXUT_SetDebugName(m_pd3dScreenCaptureResourceView, ("FrameWork Device m_pd3dScreenCaptureResourceView"));

	//if (pd3dBackBuffer) pd3dBackBuffer->Release();

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;
	ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dDepthStencilBufferDesc.Width = m_nWndClientWidth;
	d3dDepthStencilBufferDesc.Height = m_nWndClientHeight;
	d3dDepthStencilBufferDesc.MipLevels = 1;
	d3dDepthStencilBufferDesc.ArraySize = 1;
	d3dDepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
#ifdef _WITH_MSAA4_MULTISAMPLING
	d3dDepthStencilBufferDesc.SampleDesc.Count = 4;
	d3dDepthStencilBufferDesc.SampleDesc.Quality = m_n4xMSAAQualities - 1;
#else
	d3dDepthStencilBufferDesc.SampleDesc.Count = 1;
	d3dDepthStencilBufferDesc.SampleDesc.Quality = 0;
#endif
	d3dDepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dDepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3dDepthStencilBufferDesc.CPUAccessFlags = 0;
	d3dDepthStencilBufferDesc.MiscFlags = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateTexture2D(&d3dDepthStencilBufferDesc, nullptr, &m_pd3dDepthStencilBuffer))) return(false);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = d3dDepthStencilBufferDesc.Format;
#ifdef _WITH_MSAA4_MULTISAMPLING
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
#else
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
#endif
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, &m_pd3dDepthStencilView))) return(false);

	//���� Ÿ���� ���� ���߿� �ٲ�� �� �� 5�� set �ؾ���


	return(true);
}

#ifdef _MULIT_RENDER_TARGET_

bool CGameFramework::CreateRenderTargetDepthStencilViewMRT()
{
	//----------------------------------------Render Desc-----------------------------------------//
	D3D11_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
	d3dRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	d3dRTVDesc.Texture2D.MipSlice = 0;
	//d3dRTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//----------------------------------------TextUre Desc-----------------------------------------//


	//----------------------------------------TextUre Desc-----------------------------------------//
	D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
	::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTexture2DDesc.Width = m_nWndClientWidth;
	d3dTexture2DDesc.Height = m_nWndClientHeight;
	d3dTexture2DDesc.MipLevels = 1;
	d3dTexture2DDesc.ArraySize = 1;
	d3dTexture2DDesc.SampleDesc.Count = 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.CPUAccessFlags = 0;
	d3dTexture2DDesc.MiscFlags = 0;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	//----------------------------------------TextUre Desc-----------------------------------------//

	//----------------------------------------Resource Desc-----------------------------------------//
	D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
	::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	d3dSRVDesc.Texture2D.MipLevels = 1;
	//d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	//----------------------------------------Resource Desc-----------------------------------------//

	d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//--------------------------------------SceneB RTV Create-----------------------------------------//
	m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtDiffuse);
	m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtDiffuse, &d3dSRVDesc, &m_pd3dsrvDiffuse);
	m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtDiffuse, &d3dRTVDesc, &m_pd3drtvDiffuse);
	//--------------------------------------SceneB RTV Create-----------------------------------------//

	//--------------------------------------SceneR RTV Create-----------------------------------------//
	m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtPosition);
	m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtPosition, &d3dRTVDesc, &m_pd3drtvPosition);
	m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtPosition, &d3dSRVDesc, &m_pd3dsrvPosition);
	//--------------------------------------SceneR RTV Create-----------------------------------------//

	//--------------------------------------SceneG RTV Create-----------------------------------------//
	m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtNormal);
	m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtNormal, &d3dRTVDesc, &m_pd3drtvNormal);
	m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtNormal, &d3dSRVDesc, &m_pd3dsrvNormal);
	//--------------------------------------SceneG RTV Create-----------------------------------------//

	//--------------------------------------DepthZ RTV Create-----------------------------------------//
	//m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtSpecular);
	//m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtSpecular, &d3dSRVDesc, &m_pd3dsrvSpecular);
	//m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtSpecular, &d3dRTVDesc, &m_pd3drtvSpecular);
	//--------------------------------------DepthZ RTV Create-----------------------------------------//

	//--------------------------------------vDepth RTV Create-----------------------------------------//
	//m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtAmbient);
	//m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtAmbient, &d3dSRVDesc, &m_pd3dsrvAmbient);
	//m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtAmbient, &d3dRTVDesc, &m_pd3drtvAmbient);
	//--------------------------------------vDepth RTV Create-----------------------------------------//


	// Render Target View Desc
	ZeroMemory(&d3dRTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	d3dRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	d3dRTVDesc.Texture2D.MipSlice = 0;
	//d3dRTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;


	// Texture2D_Desc
	::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTexture2DDesc.Width = m_nWndClientWidth;
	d3dTexture2DDesc.Height = m_nWndClientHeight;
	d3dTexture2DDesc.MipLevels = 1;
	d3dTexture2DDesc.ArraySize = 1;
	d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	d3dTexture2DDesc.SampleDesc.Count = 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DDesc.CPUAccessFlags = 0;
	d3dTexture2DDesc.MiscFlags = 0;

																			  
	// Shader Resource View Desc
	::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	d3dSRVDesc.Texture2D.MipLevels = 1;

	d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//--------------------------------------vDepth RTV Create-----------------------------------------//
		m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtShadowMap);
		Radar::DXUT_SetDebugName(m_pd3dtxtShadowMap, ("FrameWork m_pd3dtxtShadowMap "));
		m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtShadowMap, &d3dSRVDesc, &m_pd3dsrvShadowMap);
		Radar::DXUT_SetDebugName(m_pd3dsrvShadowMap, ("FrameWork m_pd3dsrvShadowMap "));
		m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtShadowMap, &d3dRTVDesc, &m_pd3drtvShadowMap);
		Radar::DXUT_SetDebugName(m_pd3drtvShadowMap, ("FrameWork m_pd3drtvShadowMap "));
	//--------------------------------------vDepth RTV Create-----------------------------------------//

	m_pTextureToScreenShader->SetTexture(0, m_pd3dsrvDiffuse);
	m_pTextureToScreenShader->SetTexture(1, m_pd3dsrvPosition);
	m_pTextureToScreenShader->SetTexture(2, m_pd3dsrvNormal);
	m_pTextureToScreenShader->SetTexture(3, m_pd3dsrvShadowMap);
	//m_pTextureToScreenShader->SetTexture(5, m_pd3dsrvAmbient);
	//m_pTextureToScreenShader->SetTexture(4, m_pd3dsrvSpecular);


	

	//m_pShadwosToScreenShader->SetTexture(0, m_pd3dsrvShadowMap);


	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	m_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);



	//------------------------------light render target-----------------------

		m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtLight);
		Radar::DXUT_SetDebugName(m_pd3dtxtLight, ("FrameWork m_pd3dtxtLight"));

		m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtLight, &d3dRTVDesc, &m_pd3drtvLight);
		Radar::DXUT_SetDebugName(m_pd3drtvLight, ("FrameWork m_pd3drtvLight "));
	
		m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtLight, &d3dSRVDesc, &m_pd3dsrvLight);
		Radar::DXUT_SetDebugName(m_pd3dsrvLight, ("FrameWork m_pd3dsrvLight "));

	//------------------------------light render target--

		float view_scale = 0.95f;
		for (int i = 0; i < 5; ++i)
		{
			auto shader = std::make_shared<CRenderTargetShader>();
			shader->SetScreenPos(XMFLOAT2(130 + (i * 260 * view_scale), 75.0f), XMFLOAT2(256.0f * view_scale, 140.0f* view_scale));
			
			if(i == 4)
				shader->SetSRV(m_pd3dsrvLight);
			else
				shader->SetSRV(m_pTextureToScreenShader->GetSRV(i));

			shader->CreaetShader(m_pd3dDevice);
			shader->SetDeviceContext(m_pd3dDeviceContext);
			m_vScreenShader.push_back(shader);
		}

		m_pShadowRTV = std::make_shared<CRenderTargetShader>();
		m_pShadowRTV->SetScreenPos(XMFLOAT2(0.0f, 0.0f), XMFLOAT2(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT));
		m_pShadowRTV->SetSRV(m_pd3dsrvPosition);
		m_pShadowRTV->SetRTV(m_pd3drtvShadowMap);
		m_pShadowRTV->SetVSName(TEXT("VSShadowsMap.cso"));
		m_pShadowRTV->SetPSName(TEXT("PSShadowsMap.cso"));
		m_pShadowRTV->CreaetShader(m_pd3dDevice);
		m_pShadowRTV->SetDeviceContext(m_pd3dDeviceContext);

	

	return true;
}

void CGameFramework::LoadScreenShader(ID3D11Device*  pd3dDevice)
{
	m_pTextureToScreenShader = std::make_shared<CScreenShader>(MULITE_RENDER_NUMBER, 1, 0, 0);
	::GTimeRecord();
	m_pTextureToScreenShader->BuildObjects(m_pd3dDevice);
	::GTimeRecord(false, "ScreenShader BuildObjects : ");


	//m_pShadwosToScreenShader->CreateShader(m_pd3dDevice);
	
	

	//m_pShadwosToScreenShader->SetScreenPos(f4);
	//m_pShadwosToScreenShader->SetPSShaderName(TEXT("../Assets/07.Shader/DefferedShader/ScreenSubShader.cso"));
	#ifdef _USE_COMPUTE_SHADER
		::GTimeRecord();
		m_pTextureToScreenShader->ComputeLoad(m_pd3dDevice);
		::GTimeRecord(false, "ScreenShader ComputeLoad : ");
	#endif

	::GTimeRecord();
	//m_pTextureToScreenShader->SetScreenPos(f4);
	m_pTextureToScreenShader->CreateShader(m_pd3dDevice);
	::GTimeRecord(false, "ScreenShader CreateShader : ");

	CreateRenderTargetDepthStencilViewMRT();

}

void CGameFramework::SetScreenShader(std::shared_ptr<CScreenShader>& pShader)
{
	return;
}

void CGameFramework::SaveTwBarData(const std::string& path)
{
	std::ofstream ofs(path);
	if (ofs.fail())
	{
		MessageBox(m_hWnd, TEXT("TwBarData Write Error"), TEXT("TwBarData Write Error"), MB_OK);
	}
	int size = sizeof(m_stLim->color) / 4;
	for(int i = 0; i < size; ++i) ofs << "color " << m_stLim->color[i] << std::endl;;

	ofs << "lim_power " << m_stLim->fPower << std::endl;
	ofs << "lim_width " << m_stLim->fRimWidth << std::endl;
	ofs << "lim_use " << m_stLim->use << std::endl;

	ofs << "mid_grey " << Helper::Round(m_fMiddleGrey, 10000) << std::endl;
	ofs << "white " << Helper::Round(m_fWhite, 10000) << std::endl;
	ofs << "bloom_threshold " << Helper::Round(m_fBloomThreshold, 10000) << std::endl;
	ofs << "bloom_scale " << Helper::Round(m_fBloomScale, 10000) << std::endl;

	ofs << "usd_hder " << m_bHDR << std::endl;
	ofs << "use_rtv " << m_bUseRenderTargetView << std::endl;
	ofs << "use_twbar " << m_bUseTwBar << std::endl;

	ofs << "outline_width " << m_stOutLine->fWidth << std::endl;
	size = sizeof(m_stOutLine->color) / 4 ;
	for (int i = 0; i < 4; ++i) ofs << "outline_color " << (int)m_stOutLine->color[i] << std::endl;

	ofs.close();
}

void CGameFramework::ReadTwBarData(const std::string& path)
{
	std::ifstream ifs(path);
	if (ifs.fail())
	{
		MessageBox(m_hWnd, TEXT("LimLight Read Error"), TEXT("LimLigh Read Error"), MB_OK);
	}
	else
	{
		std::string str;
		int size = sizeof(m_stLim->color) / 4;
		for(int i =0; i < size; ++i) ifs >> str >>  m_stLim->color[i];

		ifs >> str >>  m_stLim->fPower;
		ifs >> str >>  m_stLim->fRimWidth;
		ifs >> str >>  m_stLim->use;

		ifs >> str >>  m_fMiddleGrey;
		ifs >> str >>  m_fWhite;
		ifs >> str >>  m_fBloomThreshold;
		ifs >> str >>  m_fBloomScale;

		ifs >> str >>  m_bHDR;
		ifs >> str >>  m_bUseRenderTargetView;
		ifs >> str >>  m_bUseTwBar;

		ifs >> str >> m_stOutLine->fWidth;
		size = sizeof(m_stLim->color) / 4;
		for (int i = 0; i < size; ++i) ifs >> str >> m_stLim->color[i];

		ifs.close();

	}
}

#endif

void CGameFramework::OnDestroy()
{
	CGameFramework::ReleaseObjects();

	if (m_pd3dDeviceContext) m_pd3dDeviceContext->ClearState();
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
	if (m_pDXGISwapChain) m_pDXGISwapChain->Release();
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();	
	if (m_pd3dDevice) m_pd3dDevice->Release();


	SOUNDMGR->ReleseInstance();
	LIGHTMGR->ReleseInstance();
	TIMEMGR->ReleseInstance();
	m_wTwBar.Release();


	for (auto& shader : m_vScreenShader)
	{
		shader->Release();
		shader->ReleaseShaderVariables();
		shader.reset();
		shader = nullptr;
	}
	m_vScreenShader.clear();

	SaveTwBarData();


	if (m_stLim)delete m_stLim;
	m_stLim = nullptr;

	if (m_stOutLine)delete m_stOutLine;
	m_stOutLine = nullptr;

	//TWBAR->ReleseInstance();
}

void CGameFramework::SceneRelease()
{
	if (m_pScene)
	{
		m_pScene->ReleaseObjects();
		//m_pScene->ReleseInstance();
	}
	m_pScene = nullptr;
}

void CGameFramework::SceneLoad()
{
	if (m_pScene) m_pScene = nullptr;
	m_pScene = CScene::GetInstance();

}

void CGameFramework::UpdatePSConstBuffer()
{
	{
		int size = sizeof(m_stLim->color) / 4;
		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
		m_pd3dDeviceContext->Map(m_pd3dLimBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		LIMLIGHT *pLimLight = (LIMLIGHT *)d3dMappedResource.pData;
		for (int i = 0; i < size; ++i) pLimLight->color[i] = m_stLim->color[i];
		pLimLight->use = m_bLimLight ? 1.0f : 0.0f; //UI -> bool, cb -> float
		pLimLight->fRimWidth = m_stLim->fRimWidth;
		pLimLight->fPower = m_stLim->fPower;
		m_pd3dDeviceContext->Unmap(m_pd3dLimBuffer, 0);
	}

	{
		int size = sizeof(m_stOutLine->color) / 4;
		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
		m_pd3dDeviceContext->Map(m_pd3dOutLineBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		OUTLINE_DATA *pOutLine = (OUTLINE_DATA *)d3dMappedResource.pData;
		pOutLine->fWidth = m_stOutLine->fWidth;
		for (int i = 0; i < size; ++i) pOutLine->color[i] = m_stOutLine->color[i];
		m_pd3dDeviceContext->Unmap(m_pd3dOutLineBuffer, 0);
	}

}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pScene)
		m_pScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{

	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		INPUT->SetOldCursorPos();
		INPUT->SetMouseLeft(true);
		break;

	case WM_RBUTTONDOWN:
		SetCapture(hWnd);
		INPUT->SetOldCursorPos();
		INPUT->SetMouseRight(true);
		break;

	case WM_LBUTTONUP:
	{
		//static bool showCusor = true;
		//showCusor = showCusor ? false : true;
		ShowCursor(true);
		INPUT->SetMouseLeft(false);
		ReleaseCapture();
	}
	break;

	case WM_RBUTTONUP:
		INPUT->SetMouseRight(false);
		ReleaseCapture();
		break;

	case WM_MOUSEMOVE:
		INPUT->SetMousePoint((int)LOWORD(lParam), (int)HIWORD(lParam));
		break;
	case WM_MOUSEWHEEL:
		((short)HIWORD(wParam) < WHEEL_NON) ?
			INPUT->SetWheel(WHEEL_DOWN) :
			INPUT->SetWheel(WHEEL_UP);
		break;

	default:
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if(TwEventWin(hWnd, nMessageID, wParam, lParam))return;

	//if (m_wTwBar.OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam))return;

	if (m_pScene)
		m_pScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{
		//KYT 16'.04.17
		/*
		input ����
		*/
	case WM_KEYDOWN:
		INPUT->SetKeyBoardPress(true);
		if (wParam == 'n' - 32)
			::DimensionsChange(YT_GameMode::GAME_MODE_DIALOG);
		break;
		//

	case WM_KEYUP:
		 
		//KYT 16'.04.17
		/*
		input ����
		*/
		INPUT->SetKeyBoardPress(false);
		//

		switch (wParam)
		{
		case VK_ESCAPE:
		case 'q' - 32:

			//KYT '16.05.05
			/*
			�����Ҷ� ���� �ȵ�°� ����
			*/
			if (::MessageBox(g_hWnd, L"������ ������??", L"���� �����", MB_OKCANCEL) == 1)
			{
			#ifdef _CONNECT_SERVER_
							//JJY 16'.04.16
							// �̺�Ʈ ����
							TCP->EV.DeleteEvent();

							TCP->RoomListClear();
							TCP->RoomPlayerListClear();

							// closesocket()
							closesocket(TCP->sock);

							// ���� ����
							WSACleanup();
			#endif
					::PostQuitMessage(0);
			}
			break;

		case VK_RETURN:
			break;
		case VK_F1:
		case VK_F2:
		case VK_F3:
			break;

		case VK_F8:
			break;
			//KYT '16.01.25 plus
			/*
			��ũ������� �߰�
			*/
			//���ϸ����
			SYSTEMTIME st;
			GetLocalTime(&st);

			_TCHAR name[128];
			_stprintf_s(name, _T("%d_%d_%d_%dh%dm%ds.png"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

			//��ũ�������ϻ���
			ID3D11Resource *res;
			m_pd3dScreenCaptureResourceView->GetResource(&res);
			D3DX11SaveTextureToFile(m_pd3dDeviceContext, res, D3DX11_IFF_PNG, name);
			::GMessageBox(L"ScreenShot", L"��ġ ���ּ���!!! ��ġ^~^");
			break;

		case VK_F9:
		{
			BOOL bFullScreenState = FALSE;
			m_pDXGISwapChain->GetFullscreenState(&bFullScreenState, nullptr);
			if (!bFullScreenState)
			{
				DXGI_MODE_DESC dxgiTargetParameters;
				dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				dxgiTargetParameters.Width = m_nWndClientWidth;
				dxgiTargetParameters.Height = m_nWndClientHeight;
				dxgiTargetParameters.RefreshRate.Numerator = 0;
				dxgiTargetParameters.RefreshRate.Denominator = 0;
				dxgiTargetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
				dxgiTargetParameters.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				m_pDXGISwapChain->ResizeTarget(&dxgiTargetParameters);
			}
			m_pDXGISwapChain->SetFullscreenState(!bFullScreenState, nullptr);
			break;
		}
		case VK_F10:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	m_nWndClientWidth = LOWORD(lParam);
	m_nWndClientHeight = HIWORD(lParam);

	m_pd3dDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	m_pd3dDepthStencilBuffer = nullptr;

	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	m_pd3dRenderTargetView = nullptr;

	if (m_pd3dScreenCaptureResourceView) m_pd3dScreenCaptureResourceView->Release();
	m_pd3dScreenCaptureResourceView = nullptr;

	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
	m_pd3dDepthStencilView = nullptr;

	m_pDXGISwapChain->ResizeBuffers(1, m_nWndClientWidth, m_nWndClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	CreateRenderTargetDepthStencilView();

	if (m_pScene)
		m_pScene->SetViewport(m_pd3dDeviceContext);

	return 0;
}
	
void CGameFramework::BuildObjects()
{

	//'16.07.29
	/*
		Ŀ�� �Ⱥ��̰��ϱ�
	*/
	SetCursor(NULL);

	//Load
	///MODELMGR->Load();

	m_pScene = CScene::GetInstance();


	CScene::GetInstance()->BuildObjects(m_pd3dDevice);

	CScene::GetInstance()->LoadStaticFBX(m_pd3dDevice);

	//CScene::GetInstance()->LoadDynamicFBX(m_pd3dDevice);

	CScene::GetInstance()->LoadSystem();

	WORLDMAP->Load(m_pd3dDevice);

	//KYT '16.04.29
	/*
	Fog �Ķ���� ������ HLSL�� �ѱ�
	*/


#ifdef   _MULIT_THREAD_RENDER_
	CGameFramework::InitializeWorkerThreads();
#endif
}

void CGameFramework::ReleaseObjects()
{
	CGameFramework::ReleaseConstBuffers();
	CGameFramework::SceneRelease();
	if (m_pd3dMainBackBufferTextrue)m_pd3dMainBackBufferTextrue->Release();
	m_pd3dMainBackBufferTextrue = nullptr;

	if (m_pd3dMainShaderResourceView)m_pd3dMainShaderResourceView->Release();
	m_pd3dMainShaderResourceView = nullptr;
	//m_pScene.reset();
	if (m_pd3dDeviceContext)
		m_pd3dDeviceContext->ClearState();
	m_pd3dDeviceContext = nullptr;

	if (m_pd3dRenderTargetView)
		m_pd3dRenderTargetView->Release();
	m_pd3dRenderTargetView = nullptr;

	if (m_pd3dScreenCaptureResourceView)
		m_pd3dScreenCaptureResourceView->Release();
	m_pd3dScreenCaptureResourceView = nullptr;

	if (m_pd3dDepthStencilBuffer)
		m_pd3dDepthStencilBuffer->Release();
	m_pd3dDepthStencilBuffer = nullptr;

	if (m_pd3dDepthStencilView)
		m_pd3dDepthStencilView->Release();
	m_pd3dDepthStencilView = nullptr;

	if (m_pDXGISwapChain)
		m_pDXGISwapChain->Release();
	m_pDXGISwapChain = nullptr;

	if (m_pd3dDeviceContext)
		m_pd3dDeviceContext->Release();
	m_pd3dDeviceContext = nullptr;

	if (m_pd3dDevice) m_pd3dDevice->Release();
	m_pd3dDevice = nullptr;

#ifdef _MULIT_RENDER_TARGET_
	//Texture2D
	if (m_pd3dtxtPosition) 
		m_pd3dtxtPosition->Release();
	m_pd3dtxtPosition = nullptr;

	if (m_pd3dtxtNormal)
		m_pd3dtxtNormal->Release();
	m_pd3dtxtNormal = nullptr;

	if (m_pd3dtxtDiffuse)
		m_pd3dtxtDiffuse->Release();
	m_pd3dtxtDiffuse = nullptr;

	//Shader Resource View
	if (m_pd3dsrvPosition)
		m_pd3dsrvPosition->Release();
	m_pd3dsrvPosition = nullptr;

	if (m_pd3dsrvNormal)
		m_pd3dsrvNormal->Release();
	m_pd3dsrvNormal = nullptr;

	if (m_pd3dsrvDiffuse)
		m_pd3dsrvDiffuse->Release();

	m_pd3dsrvDiffuse = nullptr;
	//Render Target View
	if (m_pd3drtvPosition)
		m_pd3drtvPosition->Release();
	m_pd3drtvPosition = nullptr;

	if (m_pd3drtvNormal)
		m_pd3drtvNormal->Release();
	m_pd3drtvNormal = nullptr;

	if (m_pd3drtvDiffuse)
		m_pd3drtvDiffuse->Release();
	m_pd3drtvDiffuse = nullptr;

	if (m_pTextureToScreenShader)
		m_pTextureToScreenShader->ReleaseObject();
	m_pTextureToScreenShader.reset();
#endif

	//TIMEMGR->ReleseInstance();

	m_bLoad = false;

}

void CGameFramework::CreateConstBuffers()
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	m_pd3dDevice->CreateBuffer(&d3dBufferDesc, nullptr, &CGameObject::m_pd3dcbWorldMatrix);

	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT4) * 4;
	m_pd3dDevice->CreateBuffer(&d3dBufferDesc, nullptr, &CGameObject::m_pd3dcbMaterialColors);

	CCamera::CreateConstBuffers(m_pd3dDevice);
	CCamera::OrthoCreateConstBuffer(m_pd3dDevice);
	CTexture::CreateConstBuffers(m_pd3dDevice);
}

void CGameFramework::ReleaseConstBuffers()
{
	CGameObject::ReleaseConstBuffers();
	CCamera::ReleaseConstBuffers();
	CTexture::ReleaseConstBuffers();

}

void CGameFramework::ProcessInput()
{
	//::gotoxy(0, 0);
	//std::cout << "x,y(" << INPUT->GetMousePoint().x << "," << INPUT->GetMousePoint().y << ")" << std::endl;
}

void CGameFramework::OnCollision()
{
	//while (isPlaying)
	{
		if (m_pScene) m_pScene->OnCollision(m_pd3dDeviceContext);
	}
}

void CGameFramework::AnimateObjects()
{
	if (INPUT->OnlyKeyDown(YK_F11))
	{
		SOUNDMGR->Play("����", XMFLOAT3(20, 1, 20));
	}

	SOUNDMGR->OnUpdate(PLAYER->GetPlayer()->GetPositionXMFLOAT3());
	
	if (m_pScene)
		m_pScene->AnimateObjects(TIMEMGR->GetTimeElapsed());

	gotoxy(0, 0);
	int index = 0;
	for (auto& shader : m_vScreenShader)
	{
		shader->ExpandRTV();
	}

}

void CGameFramework::FrameAdvance()
{
	ProcessInput();

	if (bControlCV == false)
	{
		if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_S))
		{
			system("cls");
			bControlCV = true;
			m_pTextureToScreenShader->ReSetShader(m_pd3dDevice);
			bControlCV = false;
			m_pPostManager->Read_CSO_Shader(m_pd3dDevice);

			//CONTENTMGR->ReSetShader(m_pd3dDevice, m_pd3dDeviceContext)
		}
	}

	if (m_bSaveButton)
	{
		SaveTwBarData();
		MessageBox(g_hWnd, TEXT("save succ"), TEXT("save _ succ"), MB_OK);
		m_bSaveButton = false;
	}
	if (m_bLoadButton)
	{
		ReadTwBarData();
		MessageBox(g_hWnd, TEXT("read succ"), TEXT("read _ succ"), MB_OK);
		m_bLoadButton = false;
	}

	CGameFramework::AnimateObjects();

	CGameFramework::OnCollision();

	#ifdef _CONNECT_SERVER_

	if (TCP->my_info.bLoadingCompletion == TRUE)
	{
		//Ŭ���̾�Ʈ ���巹Ŀ��
		PLAYER->Client_DeadReckoning();

		//JJY '16'.04.01 plus
		/*������ �����ֱ�� ����ȭ*/
		m_fCurrentCount = TIMEMGR->GetGlobalTime();

		//1�ʿ� 30�� = 1/30�ʿ� 1�� = ��0.03�ʿ� 1�� = 30�и��ʿ� 1��
		//1�ʿ� 10�� ������ ����ȭ
		if (m_fCurrentCount - m_fOldCount >= 0.01f)
		{
			//send
			P_CS_INGAME_REQUIRE_MOVE_SYNC cs_ingame_require_move_sync;
			cs_ingame_require_move_sync.size = sizeof(P_CS_INGAME_REQUIRE_MOVE_SYNC);
			cs_ingame_require_move_sync.type = CS_INGAME_REQUIRE_MOVE_SYNC;
			TCP->SendPacket((unsigned char*)&cs_ingame_require_move_sync);

			//std::cout << "Request Sync!" << std::endl;
			m_fOldCount = m_fCurrentCount;
		}
	}
	#endif

	#ifdef _MULIT_THREAD_RENDER_
	#ifdef _MULIT_RENDER_TARGET_

		//--------------------------------------------------------------------------Forawrd Rendering------------------------------------------------------//
		ID3D11RenderTargetView *pd3dRTVs[MULITE_RENDER_NUMBER] = { m_pd3drtvDiffuse, m_pd3drtvPosition, m_pd3drtvNormal, m_pd3drtvAmbient, m_pd3drtvSpecular, m_pd3drtvShadowMap };
		float fClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		if (m_pd3drtvPosition) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvPosition, fClearColor);
		if (m_pd3drtvNormal) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvNormal, fClearColor);
		if (m_pd3drtvDiffuse) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvDiffuse, fClearColor);
		if (m_pd3dDepthStencilView)m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		for (auto &pRenderThreadInfo : m_vRenderingThreadInfo)
		{
			pRenderThreadInfo.m_pd3dDeferredContext->OMSetRenderTargets(MULITE_RENDER_NUMBER, (ID3D11RenderTargetView **)&pd3dRTVs, m_pd3dDepthStencilView);
			::SetEvent(pRenderThreadInfo.m_hRenderingBeginEvent);
		}
		//�׸��� �� �׸� �� ���� ���
		::WaitForMultipleObjects(m_nRenderThreads, m_hRenderingEndEvents, TRUE, INFINITE);

		for (auto &pRenderThreadInfo : m_vRenderingThreadInfo)
		{
			m_pd3dDeviceContext->ExecuteCommandList(pRenderThreadInfo.m_pd3dDCommandList, true);
			pRenderThreadInfo.m_pd3dDCommandList->Release();
		}
		//--------------------------------------------------------------------------Forawrd Rendering------------------------------------------------------//

	#pragma region[Font]

		POINT pt = INPUT->GetMousePoint();
		//�Ź������
		FONTMGR->testPush("ī��", pt.x, pt.y, 15, YT_Color(0, 0, 0), L"(%d��%d)", pt.x, pt.y);
	#pragma endregion

		//--------------------------------------------------------------------------Deferred Rendering------------------------------------------------------//
		//if (m_pd3dRenderTargetView) m_pd3dDevinceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);

		m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


		//��ǻ�� ���
	#ifdef _USE_COMPUTE_SHADER
		ID3D11ShaderResourceView* pd3dSRVs[MULITE_RENDER_NUMBER] = { m_pd3dsrvDiffuse ,m_pd3dsrvPosition, m_pd3dsrvNormal,m_pd3dsrvSpecular,m_pd3dsrvAmbient,m_pd3dsrvShadowMap };
		m_pd3dDeviceContext->CSSetShaderResources(0, MULITE_RENDER_NUMBER, &pd3dSRVs[0]);// (ID3D11RenderTargetView **)&pd3dRTVs, m_pd3dDepthStencilView);
		m_pTextureToScreenShader->ComputeRender(m_pd3dDeviceContext, m_pd3dsrvDiffuse);
	#endif

		//����Ÿ�� �� ����
		m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);

		//������ ������
		m_pTextureToScreenShader->Render(m_pd3dDeviceContext);
		//--------------------------------------------------------------------------Deferred Rendering------------------------------------------------------//


	#else
		if (m_pScene)
			m_pScene->OnPreRender(m_pd3dDeviceContext);

		float fClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.0f };

		if (m_pd3dRenderTargetView)
			m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);

		if (m_pd3dDepthStencilView)
			m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		for (auto &pRenderThreadInfo : m_vRenderingThreadInfo)
		{
			pRenderThreadInfo.m_pd3dDeferredContext->OMSetRenderTargets(1, (ID3D11RenderTargetView **)&m_pd3dRenderTargetView, m_pd3dDepthStencilView);
			::SetEvent(pRenderThreadInfo.m_hRenderingBeginEvent);
		}
		//�׸��� �� �׸� �� ���� ���
		WaitForMultipleObjects(m_nRenderThreads, m_hRenderingEndEvents, TRUE, INFINITE);


		for (auto &pRenderThreadInfo : m_vRenderingThreadInfo)
		{
			m_pd3dDeviceContext->ExecuteCommandList(pRenderThreadInfo.m_pd3dDCommandList, true);
			pRenderThreadInfo.m_pd3dDCommandList->Release();
		}

	#endif

	#else
	#ifdef _MULIT_RENDER_TARGET_
	//--------------------------------------------------------------------------Forawrd Rendering------------------------------------------------------//
	//����Ÿ�� 4�� Ori, Pos, Nor, Col, Ami, Spe
	ID3D11RenderTargetView *pd3dRTVs[MULITE_RENDER_NUMBER] = 
	{	
			  m_pd3drtvDiffuse		// 0
			, m_pd3drtvPosition		// 1
			, m_pd3drtvNormal		// 2
			, m_pd3drtvShadowMap	// 3
	};

	float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	//4���� ����Ÿ�� Ŭ����
	if (m_pd3drtvPosition) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvPosition, fClearColor);
	if (m_pd3drtvNormal) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvNormal, fClearColor);
	if (m_pd3drtvDiffuse) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvDiffuse, fClearColor);
	if (m_pd3drtvShadowMap) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvShadowMap, fClearColor);

	if (m_pd3dDepthStencilView)
		m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//����Ÿ�� ��
	m_pd3dDeviceContext->OMSetRenderTargets(MULITE_RENDER_NUMBER, (ID3D11RenderTargetView **)&pd3dRTVs, m_pd3dDepthStencilView);

	//����!
	if (m_pScene)
	{
		//m_pd3dDeviceContext->UpdateSubresource(m_pd3dLimBuffer, 0, NULL, &m_stLim, 0, 0);
		//m_pd3dDeviceContext->UpdateSubresource(m_pd3dThickBuffer, 0, NULL, &m_stThick, 0, 0);

		UpdatePSConstBuffer();

		m_pScene->RenderEnvironment(m_pd3dDeviceContext);
		m_pScene->RenderSpacePartition(m_pd3dDeviceContext);
		m_pScene->RenderBasic(m_pd3dDeviceContext);
		m_pScene->RenderBlend(m_pd3dDeviceContext);
	}
	//--------------------------------------------------------------------------Forawrd Rendering------------------------------------------------------//

	#pragma region[Font]
		POINT pt = INPUT->GetMousePoint();
		FONTMGR->MessagePush("ī��", (float)pt.x, (float)pt.y, (float)15, YT_Color(0, 0, 0), PIVOT_HORIZONTAL_CENTER | PIVOT_VERTICLE_CENTER, L"(%d��%d)", pt.x, pt.y);
	#pragma endregion

	//--------------------------------------------------------------------------Deferred Rendering------------------------------------------------------//
	//if (m_pd3dRenderTargetView) m_pd3dDevinceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);

	m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//��ǻ�� ���
	#ifdef _USE_COMPUTE_SHADER
		ID3D11ShaderResourceView* pd3dSRVs[MULITE_RENDER_NUMBER] = { m_pd3dsrvDiffuse ,m_pd3dsrvPosition, m_pd3dsrvNormal,m_pd3dsrvAmbient, m_pd3dsrvSpecular,m_pd3dsrvShadowMap };
		m_pd3dDeviceContext->CSSetShaderResources(0, MULITE_RENDER_NUMBER, &pd3dSRVs[0]);// (ID3D11RenderTargetView **)&pd3dRTVs, m_pd3dDepthStencilView);
		m_pTextureToScreenShader->ComputeRender(m_pd3dDeviceContext, m_pd3dsrvDiffuse);
	#endif


	//m_bHDR = !INPUT->KeyDown(YT_KEY::YK_L);
	//if (GetAsyncKeyState(VK_INSERT))m_fBloomScale += 0.1F;
	//if (GetAsyncKeyState(VK_DELETE))m_fBloomScale -= 0.1F;
	//if (GetAsyncKeyState(VK_F7))	m_fMiddleGrey += 0.1F;
	//if (GetAsyncKeyState(VK_F8))	m_fMiddleGrey -= 0.1F;

	//ID3D11RenderTargetView* pd3dRTV[1] = { m_pShadowRTV->GetRTV() };
	//m_pd3dDeviceContext->OMSetRenderTargets(1, pd3dRTV, m_pd3dDepthStencilView);
	//m_pShadowRTV->Render(m_pd3dDeviceContext);


	//����Ÿ�� �� ����
	if (m_bHDR)
	{
		//FONTMGR->MessagePush("La���������L", 150, 200, 15, YT_Color(255, 0, 0), TEXT("HDR Use"));
		m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3drtvLight, m_pd3dDepthStencilView);
	}
	else
	{
		//FONTMGR->MessagePush("La���������L", 150, 200, 15, YT_Color(255, 0, 0), TEXT("HDR Not Use"));
		m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);
	}

	//������ ������
	m_pTextureToScreenShader->Render(m_pd3dDeviceContext);

	
	//--------------------------------------------------------------------------Deferred Rendering------------------------------------------------------//

	#else
		if (m_pScene) m_pScene->OnPreRender(m_pd3dDeviceContext);
		float fClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
		if (m_pd3dRenderTargetView) CGameFramework::m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
		if (m_pd3dDepthStencilView)	CGameFramework::m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		if (m_pd3dRenderTargetView) CGameFramework::m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);
		if (m_pScene) CGameFramework::m_pScene->Render(m_pd3dDeviceContext);
	#endif
	#endif


	//������ 
	float fAdaptationNorm;
	static float fAdaptation = 1.0f;
	static bool s_bFirstTime = true;
	if (s_bFirstTime)
	{
		// On the first frame we want to fully adapt the new value so use 0
		fAdaptationNorm = 0.0f;
		s_bFirstTime = false;
	}
	else
	{
		// Normalize the adaptation time with the frame time (all in seconds)
		// Never use a value higher or equal to 1 since that means no adaptation at all (keeps the old value)
		fAdaptationNorm = (fAdaptation < 0.0001f ? 1.0f : TIMEMGR->GetTimeElapsed() / fAdaptation, 0.9999f);
	}
	
	m_pPostManager->SetParameters(m_fMiddleGrey, m_fWhite, fAdaptationNorm, m_fBloomThreshold, m_fBloomScale);

	if (m_bHDR)
	{
		//FONTMGR->MessagePush("La���������L", 150, 150, 15, YT_Color(255, 0, 0), TEXT("HDR Use"));
		m_pPostManager->PostProcessing(m_pd3dDeviceContext, m_pd3dsrvLight, m_pd3dRenderTargetView);
	}
	//else 
	//	FONTMGR->MessagePush("La���������L", 150, 150, 15, YT_Color(255, 0, 0), TEXT("HDR Not Use"));
	//FONTMGR->MessagePush("La���������L", 150, 250, 15, YT_Color(255, 0, 0), TEXT("g_fBloomScale %1.2f"), m_fBloomScale);
	//FONTMGR->MessagePush("La���������L", 150, 300, 15, YT_Color(255, 0, 0), TEXT("g_fMiddleGrey %1.2f"), m_fMiddleGrey);
	
	//m_pShadwosToScreenShader->Render(m_pd3dDeviceContext);
	
	m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	

	if (m_bUseRenderTargetView)
	{
		for (auto &srv : m_vScreenShader)
		{
			srv->Render(m_pd3dDeviceContext);
		}
	}
	//if( n != -1 && n < m_vScreenShader.size())
	//	m_vScreenShader[n]->Render(m_pd3dDeviceContext);
	
	m_wTwBar.Render();



	FONTMGR->Render(m_pd3dDeviceContext);
	//TWBAR->Render();

	INPUT->SetWheel(WHEEL_NON);

	m_pDXGISwapChain->Present(0, 0);

	TIMEMGR->GetFrameRate(m_pszBuffer + 12, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);	

	if (false == m_pScene->IsGamePlaying())
	{
		::DimensionsChange(YT_GameMode::GAME_MODE_DIALOG);
	}

}

//Thread
#ifdef _MULIT_THREAD_RENDER_
void CGameFramework::InitializeWorkerThreads()
{
	m_nRenderThreads = RENDER_END;
	m_vRenderingThreadInfo.clear();
	m_vRenderingThreadInfo.assign(m_nRenderThreads, CRenderingThreadInfo());
	m_hRenderingEndEvents = new HANDLE[m_nRenderThreads];

	auto pNowSelectInfo = m_vRenderingThreadInfo.begin();
	for (int nThread = 0; nThread < m_nRenderThreads; ++nThread, ++pNowSelectInfo)
	{
		pNowSelectInfo->m_nRenderingThreadID = nThread;
		pNowSelectInfo->m_pScene = m_pScene;
		pNowSelectInfo->m_pd3dDCommandList = nullptr;
		pNowSelectInfo->m_hRenderingBeginEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
		pNowSelectInfo->m_hRenderingEndEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

		m_hRenderingEndEvents[nThread] = pNowSelectInfo->m_hRenderingEndEvent;

		m_pd3dDevice->CreateDeferredContext(0, &pNowSelectInfo->m_pd3dDeferredContext);

		pNowSelectInfo->m_pScene->SetViewport(pNowSelectInfo->m_pd3dDeferredContext);


		//KYT '16.02.24 
		/*OrthoCamera, Fog �� ��� ���۵����ؽ�Ʈ���� �ε�������Ѵ�.*/

		//pNowSelectInfo->m_pScene->Fog(m_pd3dDevice, pNowSelectInfo->m_pd3dDeferredContext);

		PLAYER->OrthoCameraLoad(m_pd3dDevice, pNowSelectInfo->m_pd3dDeferredContext);

		pNowSelectInfo->m_hRenderingThread = (HANDLE)::_beginthreadex
		(
			nullptr,
			0,
			DeferredContextThreadProc,
			&(*pNowSelectInfo),
			CREATE_SUSPENDED,
			nullptr
		);

		::ResumeThread(pNowSelectInfo->m_hRenderingThread);
	}
}
#endif

#ifdef DX12_MIGRATION
WARP_RESULT_ENUM CGameFramework::CreateDeivce()
{
	HRESULT hResult = S_OK;
	IDXGIAdapter1* pd3dAdapter = nullptr;

#ifdef _DEBUG
	hResult = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&m_pd3dDebugController);
	m_pd3dDebugController->EnableDebugLayer();
#endif //_DEBUG

	if (FAILED(hResult = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (LPVOID*)&m_pdxgiFactory)))
	{
		MessageBox(m_hWnd, _XTT("DXGIFactory������ ��ü ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), _XTT("���α׷� ���� ����"), MB_OK);
		return WARP_RESULT_ENUM::FAIL;
	}

#ifdef _SELECT_GPU_DRIVE_170904
	size_t ui64VideoMemory;
	IDXGIAdapter1* pAdapter;
	DXGI_ADAPTER_DESC1 adapterDesc;

	// DirectX �׷��� �������̽� ���丮�� ����ϴ�. 
	if (FAILED(m_pdxgiFactory->EnumAdapters1(0, (IDXGIAdapter1**)&pAdapter)))
	{
		MessageBox(m_hWnd, TEXT("��û�� �׷��� ī�� �������̽��� ���� ����Ͱ� �����ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
	}
	pAdapter->GetDesc1(&adapterDesc);
	ui64VideoMemory = (std::size_t)(adapterDesc.DedicatedVideoMemory + adapterDesc.SharedSystemMemory);

	// warp ������� ����׷���
	//���� �޸� ���ؼ� �� ���� GPU ã��
	int gpu_idx = 0;
	int select = 0;
	std::size_t comparison_videoMemory;
	std::map<unsigned long long, std::wstring, std::greater<unsigned long long >> GpuInfoMap;

	while (m_pdxgiFactory->EnumAdapters1(gpu_idx, &pAdapter) != DXGI_ERROR_NOT_FOUND)   // �׷��� ī�带 �˻�
	{
		pAdapter->GetDesc1(&adapterDesc);
		comparison_videoMemory = (std::size_t)(adapterDesc.DedicatedVideoMemory + adapterDesc.SharedSystemMemory);

		auto memory = comparison_videoMemory / 1024 / 1024;

		std::wstring tstr;
		tstr = TEXT("GPU");
		tstr += adapterDesc.Description;
		tstr += TEXT("\t[");
		tstr += std::to_wstring(gpu_idx);
		tstr += TEXT("] : ");
		GpuInfoMap.insert(std::make_pair(memory, tstr));
		if (comparison_videoMemory > ui64VideoMemory)
		{
			ui64VideoMemory = comparison_videoMemory;
			select = gpu_idx;
		}
		++gpu_idx;
	}
	for (const auto& m : GpuInfoMap)
	{
		std::wcout << m.second;
		std::wcout << m.first;
		std::wcout << "Mb" << std::endl;
	}
	wcout << _XTT("seletc ") << select << endl;
	m_pdxgiFactory->EnumAdapters1(select, &pAdapter);
#endif //  _SELECT_GPU_DRIVE_170904

	ID3D12Device* pDevice = nullptr;

	hResult = D3D12CreateDevice
	(
		pd3dAdapter,
		D3D_FEATURE_LEVEL_12_0,
		_uuidof(ID3D12Device),
		(void**)&pDevice
	);

	if (SUCCEEDED(hResult) == false)
	{
		hResult = D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), (void**)&pDevice);
	}

	if (pDevice == nullptr)
	{
		MessageBox(NULL, L"Direct3D 12 Device Cannot be Created.", L"Error", MB_OK);
		::PostQuitMessage(0);
		return WARP_RESULT_ENUM::FAIL;
	}

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount = 4;
	d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;
	hResult = pDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
	m_nMsaa4xQualityLevels = d3dMsaaQualityLevels.NumQualityLevels;
	m_bMsaa4xEnable = (m_nMsaa4xQualityLevels > 1) ? true : false;

	// CPU���� ���� �������״ϱ� ��ٷ���
	hResult = pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&m_pd3dFence);
	for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		m_nFenceValues[i] = 1;
	m_hFenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	setCBSrcDescriptorIncrementSize(pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	if (pd3dAdapter) pd3dAdapter->Release();

	gSetDevice(pDevice);

	return WARP_RESULT_ENUM::OK;
}

WARP_RESULT_ENUM CGameFramework::CreateCommandQueueAndList()
{
	ID3D12CommandAllocator* pd3dCommandAllocator = nullptr;
	ID3D12CommandQueue* pd3dCommandQueue = nullptr;
	ID3D12GraphicsCommandList* pd3dCommandList = nullptr;

	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	HRESULT hResult;
	if (gDevice == nullptr)
		return WARP_RESULT_ENUM::FAIL;
	hResult = gDevice->CreateCommandQueue(&d3dCommandQueueDesc, _uuidof(ID3D12CommandQueue), (void**)&pd3dCommandQueue);

	hResult = gDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&pd3dCommandAllocator);

	hResult = gDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pd3dCommandAllocator, nullptr, __uuidof(ID3D12GraphicsCommandList), (void**)&pd3dCommandList);

	hResult = pd3dCommandList->Close();

	CGlobalvariable::GetInstance()->SeCommandAllocator(pd3dCommandAllocator);
	CGlobalvariable::GetInstance()->SetCommandQueue(pd3dCommandQueue);
	CGlobalvariable::GetInstance()->SetGraphicsCommandList(pd3dCommandList);

	if (hResult != S_OK)
	{
		return WARP_RESULT_ENUM::FAIL;
	}

	return WARP_RESULT_ENUM::OK;
}

WARP_RESULT_ENUM CGameFramework::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT + RENTER_TARGET_BUFFER_COUNT;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	HRESULT hResult = gDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_pd3dRtvDescriptorHeap);
	m_nRtvDescriptorIncrementSize = gDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hResult = gDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_pd3dDsvDescriptorHeap);

	if (hResult != S_OK)
	{
		return WARP_RESULT_ENUM::FAIL;
	}

	return WARP_RESULT_ENUM::OK;
}

WARP_RESULT_ENUM CGameFramework::CreateSwapChain()
{
	m_nWndClientWidth = m_rcClient.right - m_rcClient.left;
	m_nWndClientHeight = m_rcClient.bottom - m_rcClient.top;

#ifdef _WITH_CREATE_SWAPCHAIN_FOR_HWND
	DXGI_SWAP_CHAIN_DESC1 dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));
	dxgiSwapChainDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.BufferCount = m_nSwapChainBuffers;
	dxgiSwapChainDesc.Scaling = DXGI_SCALING_NONE;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	dxgiSwapChainDesc.Flags = 0;
#else
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#endif
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxgiSwapChainFullScreenDesc;
	::ZeroMemory(&dxgiSwapChainFullScreenDesc, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));
	dxgiSwapChainFullScreenDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainFullScreenDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainFullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Windowed = TRUE;

	HRESULT hResult = m_pdxgiFactory->CreateSwapChainForHwnd(m_pd3dCommandQueue, m_hWnd, &dxgiSwapChainDesc, &dxgiSwapChainFullScreenDesc, NULL, (IDXGISwapChain1**)&m_pDXGISwapChain3);
#else
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.Windowed = TRUE;
#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	dxgiSwapChainDesc.Flags = 0;
#else
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#endif

	HRESULT hResult = m_pdxgiFactory->CreateSwapChain(gCommandQueue, &dxgiSwapChainDesc, (IDXGISwapChain**)&m_pDXGISwapChain);
#endif

	if (!m_pDXGISwapChain)
	{
		MessageBox(NULL, L"Swap Chain Cannot be Created.", L"Error", MB_OK);
		return WARP_RESULT_ENUM::FAIL;
	}

	hResult = m_pdxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);
	m_nSwapChainBufferIndex = m_pDXGISwapChain3->GetCurrentBackBufferIndex();

	return WARP_RESULT_ENUM::OK;
}
#endif //DX12_MIGRATION