#include "stdafx.h"
#include "DefferedThreadProc.h"
#include "RenderingThreadInfo.h"
#include "GameFramework.h"

#ifdef _MULIT_THREAD_RENDER_

UINT WINAPI DeferredContextThreadProc(LPVOID lpParam)
{
	CRenderingThreadInfo *pRenderingThreadInfo = reinterpret_cast<CRenderingThreadInfo*>(lpParam);
	ID3D11DeviceContext *pd3dDeferredContext = pRenderingThreadInfo->m_pd3dDeferredContext;
		
	while (true)
		{
			::WaitForSingleObject(pRenderingThreadInfo->m_hRenderingBeginEvent, INFINITE);
			int nThread = pRenderingThreadInfo->m_nRenderingThreadID;
			pRenderingThreadInfo->m_pScene->RenderAway(pd3dDeferredContext);
			//LIGHTMGR->UpdateConstBuffer(pd3dDeferredContext);

			switch (nThread)
			{
			case RENDER_BASIC:
				pRenderingThreadInfo->m_pScene->RenderBasic(pd3dDeferredContext);
				break;

			case RENDER_BLEND:
				pRenderingThreadInfo->m_pScene->RenderBlend(pd3dDeferredContext);
				break;

			case RENDER_SPACE_PARTITON:
				pRenderingThreadInfo->m_pScene->RenderSpacePartition(pd3dDeferredContext);
				break;

			case RENDER_ENVIROMENT:
				pRenderingThreadInfo->m_pScene->RenderEnvironment(pd3dDeferredContext);
				break;
			}
			pd3dDeferredContext->FinishCommandList(true, &pRenderingThreadInfo->m_pd3dDCommandList);
			::SetEvent(pRenderingThreadInfo->m_hRenderingEndEvent);
		}
	return false;
}

#endif


