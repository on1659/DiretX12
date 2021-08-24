#pragma once

#ifdef _MULIT_THREAD_RENDER_

class CScene;

enum MULITTHREADRENDER
{
	RENDER_ENVIROMENT,
	RENDER_BASIC,
	RENDER_SPACE_PARTITON,
	RENDER_BLEND,
	RENDER_END,
};

class CRenderingThreadInfo
{
public:
	int						m_nRenderingThreadID;
	HANDLE					m_hRenderingThread;
	HANDLE					m_hRenderingBeginEvent;
	HANDLE					m_hRenderingEndEvent;
	ID3D11DeviceContext		*m_pd3dDeferredContext;
	ID3D11CommandList		*m_pd3dDCommandList;
	CScene*					 m_pScene;

public:
	CRenderingThreadInfo()
	{
		m_nRenderingThreadID = 0;
		m_hRenderingThread = nullptr;
		m_hRenderingBeginEvent = nullptr;
		m_hRenderingEndEvent = nullptr;
		m_pd3dDeferredContext = nullptr;
		m_pd3dDCommandList = nullptr;
		m_pScene = nullptr;
	}

	~CRenderingThreadInfo()
	{
		if (m_pd3dDeferredContext) m_pd3dDeferredContext->Release();
		if (m_hRenderingBeginEvent) ::CloseHandle(m_hRenderingBeginEvent);
		if (m_hRenderingEndEvent) ::CloseHandle(m_hRenderingEndEvent);
	}
};


#endif
