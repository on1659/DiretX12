#include "stdafx.h"
#include "FontManager.h"

CFontManager::CFontManager()
{
	m_pFW1FontFactory = nullptr;
	m_pFWTextRender = nullptr;

	m_pFW1Glyphrovider = nullptr;

	m_queMessage.clear();

	m_pd3dDevice = nullptr;

}

CFontManager::~CFontManager()
{
}

void CFontManager::ReleseInstance()
{
	m_pd3dDevice = nullptr;
	if (m_pFW1FontFactory) m_pFW1FontFactory->Release();
	m_pFW1FontFactory = nullptr;

	for (auto &font : m_mapFW1Font)
	{
		font.second->Release();
		font.second.release();
	}
	if (m_pFWTextRender)m_pFWTextRender->Release();
	m_pFWTextRender = nullptr;

	if (m_pFW1Glyphrovider) m_pFW1Glyphrovider->Release();
	m_pFW1Glyphrovider = nullptr;

	m_queMessage.clear();

	CSingleTonBase::ReleseInstance();
}

void CFontManager::Load(ID3D11Device* pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1FontFactory)))
	{
		::GMessageBox(L"폰트를 만들 수 없습니다", L"폰트못만듬!");
		return;
	}

	AddFont(FontType("기본", L"a옛날목욕탕L"));
	AddFont(FontType("카툰", L"210 카툰스토리L"));

}

void CFontManager::AddFont(FontType font)
{
	if (nullptr != FindFont(font.name)) return;

	IFW1FontWrapper* pFW1Font = nullptr;

	if (FAILED(m_pFW1FontFactory->CreateFontWrapper(m_pd3dDevice, font.fontFamily.c_str(), &pFW1Font)))
	{
		::GMessageBox("폰트 로딩오류", "폰트없음");
		return;
	}

	std::unique_ptr<IFW1FontWrapper> pUniFont = nullptr;
	pUniFont.reset(pFW1Font);

	m_mapFW1Font.insert(make_pair(font.name, move(pUniFont)));
	pUniFont.reset(nullptr);
}

IFW1FontWrapper *CFontManager::FindFont(std::string name)
{
	if (0 == m_mapFW1Font.size())
	{
		return nullptr;
	}

	if (name.size() == 0)
		return m_mapFW1Font.begin()->second.get();

	auto iter = m_mapFW1Font.find(name);
	if (m_mapFW1Font.end() != iter)
		return iter->second.get();
	return nullptr;
}

void CFontManager::Destroy()
{
	if (m_pFW1FontFactory)
	{
		m_pFW1FontFactory->Release();
		m_pFW1FontFactory = nullptr;
	}

	for (auto &m : m_mapFW1Font)
	{
		m.second.get()->Release();	//* Release()
		m.second.release();			//unique() Release();
		m.second = nullptr;
	}
	m_mapFW1Font.clear();
	m_queMessage.clear();
}

void CFontManager::RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::string msg, float fontSize, float posX, float posY, UINT32 color, UINT pivot)
{
	if (false == m_mapFW1Font.begin()->second) return;

	TCHAR* tChar = new TCHAR[msg.size()];
	mbstowcs(tChar, msg.c_str(), ((int)strlen(msg.c_str()) + 1));

	/// <param name="Color">The color of the text, as 0xAaBbGgRr.</param>
	m_mapFW1Font.begin()->second->DrawString(
		pd3dDeviceContext,
		tChar,
		fontSize,
		posX,
		posY,
		color,
		pivot);

}

void CFontManager::RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::wstring msg, float fontSize, float posX, float posY, UINT32 color, UINT pivot)
{
	if (false == m_mapFW1Font.begin()->second) return;

	/// <param name="Color">The color of the text, as 0xAaBbGgRr.</param>
	m_mapFW1Font.begin()->second->DrawString
	(
		pd3dDeviceContext,
		msg.c_str(),
		fontSize,
		posX,
		posY,
		color,
		pivot
	);
}

void CFontManager::RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::string font, std::string msg, float fontSize, float posX, float posY, UINT32 color, UINT pivot)
{
	auto fontIter = FindFont(font);
	if (false == fontIter) return;

	TCHAR* tChar = new TCHAR[msg.size()];
	mbstowcs(tChar, msg.c_str(), ((int)strlen(msg.c_str()) + 1));

	/// <param name="Color">The color of the text, as 0xAaBbGgRr.</param>
	fontIter->DrawString
	(
		pd3dDeviceContext,
		tChar,
		fontSize,
		posX,
		posY,
		color,
		pivot
	);
}

void CFontManager::RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::string font, std::wstring msg, float fontSize, float posX, float posY, UINT32 color, UINT pivot)
{
	auto fontIter = FindFont(font);
	if (false == fontIter) return;

	/// <param name="Color">The color of the text, as 0xAaBbGgRr.</param>
	fontIter->DrawString
	(
		pd3dDeviceContext,
		msg.c_str(),
		fontSize,
		posX,
		posY,
		color,
		pivot
	);
}

