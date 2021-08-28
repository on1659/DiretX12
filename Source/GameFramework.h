#pragma once

#include <Windows.h>
#include "Timer.h"
#include "Player.h"
#include "Scene.h"
#include "SingleTon.h"
#include "TerrainPlayer.h"
#include "PostFX.h"
#include "PostProcessingManager.h"
#include "RenderTargetShader.h"
#include "TwBarManager.h"

#ifdef _MULIT_RENDER_TARGET_
class CScreenShader;
class CRenderingThreadInfo;
#endif // _MULIT_THREAD_RENDER_

class CPostProcessingRenderContainer;

class CGameFramework : public CSingleTonBase<CGameFramework>
{


public:
	CGameFramework();

	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	bool OnCreate();

	void ThreadCreate();

	void OnDestroy();

	bool CreateRenderTargetDepthStencilView();
	bool CreateDirect3DDisplay();

	void CreateConstBuffers();
	void ReleaseConstBuffers();

	void BuildObjects();
	void ReleaseObjects();

	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();
	void OnCollision();

	void SceneRelease();
	
	void SceneLoad();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE								m_hInstance;
	HWND									m_hWnd;
	HMENU									m_hMenu;

	int										m_nWndClientWidth;	
	int										m_nWndClientHeight;
	RECT									m_rcClient;

	ID3D11Device							*m_pd3dDevice;
	ID3D11DeviceContext						*m_pd3dDeviceContext;
	IDXGISwapChain							*m_pDXGISwapChain;
	UINT									m_n4xMSAAQualities;

	CScene*									m_pScene;

	CPlayer									*m_pPlayer;
	CCamera									*m_pCamera;


	POINT									m_ptOldCursorPos;
	_TCHAR									m_pszBuffer[50];

	ID3D11Texture2D							*m_pd3dDepthStencilBuffer;
	ID3D11DepthStencilView					*m_pd3dDepthStencilView;

	bool bControlCV;
public:
	ID3D11RenderTargetView					*m_pd3dRenderTargetView;
	ID3D11ShaderResourceView				*m_pd3dMainShaderResourceView;
	ID3D11Texture2D							*m_pd3dMainBackBufferTextrue;

private:
	//KYT '16.01.25 plus
	/*
	스크린샷기능 추가
	*/
	ID3D11ShaderResourceView				*m_pd3dScreenCaptureResourceView;
	std::vector<std::thread*>				m_vThread;
	bool									m_bLoad;

#ifdef _CONNECT_SERVER_

	//JJY '16'.04.01 plus
	float m_fCurrentCount = 0.0f;
	float m_fOldCount = 0.0f;

#endif

private:
	std::vector<CScene*>					 m_State;


public:
	ID3D11DepthStencilView* GetDepthStencilView() { return m_pd3dDepthStencilView; }


public:


#ifdef _MULIT_THREAD_RENDER_
private:
	int										m_nRenderThreads;
	std::vector<CRenderingThreadInfo>		m_vRenderingThreadInfo;
	HANDLE									*m_hRenderingEndEvents;

public:

	void InitializeWorkerThreads();

#endif

public:
	ID3D11DeviceContext* GetDeviceContext() { return m_pd3dDeviceContext; }
	ID3D11Device* GetDevice() { return m_pd3dDevice; }

#ifdef _MULIT_RENDER_TARGET_

public:
	bool CreateRenderTargetDepthStencilViewMRT();

	void LoadScreenShader(ID3D11Device* pd3dDevice);

private:
	ID3D11Texture2D			 *m_pd3dtxtDiffuse;		//0
	ID3D11Texture2D			 *m_pd3dtxtPosition;	//1
	ID3D11Texture2D			 *m_pd3dtxtNormal;		//2
	//ID3D11Texture2D			 *m_pd3dtxtSpecular;//3
	//ID3D11Texture2D			 *m_pd3dtxtAmbient;	//4
	ID3D11Texture2D			 *m_pd3dtxtShadowMap;	//5

	ID3D11ShaderResourceView *m_pd3dsrvDiffuse;		//0
	ID3D11ShaderResourceView *m_pd3dsrvPosition;	//1
	ID3D11ShaderResourceView *m_pd3dsrvNormal;		//2
	//ID3D11ShaderResourceView *m_pd3dsrvSpecular;	//3
	//ID3D11ShaderResourceView *m_pd3dsrvAmbient;	//4
	ID3D11ShaderResourceView *m_pd3dsrvShadowMap;	//5


