#pragma once

#include "../Assets/import/AntTweakBar/include/AntTweakBar.h"

class CTwBarManager : public Object
{
	
public:
	CTwBarManager();

	~CTwBarManager();

	void Release() override;

	bool Load(ID3D11Device* pd3dDevice);

	void AddBar(const std::string& str_category, std::string name, int width, int height);

	TwBar* Find(const std::string& name, const int& width = 0, const int& height = 0);

	TwBar* Find(const std::string& name, int color[3], const int& width = 0, const int& height = 0);

	void Temp_Inesrt(void* onoff_hdr, void* fMiddleGrey, void* fBloomScale);

	void Render();

	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	std::map<
		std::string, TwBar*> m_mTwBar;

};