void CFontManager::RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::string font, TCHAR *msg, float fontSize, float posX, float posY, UINT32 color, UINT pivot)
{
	auto fontIter = FindFont(font);
	if (false == fontIter) return;

	WCHAR wMsg[128];
	_stprintf_s(wMsg, _countof(wMsg), msg, msg);

	/// <param name="Color">The color of the text, as 0xAaBbGgRr.</param>
	fontIter->DrawString
	(
		pd3dDeviceContext,
		wMsg,
		fontSize,
		posX,
		posY,
		color,
		pivot
	);
}


void CFontManager::RenderText(ID3D11DeviceContext * pd3dDeviceContext, FontMessageType fontMessage)
{
	RenderText(pd3dDeviceContext, fontMessage.fontFaimly, fontMessage.msg, fontMessage.fontSize, fontMessage.x, fontMessage.y, fontMessage.color, fontMessage.pivot);
}

void CFontManager::SetShader(ID3D11DeviceContext * pd3dDeviceContext, std::string name)
{
	FindFont(name)->SetShader(pd3dDeviceContext, YT_TEXT::PIVOT_LEFT);
}

void CFontManager::SetDynamic(bool check, std::string name)
{
	FindFont(name)->SetDynamic(check);
}

void CFontManager::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	//KYT '16.10.01
	/*
		영상용
	*/

	while (false == m_queMessage.empty())
	{
		RenderText(pd3dDeviceContext, m_queMessage.front());
		m_queMessage.pop_front();
	}
}

void CFontManager::MessagePush(std::string font, float posX, float posY, float fontSize, UINT32 color, UINT pivot, TCHAR * msg, ...)
{
	va_list arg;
	va_start(arg, msg);
	wchar_t buf[500] = { NULL };
	vswprintf(buf, msg, arg);
	int size = 0;
	for (int i = 0; buf[i] != NULL; ++i) size++;
	MessagePush((FontMessageType(font, buf, fontSize, posX, posY, color, pivot)));

}

void CFontManager::MessagePush(float posX, float posY, float fontSize, UINT32 color, UINT pivot, TCHAR * msg, ...)
{
	va_list arg;
	va_start(arg, msg);
	TCHAR buf[500] = { NULL };
	vswprintf(buf, msg, arg);
	int size = 0;
	for (int i = 0; buf[i] != NULL; ++i) size++;
	MessagePush((FontMessageType(m_mapFW1Font.begin()->first, buf, fontSize, posX, posY, color, pivot)));

}

void CFontManager::MessagePush(float posX, float posY, float fontSize, UINT32 color, TCHAR * msg, ...)
{
	va_list arg;
	va_start(arg, msg);
	TCHAR buf[500] = { NULL };
	vswprintf(buf, msg, arg);
	int size = 0;
	for (int i = 0; buf[i] != NULL; ++i) size++;

	MessagePush((FontMessageType(m_mapFW1Font.begin()->first, buf, fontSize, posX, posY, color)));

}

void CFontManager::MessagePush(std::string font, float posX, float posY, float fontSize, UINT32 color, TCHAR * msg, ...)
{
	va_list arg;
	va_start(arg, msg);
	TCHAR buf[500] = { NULL };
	vswprintf(buf, msg, arg);
	int size = 0;
	for (int i = 0; buf[i] != NULL; ++i) size++;
	std::wstring wstr;
	wstr.append(buf);
	MessagePush((FontMessageType(m_mapFW1Font.begin()->first, wstr, fontSize, posX, posY, color)));

}

void CFontManager::MessagePush(std::string font, float posX, float posY, float fontSize, TCHAR * msg, ...)
{
	va_list arg;
	va_start(arg, msg);
	TCHAR buf[500] = { NULL };
	vswprintf(buf, msg, arg);
	int size = 0;
	for (int i = 0; buf[i] != NULL; ++i) size++;
	std::wstring wstr;
	wstr.append(buf);
	MessagePush((FontMessageType(m_mapFW1Font.begin()->first, wstr, fontSize, posX, posY, YT_Color(0,0,0))));

}

void CFontManager::MessagePush(std::string font, std::string msg, float posX, float posY, float fontSize, UINT32 color, UINT pivot)
{
	TCHAR* tChar = new TCHAR[msg.size()];
	mbstowcs(tChar, msg.c_str(), ((int)strlen(msg.c_str()) + 1));
	MessagePush(FontMessageType(font, tChar, fontSize, posX, posY, color, pivot));
}

void CFontManager::MessagePush(FontMessageType fontMessageType)
{
	//if (m_queMessage.size() > 1000) m_queMessage.clear();
	m_queMessage.push_back(fontMessageType);
}

//void CFontManager::RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::wstring text, float fontSize, FW1_RECTF* rct, UINT32 color, UINT pivot)
//{
//	if (!m_pFW1Font) return;
//
//	m_pFW1Font->DrawString(
//		pd3dDeviceContext,
//		text.c_str(),
//		NULL,
//		fontSize,
//		rct,
//		color,
//		NULL,
//		NULL,
//		pivot);
//
//}