	ID3D11RenderTargetView   *m_pd3drtvDiffuse;		//0
	ID3D11RenderTargetView   *m_pd3drtvPosition;	//1
	ID3D11RenderTargetView   *m_pd3drtvNormal;		//2
	//ID3D11RenderTargetView   *m_pd3drtvSpecular;	//3
	//ID3D11RenderTargetView   *m_pd3drtvAmbient;	//4
	ID3D11RenderTargetView   *m_pd3drtvShadowMap;	//5

	std::shared_ptr<CScreenShader> m_pTextureToScreenShader;
	std::vector<
		std::shared_ptr<CRenderTargetShader>> m_vScreenShader;
	std::shared_ptr<CRenderTargetShader>	m_pShadowRTV;

#endif

public:
	void SetInstance(HINSTANCE hInstance) { m_hInstance = hInstance; }

	void SetHwnd(HWND hMainWnd) { m_hWnd = hMainWnd; }

	void SetScreenShader(std::shared_ptr<CScreenShader>& pShader);

private:
	bool					isPlaying;
	bool					timeTick;


//Post
private:
	//--------------------------light render target----------------
	ID3D11Texture2D			 *m_pd3dtxtLight;
	ID3D11ShaderResourceView *m_pd3dsrvLight;
	ID3D11RenderTargetView   *m_pd3drtvLight;
	//--------------------------light render target----------------

	std::unique_ptr<CPostProcessingManager> m_pPostManager;
	float m_fMiddleGrey;
	float m_fWhite;
	float m_fBloomThreshold;
	float m_fBloomScale;
	CTwBarManager					m_wTwBar;
	
	
	bool							m_bUseTwBar;
	bool							m_bHDR;
	bool							m_bUseRenderTargetView;


public:
	
	bool IsTwBar() const { return m_bUseTwBar; }

private:

	struct LIMLIGHT
	{
		float color[3] = { 1.0f, 0.0f, 0.0f };
		float fRimWidth = 0.8f;
		float fPower = 0.1f;
		float use = 1.0f;
		float padding[2];
	};

	struct OUTLINE_DATA
	{
		float fWidth;
		float color[3] = { 1.0f, 0.0f, 0.0f };
	};

	bool					m_bLimLight;
	LIMLIGHT			   *m_stLim;
	ID3D11Buffer*			m_pd3dLimBuffer;

	OUTLINE_DATA			*m_stOutLine;
	ID3D11Buffer			*m_pd3dOutLineBuffer;

	void SaveTwBarData(const std::string& path = "TwBarData.txt");
	
	void ReadTwBarData(const std::string& path = "TwBarData.txt");
	
	void UpdatePSConstBuffer();

	bool					m_bSaveButton;

	bool					m_bLoadButton;


#ifdef DX12_MIGRATION
private:
	IDXGISwapChain3* m_pDXGISwapChain3;

#ifdef _DEBUG
	ID3D12Debug* m_pd3dDebugController;
#endif //_DEBUG

	bool							m_bMsaa4xEnable;
	UINT							m_nMsaa4xQualityLevels;

	ComPtr<ID3D12Fence>				m_pd3dFence;
	UINT64							m_nFenceValues[SWAP_CHAIN_BUFFER_COUNT];
	HANDLE							m_hFenceEvent;
	UINT							m_nRtvDescriptorIncrementSize;
	ComPtr<IDXGIFactory4>			m_pdxgiFactory;

	UINT							m_nSwapChainBufferIndex;

	ComPtr<ID3D12DescriptorHeap>	m_pd3dRtvDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap>	m_pd3dDsvDescriptorHeap;


	WARP_RESULT_ENUM				CreateDeivce();
	WARP_RESULT_ENUM				CreateCommandQueueAndList();
	WARP_RESULT_ENUM				CreateRtvAndDsvDescriptorHeaps();
	WARP_RESULT_ENUM				CreateCommandComplete();
	WARP_RESULT_ENUM				CreateSwapChain();
#endif //DX12_MIGRATION

};

