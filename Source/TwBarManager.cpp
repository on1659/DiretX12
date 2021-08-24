#include "stdafx.h"
#include "TwBarManager.h"


CTwBarManager::CTwBarManager()
{
}


CTwBarManager::~CTwBarManager()
{
}

void CTwBarManager::Release()
{
	Object::Release();
	TwTerminate();

}

bool CTwBarManager::Load(ID3D11Device * pd3dDevice)
{
	// Initialize AntTweakBar
	if (!TwInit(TW_DIRECT3D11, pd3dDevice))
	{
		MessageBoxA(g_hWnd, TwGetLastError(), "AntTweakBar initialization failed", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}
TwBar* CTwBarManager::Find(const std::string& name,const int& width, const int& height)
{
	//auto iter = m_mTwBar.find(name);
	//
	//if (iter == m_mTwBar.end())
	//{
	//	auto bar = std::make_shared<TwBar>(name);
	//	int size[2] = { width, height };
	//	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	//	TwSetParam(bar.get(), NULL, "size", TW_PARAM_DOUBLE, 2, size);
	//	return bar;
	//}
	//return iter->second;
	return nullptr;
}

TwBar* CTwBarManager::Find(const std::string & name, int color[3], const int & width, const int & height)
{
	//auto iter = m_mTwBar.find(name);
	//
	//if (iter == m_mTwBar.end())
	//{
	//	auto bar = std::make_shared<TwBar>(name);
	//	int size[2] = { width, height };
	//	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	//	TwSetParam(bar.get(), NULL, "size", TW_PARAM_DOUBLE, 2, size);
	//	TwSetParam(bar.get(), NULL, "color", TW_PARAM_INT32, 3, color);
	//	return bar;
	//}
	//return iter->second;
	return nullptr;
}

void CTwBarManager::AddBar(const std::string& str_category, std::string name, int width, int height)
{
	//auto twbar = Find(str_category);

//	TwAddVarRW(twbar.get(), name.c_str(), TW_TYPE_FLOAT, &fTime, "min=0 max=4 step=0.01 keyincr=F1 keydecr=F2 help='Scale the object (1=original size).'"); // bar

}

void CTwBarManager::Temp_Inesrt(void* bHDR, void* fMiddleGrey, void* fBloomScale)
{

	int pos[2] = { 30, 150 };
	int size[2] = { 320, 700 };
	int color[3] = { 237, 180, 237 };

	TwBar* twBar = TwNewBar("HDR");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	TwSetParam(twBar, NULL, "size", TW_PARAM_DOUBLE, 2, size);
	TwSetParam(twBar, NULL, "color", TW_PARAM_INT32, 3, color);
	TwSetParam(twBar, NULL, "position", TW_PARAM_INT32, 2, pos);

	TwAddVarRW(twBar, "MiddleGrey", TW_TYPE_FLOAT, &fMiddleGrey, "min=0 max=10 step=0.01 keyincr=F9 keydecr=F10 help='Scale the object (1=original size).'"); // bar
	TwAddVarRW(twBar, "BloomScale", TW_TYPE_FLOAT, &fBloomScale, "min=0 max=10 step=0.1 keyincr=F7 keydecr=F8 help='Scale the object (1=original size).'"); // bar
	TwAddVarRW(twBar, "HDR", TW_TYPE_BOOL8, &bHDR, "key=l");


	m_mTwBar["HDR"] = std::move(twBar);

}

void CTwBarManager::Render()
{
	TwDraw();
}

bool CTwBarManager::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return TwEventWin(hWnd, nMessageID, wParam, lParam);
}

