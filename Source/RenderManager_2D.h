#pragma once

#include "stdafx.h"


enum eNumImage
{
	IMAGE_TEST,
	IMAGE_BOOM,
	IMAGE_END,
};

class Image
{
private:
	CImage*					m_image;
	int						m_Width;
	int						m_Height;

public:
	Image();
	Image(TCHAR* filePath);
	~Image();

	void Init(TCHAR* filePath);

	void LoadCImage(TCHAR* filePath);

	int GetWidth(){ return m_Width; }

	int GetHeight(){ return m_Height; }

	CImage* GetCimage(){ return m_image; }

	Image* GetImage(){ return this; }

	void Release() { m_image->Destroy(); }

};

class CRenderManager_2D : public CSingleTonBase<CRenderManager_2D>
{

private:
	std::map<std::string, std::vector<Image>> m_ImageMap;

public:

	void LoadCImage();

	void Release();

	void AddImage(TCHAR* filePath, std::string name);

	void Render(HDC hdc, std::string name, float left, float top, float width, float height, float s_left, float s_top, float s_width, float s_height, COLORREF removeColor);

	void DrawImageSpriteAlphaBackground(HDC hdc, std::string name, float left, float top, float width, float height, float s_left, float s_top, float s_width, float s_height, COLORREF removeColor, int alpha);

	Image* GetImage(std::string name);

	std::vector<Image>*	 FindImage(std::string name);

public:
	CRenderManager_2D();

	~CRenderManager_2D();



};

