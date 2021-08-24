#pragma once

#include "..\Assets\import\FW1FontWrapper_1_1\FW1FontWrapper.h"
#include "Draw_2D.h"

#define FAILED(hr) (((HRESULT)(hr)) < 0)

/*
사용법!

FONTMGR->MessagePush()에서 Message push해주면 나중에 Deffered Renderering에서 알아서 Render해줌.

*/


enum YT_TEXT
{
	PIVOT_TOP = FW1_TOP,
	PIVOT_LEFT = FW1_LEFT,
	PIVOT_HORIZONTAL_CENTER = FW1_CENTER,
	PIVOT_RIGHT = FW1_RIGHT,
	PIVOT_VERTICLE_CENTER = FW1_VCENTER,
	PIVOT_BOTTOM = FW1_BOTTOM
};

class YT_Color
{
private:
	UINT32	m_color32;

public:

	YT_Color() 
	{ 
		m_color32 = 0; 
	}

	YT_Color(int RGB)
	{
		SetColor(RGB); 
	};

	~YT_Color() {};

	YT_Color(FLOAT Red, FLOAT Green, FLOAT Blue, FLOAT Alpha = 255.0f)
	{
		UINT32 color32;
		BYTE *colorBytes = reinterpret_cast<BYTE*>(&color32);
		colorBytes[0] = static_cast<BYTE>(Red);
		colorBytes[1] = static_cast<BYTE>(Green);
		colorBytes[2] = static_cast<BYTE>(Blue);
		colorBytes[3] = static_cast<BYTE>(Alpha);

		m_color32 = color32;

	}


	void operator()(UINT32 color32) { m_color32 = color32; }

	void operator=(UINT32 color32) { m_color32 = color32; }

	void operator()(FLOAT Red, FLOAT Green, FLOAT Blue, FLOAT Alpha = 255.0f)
	{
		UINT32 color32;
		BYTE *colorBytes = reinterpret_cast<BYTE*>(&color32);
		colorBytes[0] = static_cast<BYTE>(Red);
		colorBytes[1] = static_cast<BYTE>(Green);
		colorBytes[2] = static_cast<BYTE>(Blue);
		colorBytes[3] = static_cast<BYTE>(Alpha);


		m_color32 = color32;
	}

	operator UINT32() const { return m_color32; }

	UINT32 ytRand_rgb_bri()
	{
		float rgb[3];
		rgb[0] = (float)((rand() % 255) *1.5);
		rgb[1] = (float)((rand() % 255) *1.2);
		rgb[2] = (float)((rand() % 255) *0.9);
		SetColor(rgb[0], rgb[1], rgb[2]);
	}

	//Set the color
	void SetColor(FLOAT Red, FLOAT Green, FLOAT Blue, FLOAT Alpha = 255.0f)
	{
		UINT32 color32;
		BYTE *colorBytes = reinterpret_cast<BYTE*>(&color32);
		colorBytes[0] = static_cast<BYTE>(Red);
		colorBytes[1] = static_cast<BYTE>(Green);
		colorBytes[2] = static_cast<BYTE>(Blue);
		colorBytes[3] = static_cast<BYTE>(Alpha);

		m_color32 = color32;
	}

	void SetColor(int RGB)
	{
		SetColor
		(
			Draw_2D::getRGB(RGB, 0)
			, Draw_2D::getRGB(RGB, 1)
			, Draw_2D::getRGB(RGB, 2)
		);
	}

};

class FontMessageType
{
public:
	std::string fontFaimly;
	std::wstring	  msg;
	float x;
	float y;
	float fontSize;
	UINT color = YT_Color(0, 0, 0);
	UINT pivot = (PIVOT_LEFT | PIVOT_TOP);


	FontMessageType
	(
		std::string fontFaimly = "기본"
		, std::wstring msg = L""
		, float fontSize = 15
		, float x = 0
		, float y = 0
		, UINT color = YT_Color(0, 0, 0)
		, UINT pivot = (PIVOT_LEFT | PIVOT_TOP)
	)
		: fontFaimly(fontFaimly)
		, msg(msg)
		, fontSize(fontSize)
		, x(x)
		, y(y)
		, color(color)
		, pivot(pivot)
	{
	}

	~FontMessageType() {}
};




class CFontManager : public CSingleTonBase<CFontManager>
{
private:
	ID3D11Device*								m_pd3dDevice;

	class FontType
	{
	public:
		std::string		 name;
		std::wstring	 fontFamily;
		FontType(std::string name, std::wstring fontFamily) : name(name), fontFamily(fontFamily) {}
		~FontType() {}
	};


	IFW1Factory*								 m_pFW1FontFactory;

	std::map<std::string
		, std::unique_ptr<IFW1FontWrapper>>		 m_mapFW1Font;
	IFW1TextRenderer*							 m_pFWTextRender;
	IFW1GlyphProvider*							 m_pFW1Glyphrovider;

	std::deque<FontMessageType>					 m_queMessage;

private:
	//Default Text Render
	void RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::string text, float fontSize,
		float posX, float posY, UINT32 color = YT_Color(0, 0, 0), UINT pivot = (PIVOT_LEFT | PIVOT_TOP));

	void RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::wstring text, float fontSize,
		float posX, float posY, UINT32 color = YT_Color(0, 0, 0), UINT pivot = (PIVOT_LEFT | PIVOT_TOP));

	//SetingFont Render
	void RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::string font, std::string text, float fontSize,
		float posX, float posY, UINT32 color = YT_Color(0, 0, 0), UINT pivot = (PIVOT_LEFT | PIVOT_TOP));

	void RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::string font, std::wstring text, float fontSize,
		float posX, float posY, UINT32 color = YT_Color(0, 0, 0), UINT pivot = (PIVOT_LEFT | PIVOT_TOP));

	void RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::string font, TCHAR* msg, float fontSize,
		float posX, float posY, UINT32 color = YT_Color(0, 0, 0), UINT pivot = (PIVOT_LEFT | PIVOT_TOP));

	void RenderText(ID3D11DeviceContext* pd3dDeviceContext, FontMessageType fontMessage);

public:
	CFontManager();

	~CFontManager();

	void ReleseInstance();

	void SetShader(ID3D11DeviceContext* pd3dDeviceContext, std::string name = "");

	void SetDynamic(bool check, std::string name = "");

	void Render(ID3D11DeviceContext* pd3dDeviceContext);

	void MessagePush(std::string font, float posX, float posY, float fontSize, UINT32 color, UINT pivot, TCHAR* msg, ...);

	void MessagePush(float posX, float posY, float fontSize, UINT32 color, UINT pivot, TCHAR* msg, ...);

	void MessagePush(float posX, float posY, float fontSize, UINT32 color, TCHAR* msg, ...);

	void MessagePush(std::string font, float posX, float posY, float fontSize, UINT32 color, TCHAR* msg, ...);

	void MessagePush(std::string font, float posX, float posY, float fontSize, TCHAR* msg, ...);

	void MessagePush(std::string font, std::string msg,
		float posX, float posY, float fontSize = 15, UINT32 color = YT_Color(0, 0, 0), UINT pivot = (PIVOT_LEFT | PIVOT_TOP));

	void MessagePush(FontMessageType fontMessageType);

	//void RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::wstring text, float fontSize,
	//	FW1_RECTF* rct, UINT32 color, UINT pivot);
	//

public:
	void CFontManager::Load(ID3D11Device* pd3dDevice);

	void CFontManager::Destroy();

private:

	void CFontManager::AddFont(FontType font);

	IFW1FontWrapper* CFontManager::FindFont(std::string name);


};


