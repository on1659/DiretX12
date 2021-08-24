#pragma once

#include "stdafx.h"
#include "Camera.h"
#include "Shader.h"

class LIGHTS;

class CSceneState abstract
{
	protected:
		std::string					m_name;

	public:

	CSceneState(){}

	~CSceneState(){}


	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;

	virtual void BuildObjects(ID3D11Device *pd3dDevice) = 0;

	virtual void ReleaseObjects() = 0;

	virtual void AnimateObjects(float fTimeElapsed){}
	
	virtual bool ProcessInput(POINT ptOldCursorPos) = 0;
	
	virtual void OnPreRender(ID3D11DeviceContext *pd3dDeviceContext) = 0;

#ifdef _MULIT_THREAD_RENDER_

	//Render Away
	virtual void RenderAway(ID3D11DeviceContext *pd3DeviceContext) = 0;

	//Player, UI ...
	virtual void RenderBasic(ID3D11DeviceContext *pd3DeviceContext) = 0;

	//SkyBox, Terrain ...
	virtual void RenderEnvironment(ID3D11DeviceContext	*pd3dDeviceContext) = 0;

	//Blend Shader
	virtual void RenderBlend(ID3D11DeviceContext *pd3dDeviceContext) = 0;

	//Dynamic Shader
	virtual void RenderSpacePartition(ID3D11DeviceContext *pd3dDeviceContext) = 0;

#else
	//Non MTR
	virtual void Render(ID3D11DeviceContext *pd3DeviceContext) = 0;

#endif
	
	virtual void OnCollision(ID3D11DeviceContext *pd3dDeviceContext) = 0;
	
};

