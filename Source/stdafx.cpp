// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// LabProject13-3.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

#include "GameFrameWork.h"
#include "GameFrameWork_2D.h"

HWND g_hWnd;
std::thread* loadingThread;

int GMessageBox(std::string _title, std::string _message)
{
	const char* msgChar = _message.c_str();
	int msgLen = 1 + strlen(msgChar);
	TCHAR* message = new TCHAR[msgLen];
	mbstowcs(message, msgChar, msgLen);
	
	const char* titleChar = _title.c_str();
	int titleLen = 1 + strlen(titleChar);
	TCHAR* title = new TCHAR[titleLen];
	mbstowcs(title, titleChar, titleLen);

	return MessageBox(nullptr, message, title, MB_OK);
}

int GMessageBox(TCHAR* title, TCHAR* message)
{
	return MessageBox(nullptr, message, title, MB_OK);
}

int RandomRange(int start, int end)
{
	std::uniform_int_distribution<> distribution(start, end); // ★ Probability Distribution.
	return (distribution(std::default_random_engine()));
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[])
{
	int nLen = (int)strlen(pstrSrc) + 1;
	mbstowcs(pwstrDest, pstrSrc, nLen);
}

TCHAR* tChar(LPCSTR pszShaderName)
{
	size_t size = strlen(pszShaderName);
	TCHAR* tStr = new TCHAR[size];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszShaderName, (int)size, tStr, 256);
	tStr[size] = NULL;
	return tStr;
}

TCHAR* StringToTCHAR(std::string& s)
{
	const char* all = s.c_str();
	size_t len = 1 + strlen(all);
	wchar_t* t = new wchar_t[len];
	if (nullptr == t) throw std::bad_alloc();
	mbstowcs(t, all, len);
	return (TCHAR*)t;
}

bool gbGameMode = YT_GameMode::GAME_MODE_DIALOG;

void GTimeRecord(bool start, std::string str)
{
	if (start) cStart = std::chrono::high_resolution_clock::now();
	else
	{
		auto d = std::chrono::high_resolution_clock::now() - cStart;
		std::cout << str << "Time : " << std::chrono::duration_cast<std::chrono::milliseconds>(d).count() * 0.001f << "s \n";
	}

}


void ConsoleShow(XMFLOAT3 x3, std::string msg)
{
	std::cout << msg << "(" << x3.x << "," << x3.y << "," << x3.z << ")" << std::endl;
}

XMFLOAT4 gBulrMode = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);

void GBulrChange(YT_BlurMode state)
{
	if (state == YT_BlurMode::BLUR_NOTHING)
	{
		if (gBulrMode.y + gBulrMode.z + gBulrMode.w == 0)
			return;
		else
		{
			gBulrMode.y = YT_BlurMode::BLUR_NOTHING;
		}
	}

	gBulrMode.x = YT_BlurMode::BLUR_CHANGE;

	if (state == YT_BlurMode::BLUR_RUN)
	{
		gBulrMode.y = YT_BlurMode::BLUR_RUN;
	}

	if (state == YT_BlurMode::BLUR_FLASH)
	{
		gBulrMode.z = 8;// Helper::RuntimeDataModify(XMFLOAT3(), "f_FlashBang").x;
	}

	if (state == YT_BlurMode::BLUR_RESPAWN)
	{
		gBulrMode.w = YT_BlurMode::BLUR_RESPAWN;// Helper::RuntimeDataModify(XMFLOAT3(), "f_FlashBang").x;
	}

}