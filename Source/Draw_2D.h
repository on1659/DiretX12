#pragma once


#include <windows.h>

enum Color_2D{
	RED_2D,			//0
	BLUE_2D,			//1
	GREEN_2D,			//2	
	BLACK_2D,			//3
	ORANGE_2D,			//4
	CYAN_2D,			//5
	MAGENTA_2D,		//6
	YELLOW_2D,			//7



	BRIGHT_RED_1_2D,			//238,20,45
	BRIGHT_RED_2_2D,			//229 52 165
	BRIGHT_RED_3_2D,			//231 15 62
	BRIGHT_RED_4_2D,			//246 122 218
	BRIGHT_RED_5_2D,			//238 5 33
	BRIGHT_RED_6_2D,			//243 58 60

	BRIGHT_YELLOW_1_2D,		// 235 249,81
	BRIGHT_YELLOW_2_2D,		//244,229,109
	BRIGHT_YELLOW_3_2D,		//244 213 28
	BRIGHT_YELLOW_4_2D,		//253 218 76
	BRIGHT_YELLOW_5_2D,		//228,234,92
	BRIGHT_YELLOW_6_2D,		//234,228,106

	BRIGHT_ORANGE_1_2D,		//241 132 18
	BRIGHT_ORANGE_2_2D,		//244 129 83

	BRIGHT_GREEN_1_2D,		//10  115 225 121
	BRIGHT_GREEN_2_2D,		// 102 250 82
	BRIGHT_GREEN_3_2D,		//177,244,83
	BRIGHT_GREEN_4_2D,		//117 226 153
	BRIGHT_GREEN_5_2D,		//7, 226, 129
	BRIGHT_GREEN_6_2D,		//111,226,120
	BRIGHT_GREEN_7_2D,		//90,254,108
	BRIGHT_GREEN_8_2D,		//111 226 120
	BRIGHT_GREEN_9_2D,		//163 255 118

	BRIGHT_BLUE_1_2D,		//11 33 201 216
	BRIGHT_BLUE_2_2D,		//77,94,209
	BRIGHT_BLUE_4_2D,		//84 216 177
	BRIGHT_BLUE_3_2D,	//30,168,194

	//보라색이문제가있네

	WHITE_2D,			//10
	BRUSH_2D,			//8
	BRUSH2_2D,			//9
	BRIGHT_BLACK_2D,
	CHANGE_2D,
	RANDOM_2D,			//11
	REMOVE_COLOR_2D,
	REMOVE_STARPIX_2D,
	REMOVE_GHOSTPIX_2D,
	REMOVE_STONEBALL_2D,
	REMOVE_RUNAPIX_2D,
	REMOVE_SOLARPIX_2D,
	REMOVE_BUTTON_2D,
	REMOVE_BUTTON2_2D,
	RANDOBJECT_COLOR_2D,

};

class Draw_2D
{

public:
	Draw_2D(){ return; }
	~Draw_2D(){ return; }


	void static drawText(HDC hdc, TCHAR* msg, int x, int y, int RGB){
		TCHAR* message = { nullptr };
		int rgb[3];
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(rgb[0], rgb[1], rgb[2]));
		TextOut(hdc, x, y, msg, sizeof(msg));
	}

	void static drawText(HDC hdc, float data, int x, int y, int RGB){
		int rgb[3];
		TCHAR *message = new TCHAR(sizeof(data));// nullptr;
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
		SetTextColor(hdc, RGB(rgb[0], rgb[1], rgb[2]));
		SetBkMode(hdc, TRANSPARENT);
		wsprintf(message, L"%d", (int)data);
		TextOut(hdc, x, y, message, sizeof(message));
	}

	void static drawText(HDC hdc, int x, int y, int RGB, TCHAR* msg, ...)
	{
		va_list arg;
		va_start(arg, msg);
		TCHAR buf[500] = { NULL };
		vswprintf(buf, msg, arg);
		int count = 0;
		for (int i = 0; buf[i] != NULL; ++i)
			count++;
		int rgb[3];
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(rgb[0], rgb[1], rgb[2]));
		TextOut(hdc, x, y, (TCHAR*)buf, count);
		va_end(arg);
	}

	void static drawText(HDC hdc, int x, int y, COLORREF RGB, TCHAR* msg, ...)
	{
		va_list arg;
		va_start(arg, msg);
		TCHAR buf[500] = { NULL };
		vswprintf(buf, msg, arg);
		int count = 0;
		for (int i = 0; buf[i] != NULL; ++i)
			count++;
		int rgb[3];
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB);
		TextOut(hdc, x, y, (TCHAR*)buf, count);
		va_end(arg);
	}

	void static drawText(HDC hdc, TCHAR _message[], int data, int x, int y, int RGB, int size, TCHAR _font[]){
		TCHAR message[80];
		int rgb[3];
		HFONT hFont, OldFont;
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
		SetTextColor(hdc, RGB(rgb[0], rgb[1], rgb[2]));
		hFont = CreateFont(size, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, (_font));
		OldFont = (HFONT)SelectObject(hdc, hFont);
		SetBkMode(hdc, TRANSPARENT);
		wsprintf(message, L"%s %d", _message, data);
		TextOut(hdc, x, y, message, wcslen(message));
		DeleteObject(hFont);
		DeleteObject(OldFont);
	}

	void static drawText(HDC hdc, int x, int y, int RGB, int size, TCHAR _font[], TCHAR *_message, ...)
	{
		TCHAR message[80];
		int rgb[3];
		HFONT hFont, OldFont;
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
		SetTextColor(hdc, RGB(rgb[0], rgb[1], rgb[2]));
		hFont = CreateFont(size, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, (_font));
		OldFont = (HFONT)SelectObject(hdc, hFont);
		SetBkMode(hdc, TRANSPARENT);



		va_list arg;
		va_start(arg, _message);
		TCHAR buf[500] = { NULL };
		vswprintf(buf, _message, arg);
		int count = 0;
		for (int i = 0; buf[i] != NULL; ++i)
			count++;
		TextOut(hdc, x, y, buf, count);

		DeleteObject(hFont);
		DeleteObject(OldFont);
		va_end(arg);

	}

	void static drawText(HDC hdc, int x, int y, int RGB, int size, TCHAR *_message, ...)
	{
		int rgb[3];
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
		SetTextColor(hdc, RGB(rgb[0], rgb[1], rgb[2]));
		SetBkMode(hdc, TRANSPARENT);

		va_list arg;
		va_start(arg, _message);
		TCHAR buf[500] = { NULL };
		vswprintf(buf, _message, arg);
		int count = 0;
		for (int i = 0; buf[i] != NULL; ++i)
			count++;
		TextOut(hdc, x, y, buf, count);

		va_end(arg);

	}


	/*
	void static drawText(HDC hdc, std::string _message, int data, int x, int y, int RGB){
	char* message[80];
	int rgb[3];
	for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(rgb[0], rgb[1], rgb[2]));
	wsprintf(message, "%s %d", _message, data);
	TextOutA(hdc, x, y, message, wcslen(message));
	}

	void static drawText(HDC hdc, int data, int x, int y, int RGB){
	int rgb[3];
	TCHAR message[80];
	for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
	SetTextColor(hdc, RGB(rgb[0], rgb[1], rgb[2]));
	SetBkMode(hdc, TRANSPARENT);
	wsprintfW(message, L"%d", data);
	TextOut(hdc, x, y, message, wcslen(message));

	}

	void static drawText(HDC hdc, float data, int x, int y, int RGB){
	int rgb[3];
	TCHAR message[80];
	for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
	SetTextColor(hdc, RGB(rgb[0], rgb[1], rgb[2]));
	SetBkMode(hdc, TRANSPARENT);
	wsprintfW(message, L"%d", data);
	TextOut(hdc, x, y, message, wcslen(message));
	}

	*/


	void static drawText(HDC hdc, TCHAR message[], int x, int y, int RGB, int size, TCHAR _font[]){
		int rgb[3];
		HFONT hFont, OldFont;
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
		SetTextColor(hdc, RGB(rgb[0], rgb[1], rgb[2]));
		hFont = CreateFont(size, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, (_font));
		OldFont = (HFONT)SelectObject(hdc, hFont);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(rgb[0], rgb[1], rgb[2]));
		TextOut(hdc, x, y, message, wcslen(message));
		SelectObject(hdc, OldFont);
		DeleteObject(hFont);
		DeleteObject(OldFont);
	}






	/*
	void static drawText(HDC hdc, char _message[], int data, int x, int y, int RGB, int size, TCHAR _font[]){
	TCHAR message[80];
	int rgb[3];
	for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
	SetTextColor(hdc, RGB(rgb[0], rgb[1], rgb[2]));
	SetBkMode(hdc, TRANSPARENT);
	wsprintf(message, L"%s %d", _message, data);
	TextOut(hdc, x, y, message, wcslen(message));

	}

	void static drawText(HDC hdc, char _message[], float data, int x, int y, int RGB){
	TCHAR message[80];
	int rgb[3];
	for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
	SetTextColor(hdc, RGB(rgb[0], rgb[1], rgb[2]));
	SetBkMode(hdc, TRANSPARENT);
	wsprintf(message, L"%c %1.0f", _message, data);
	TextOut(hdc, x, y, message, wcslen(message));
	}
	*/

	void static drawLine(HDC hdc, int start_x, int start_y, int end_x, int end_y, int RGB){
		int rgb[3];
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
		HPEN MyPen, OldPen;
		MyPen = CreatePen(PS_SOLID, 3, RGB(rgb[0], rgb[1], rgb[2]));
		OldPen = (HPEN)SelectObject(hdc, MyPen);
		MoveToEx(hdc, start_x, start_y, nullptr);
		LineTo(hdc, end_x, end_y);
		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);
		DeleteObject(OldPen);

	}

	void static drawLine(HDC hdc, int start_x, int start_y, int end_x, int end_y){
		HPEN MyPen, OldPen;
		MyPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		OldPen = (HPEN)SelectObject(hdc, MyPen);
		MoveToEx(hdc, start_x, start_y, nullptr);
		LineTo(hdc, end_x, end_y);
		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);
		DeleteObject(OldPen);

	}

	void static drawLine(HDC hdc, int start_x, int start_y, int end_x, int end_y, int Linesize, int rgb[]){
		HPEN MyPen, OldPen;
		MyPen = CreatePen(PS_SOLID, Linesize, RGB(rgb[0], rgb[1], rgb[2]));
		OldPen = (HPEN)SelectObject(hdc, MyPen);
		MoveToEx(hdc, start_x, start_y, nullptr);
		LineTo(hdc, end_x, end_y);
		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);
		DeleteObject(OldPen);
	}




	///DrawRect
	void static drawRect(HDC hdc, float _l, float _t, float _w, float _h, int RGB, bool type)
	{
		int l = (int)_l;
		int t = (int)_t;
		int w = (int)_w;
		int h = (int)_h;


		HPEN MyPen, OldPen;
		int rgb[3];
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
		MyPen = CreatePen(PS_SOLID, 1, RGB(rgb[0], rgb[1], rgb[2]));
		OldPen = (HPEN)SelectObject(hdc, MyPen);

		if (type)
			Rectangle(hdc, l, t, l + w, t + h);

		else
		{
			MoveToEx(hdc, l, t, nullptr);
			LineTo(hdc, l + w, t);

			MoveToEx(hdc, l + w, t, nullptr);
			LineTo(hdc, l + w, t + h);


			MoveToEx(hdc, l + w, t + h, nullptr);
			LineTo(hdc, l, t + h);


			MoveToEx(hdc, l, t + h, nullptr);
			LineTo(hdc, l, t);

		}

		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);
		DeleteObject(OldPen);
		//Rectangle(hdc, (int)rect.left, (int)rect.top, (int)rect.left + (int)rect.width, (int)rect.top + (int)rect.height);
	}

	//Draw_2D
	void static drawRect(HDC hdc, float _l, float _t, float _w, float _h, int RGB, int PENRGB, int penSize, bool type){
		int l = (int)_l;
		int t = (int)_t;
		int w = (int)_w;
		int h = (int)_h;

		int rgb[3];
		int penRgb[3];
		for (int i = 0; i < 3; i++){
			rgb[i] = getRGB(RGB, i);
			penRgb[i] = getRGB(PENRGB, i);
		}
		HPEN MyPen, OldPen;
		MyPen = CreatePen(PS_SOLID, penSize, RGB(penRgb[0], penRgb[1], penRgb[2]));
		OldPen = (HPEN)SelectObject(hdc, MyPen);

		if (type == false){


			MoveToEx(hdc, l, t, nullptr);
			LineTo(hdc, l + w, t);

			MoveToEx(hdc, l + w, t, nullptr);
			LineTo(hdc, l + w, t + h);


			MoveToEx(hdc, l + w, t + h, nullptr);
			LineTo(hdc, l, t + h);


			MoveToEx(hdc, l, t + h, nullptr);
			LineTo(hdc, l, t);
		}
		else{
			HBRUSH hBrush, oldBrush;
			hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Rectangle(hdc, l, t, l + w, t + h);

			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			DeleteObject(oldBrush);
		}

		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);
		DeleteObject(OldPen);
		//Rectangle(hdc, (int)rect.left, (int)rect.top, (int)rect.left + (int)rect.width, (int)rect.top + (int)rect.height);
	}

	void static drawRect(HDC hdc, int x, int y, int size, int RGB){
		int rgb[3];
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);

		HPEN MyPen, OldPen;
		MyPen = CreatePen(PS_SOLID, 3, RGB(rgb[0], rgb[1], rgb[2]));
		OldPen = (HPEN)SelectObject(hdc, MyPen);

		MoveToEx(hdc, x, y, nullptr);
		LineTo(hdc, x + size, y);

		MoveToEx(hdc, x + size, y, nullptr);
		LineTo(hdc, x + size, y + size);


		MoveToEx(hdc, x + size, y + size, nullptr);
		LineTo(hdc, x, y + size);


		MoveToEx(hdc, x, y + size, nullptr);
		LineTo(hdc, x, y);

		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);
		DeleteObject(OldPen);
		//Rectangle(hdc, x, y, x + size, y + size);
	}

	void static drawRect(HDC hdc, int x, int y, int sizeX, int sizeY, int RGB){
		int rgb[3];
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
		HPEN MyPen, OldPen;
		MyPen = CreatePen(PS_SOLID, 1, RGB(rgb[0], rgb[1], rgb[2]));
		OldPen = (HPEN)SelectObject(hdc, MyPen);
		MoveToEx(hdc, x, y, nullptr);
		LineTo(hdc, x + sizeX, y);

		MoveToEx(hdc, x + sizeX, y, nullptr);
		LineTo(hdc, x + sizeX, y + sizeY);

		MoveToEx(hdc, x + sizeX, y + sizeY, nullptr);
		LineTo(hdc, x, y + sizeY);

		MoveToEx(hdc, x, y + sizeY, nullptr);
		LineTo(hdc, x, y);

		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);
		DeleteObject(OldPen);
		//Rectangle(hdc, (int)rect.left, (int)rect.top, (int)rect.left + (int)rect.width, (int)rect.top + (int)rect.height);
	}

	void static drawRect(HDC hdc, int x, int y, int sizeX, int sizeY, int RGB, bool type){
		int rgb[3];
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
		HPEN MyPen, OldPen;
		MyPen = CreatePen(PS_SOLID, 3, RGB(rgb[0], rgb[1], rgb[2]));
		OldPen = (HPEN)SelectObject(hdc, MyPen);

		if (type == false){
			MoveToEx(hdc, x, y, nullptr);
			LineTo(hdc, x + sizeX, y);

			MoveToEx(hdc, x + sizeX, y, nullptr);
			LineTo(hdc, x + sizeX, y + sizeY);

			MoveToEx(hdc, x + sizeX, y + sizeY, nullptr);
			LineTo(hdc, x, y + sizeY);

			MoveToEx(hdc, x, y + sizeY, nullptr);
			LineTo(hdc, x, y);
		}
		else{
			HBRUSH hBrush, oldBrush;
			hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, x, y, x + sizeX, y + sizeY);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			DeleteObject(oldBrush);
		}

		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);
		DeleteObject(OldPen);
		//Rectangle(hdc, (int)rect.left, (int)rect.top, (int)rect.left + (int)rect.width, (int)rect.top + (int)rect.height);
	}

	void static drawRect(HDC hdc, int x, int y, int sizeX, int sizeY, int RGB, int PENRGB, bool type)	{

		HPEN MyPen, OldPen;
		int rgb[3], penRgb[3];
		for (int i = 0; i < 3; i++){
			rgb[i] = getRGB(RGB, i);
			penRgb[i] = getRGB(PENRGB, i);
		}
		MyPen = CreatePen(PS_SOLID, 1, RGB(penRgb[0], penRgb[1], penRgb[2]));
		OldPen = (HPEN)SelectObject(hdc, MyPen);

		if (type == false){
			MoveToEx(hdc, x, y, nullptr);
			LineTo(hdc, x + sizeX, y);

			MoveToEx(hdc, x + sizeX, y, nullptr);
			LineTo(hdc, x + sizeX, y + sizeY);

			MoveToEx(hdc, x + sizeX, y + sizeY, nullptr);
			LineTo(hdc, x, y + sizeY);

			MoveToEx(hdc, x, y + sizeY, nullptr);
			LineTo(hdc, x, y);
		}
		else
		{
			HBRUSH hBrush, oldBrush;
			hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, x, y, x + sizeX, y + sizeY);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			DeleteObject(oldBrush);
		}

		SelectObject(hdc, OldPen);

		DeleteObject(MyPen);
		DeleteObject(OldPen);
		//Rectangle(hdc, (int)rect.left, (int)rect.top, (int)rect.left + (int)rect.width, (int)rect.top + (int)rect.height);
	}

	void static drawRect(HDC hdc, float _l, float _t, float _w, float _h, int RGB, int PENRGB, bool type = false)	{
		int l = (int)_l;
		int t = (int)_t;
		int w = (int)_w;
		int h = (int)_h;

		HPEN MyPen, OldPen;
		int rgb[3], penRgb[3];
		for (int i = 0; i < 3; i++){
			rgb[i] = getRGB(RGB, i);
			penRgb[i] = getRGB(PENRGB, i);
		}
		MyPen = CreatePen(PS_SOLID, 1, RGB(penRgb[0], penRgb[1], penRgb[2]));
		OldPen = (HPEN)SelectObject(hdc, MyPen);

		if (type == false)
		{

			MoveToEx(hdc, (int)l, (int)t, nullptr);
			LineTo(hdc, (int)(l + w), (int)t);

			MoveToEx(hdc, (int)(l + w), (int)t, nullptr);
			LineTo(hdc, (int)(l + w), (int)(t + h));


			MoveToEx(hdc, (int)(l + w), (int)(t + h), nullptr);
			LineTo(hdc, (int)l, (int)(t + h));


			MoveToEx(hdc, (int)l, (int)(t + h), nullptr);
			LineTo(hdc, (int)l, (int)t);
		}
		else{
			HBRUSH hBrush, oldBrush;
			hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Rectangle(hdc, (int)l, (int)t, (int)(l + w), (int)(t + h));

			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			DeleteObject(oldBrush);
		}

		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);
		DeleteObject(OldPen);
		//Rectangle(hdc, (int)rect.left, (int)rect.top, (int)rect.left + (int)rect.width, (int)rect.top + (int)rect.height);
	}

	void static drawProgrees(HDC hdc, int persent, float _l, float _t, float _w, float _h, int RGB, int PENRGB)
	{
		float per =static_cast<float>(_w * persent * 0.01f);
		if (per > _w)per = _w;
		Draw_2D::drawRect(hdc, _l, _t, _w, _h, WHITE, PENRGB, true);
		Draw_2D::drawRect(hdc, _l, _t, per, _h, RGB, PENRGB, true);
	}

	/*
	//DrawRect Center
	void static drawRectCenter(HDC hdc, int x, int y, int size, int RGB){
	int rgb[3];
	for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
	HPEN MyPen, OldPen;
	MyPen = CreatePen(PS_SOLID, 3, RGB(rgb[0], rgb[1], rgb[2]));
	OldPen = (HPEN)SelectObject(hdc, MyPen);

	MoveToEx(hdc, x - size, y - size, nullptr);
	LineTo(hdc, x - size + size * 2, y - size);

	MoveToEx(hdc, x - size + size * 2, y - size, nullptr);
	LineTo(hdc, x - size + size * 2, y - size + size * 2);


	MoveToEx(hdc, x - size + size * 2, y - size + size * 2, nullptr);
	LineTo(hdc, x - size, y - size + size * 2);


	MoveToEx(hdc, x - size, y - size + size * 2, nullptr);
	LineTo(hdc, x - size, y - size);

	SelectObject(hdc, OldPen);
	DeleteObject(MyPen);
	DeleteObject(OldPen);
	//Rectangle(hdc, x - size, y - size, x - size + size*2, y - size + size*2);
	}

	void static drawRectCenter(HDC hdc, float _l, float _t, float _w, float _h, int RGB, bool type){
	int rgb[3];
	for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
	HPEN MyPen, OldPen;
	MyPen = CreatePen(PS_SOLID, 1, RGB(rgb[0], rgb[1], rgb[2]));
	OldPen = (HPEN)SelectObject(hdc, MyPen);

	if (type == false){

	MoveToEx(hdc, l, t, nullptr);
	LineTo(hdc, l + w, t);

	MoveToEx(hdc, l + w, t, nullptr);
	LineTo(hdc, l + w, t + h);


	MoveToEx(hdc, l + w, t + h, nullptr);
	LineTo(hdc, l, t + h);


	MoveToEx(hdc, l, t + h, nullptr);
	LineTo(hdc, l, t);
	}
	else{
	HBRUSH hBrush, oldBrush;
	hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));

	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, l, t, l + w, t + h);

	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
	DeleteObject(oldBrush);
	}
	SelectObject(hdc, OldPen);
	DeleteObject(MyPen);
	DeleteObject(OldPen);
	//Rectangle(hdc, (int)rect.left, (int)rect.top, (int)rect.left + (int)rect.width, (int)rect.top + (int)rect.height);
	}

	void static drawRectCenter(HDC hdc, float _l, float _t, float _w, float _h, int RGB, int penSize, bool type){
	int rgb[3];
	for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);


	HPEN MyPen, OldPen;
	MyPen = CreatePen(PS_SOLID, penSize, RGB(rgb[0], rgb[1], rgb[2]));
	OldPen = (HPEN)SelectObject(hdc, MyPen);

	if (type == false){
	MoveToEx(hdc, l, t, nullptr);
	LineTo(hdc, l + w, t);

	MoveToEx(hdc, l + w, t, nullptr);
	LineTo(hdc, l + w, t + h);


	MoveToEx(hdc, l + w, t + h, nullptr);
	LineTo(hdc, l, t + h);


	MoveToEx(hdc, l, t + h, nullptr);
	LineTo(hdc, l, t);
	}
	else{
	HBRUSH hBrush, oldBrush;
	hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));

	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Rectangle(hdc, l, t, l + w, t + h);

	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
	DeleteObject(oldBrush);
	}

	SelectObject(hdc, OldPen);
	DeleteObject(MyPen);
	DeleteObject(OldPen);
	//Rectangle(hdc, (int)rect.left, (int)rect.top, (int)rect.left + (int)rect.width, (int)rect.top + (int)rect.height);
	}

	//DrawRect Round
	void static drawRectRoundCenter(HDC hdc, float _l, float _t, float _w, float _h, int RGB, int PENRGB, int round){
	int rgb[3];
	int penRgb[3];
	for (int i = 0; i < 3; i++)
	{
	rgb[i] = getRGB(RGB, i);
	penRgb[i] = getRGB(PENRGB, i);
	}
	HPEN MyPen, OldPen;
	MyPen = CreatePen(PS_SOLID, 1, RGB(penRgb[0], penRgb[1], penRgb[2]));
	OldPen = (HPEN)SelectObject(hdc, MyPen);

	HBRUSH hBrush, oldBrush;
	hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	RoundRect(hdc, l + w/2, t + h/2, l + w, l + h, round, round );
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
	DeleteObject(oldBrush);

	SelectObject(hdc, OldPen);

	DeleteObject(MyPen);
	DeleteObject(OldPen);

	DeleteObject(hBrush);
	DeleteObject(oldBrush);
	//Rectangle(hdc, (int)rect.left, (int)rect.top, (int)rect.left + (int)rect.width, (int)rect.top + (int)rect.height);
	}
	*/


	void static drawRectRound(HDC hdc, float _l, float _t, float _w, float _h, int RGB, int PENRGB, int round){
		int l = (int)_l;
		int t = (int)_t;
		int w = (int)_w;
		int h = (int)_h;

		int rgb[3];
		int penRgb[3];
		for (int i = 0; i < 3; i++)
		{
			rgb[i] = getRGB(RGB, i);
			penRgb[i] = getRGB(PENRGB, i);
		}
		HPEN MyPen, OldPen;
		MyPen = CreatePen(PS_SOLID, 1, RGB(penRgb[0], penRgb[1], penRgb[2]));
		OldPen = (HPEN)SelectObject(hdc, MyPen);

		HBRUSH hBrush, oldBrush;
		hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		RoundRect(hdc, (int)l, (int)t, (int)(l + w), (int)(t + h), round, round);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		DeleteObject(oldBrush);

		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);
		DeleteObject(OldPen);
		//Rectangle(hdc, (int)rect.left, (int)rect.top, (int)rect.left + (int)rect.width, (int)rect.top + (int)rect.height);
	}

	void static drawRectRoundAlpha(HDC hdc, float _l, float _t, float _w, float _h, int RGB, int PENRGB, int alpha, int round){
		int l = (int)_l;
		int t = (int)_t;
		int w = (int)_w;
		int h = (int)_h;

		int rgb[3], penRgb[3];
		for (int i = 0; i < 3; i++)
		{
			rgb[i] = getRGB(RGB, i);
			penRgb[i] = getRGB(PENRGB, i);
		}
		HDC LayDC;
		HBITMAP Lay;
		BLENDFUNCTION bf;

		HPEN MyPen, OldPen;
		HBRUSH hBrush, oldBrush;

		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.AlphaFormat = 0;
		bf.SourceConstantAlpha = alpha;

		Lay = CreateCompatibleBitmap(hdc, (int)w, (int)h);
		LayDC = CreateCompatibleDC(hdc);
		SelectObject(LayDC, Lay);
		BitBlt(LayDC, 0, 0, (int)w, (int)h, hdc, (int)l, (int)t, SRCCOPY);

		MyPen = CreatePen(PS_SOLID, 1, RGB(penRgb[0], penRgb[1], penRgb[2]));
		OldPen = (HPEN)SelectObject(LayDC, MyPen);

		hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
		oldBrush = (HBRUSH)SelectObject(LayDC, hBrush);

		RoundRect(LayDC, 0, 0, (int)w, (int)h, round, round);
		SelectObject(LayDC, oldBrush);
		DeleteObject(hBrush);
		DeleteObject(oldBrush);

		SelectObject(LayDC, OldPen);
		DeleteObject(MyPen);
		DeleteObject(OldPen);

		AlphaBlend(hdc, (int)l, (int)t, (int)w, (int)h, LayDC, 0, 0, (int)w, (int)h, bf);


		DeleteDC(LayDC);
		DeleteObject(Lay);


		//Rectangle(hdc, (int)rect.left, (int)rect.top, (int)rect.left + (int)rect.width, (int)rect.top + (int)rect.height);
	}

	//

	void static drawCicle(HDC hdc, int cx, int cy, int size, int RGB, int PENRGB, int Linesize){
		HBRUSH hBrush, oldBrush;
		HPEN hPen, oldPen;
		int rgb[3];
		int penRgb[3];
		for (int i = 0; i < 3; i++){
			rgb[i] = getRGB(RGB, i);
			penRgb[i] = getRGB(PENRGB, i);
		}
		hPen = CreatePen(PS_DOT, Linesize, RGB(penRgb[0], penRgb[1], penRgb[2]));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, cx - size / 2, cy - size / 2, cx + size / 2, cy + size / 2);
		SelectObject(hdc, oldBrush);
		SelectObject(hdc, oldPen);
		DeleteObject(hBrush);
		DeleteObject(oldBrush);
		DeleteObject(hPen);
		DeleteObject(oldPen);
	}

	void static drawCicle(HDC hdc, int start_x, int start_y, int end_x, int end_y, int RGB){
		HBRUSH hBrush, oldBrush;
		int rgb[3];
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);
		hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, start_x, start_y, end_x, end_y);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		DeleteObject(oldBrush);
	}

	void static drawCicle(HDC hdc, float _l, float _t, float _w, float _h, int RGB, int PENRGB, int Linesize){
		int l = (int)_l;
		int t = (int)_t;
		int w = (int)_w;
		int h = (int)_h;

		HBRUSH hBrush, oldBrush;
		HPEN hPen, oldPen;
		int rgb[3];
		int penRgb[3];
		for (int i = 0; i < 3; i++){
			rgb[i] = getRGB(RGB, i);
			penRgb[i] = getRGB(PENRGB, i);
		}

		hPen = CreatePen(PS_DOT, Linesize, RGB(penRgb[0], penRgb[1], penRgb[2]));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, (int)l, (int)t, (int)(l + w), (int)(t + h));
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		DeleteObject(oldBrush);
		DeleteObject(hPen);
		DeleteObject(oldPen);
	}

	void static drawCircleAlpha(HDC hdc, float _l, float _t, float _w, float _h, int RGB, int PENRGB, int alpha){
		int l = (int)_l;
		int t = (int)_t;
		int w = (int)_w;
		int h = (int)_h;

		int rgb[3], penRgb[3];
		for (int i = 0; i < 3; i++)
		{
			rgb[i] = getRGB(RGB, i);
			penRgb[i] = getRGB(PENRGB, i);
		}
		HDC LayDC;
		HBITMAP Lay;
		BLENDFUNCTION bf;

		HPEN MyPen, OldPen;
		HBRUSH hBrush, oldBrush;

		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.AlphaFormat = 0;
		bf.SourceConstantAlpha = alpha;

		Lay = CreateCompatibleBitmap(hdc, (int)w, (int)h);
		LayDC = CreateCompatibleDC(hdc);
		SelectObject(LayDC, Lay);
		BitBlt(LayDC, 0, 0, (int)w, (int)h, hdc, (int)l, (int)t, SRCCOPY);

		MyPen = CreatePen(PS_SOLID, 1, RGB(penRgb[0], penRgb[1], penRgb[2]));
		OldPen = (HPEN)SelectObject(LayDC, MyPen);

		hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
		oldBrush = (HBRUSH)SelectObject(LayDC, hBrush);

		Ellipse(LayDC, 0, 0, (int)w, (int)h);
		SelectObject(LayDC, oldBrush);
		DeleteObject(hBrush);
		DeleteObject(oldBrush);

		SelectObject(LayDC, OldPen);
		DeleteObject(MyPen);
		DeleteObject(OldPen);

		AlphaBlend(hdc, (int)l, (int)t, (int)w, (int)h, LayDC, 0, 0, (int)w, (int)h, bf);


		DeleteDC(LayDC);
		DeleteObject(Lay);


		//Rectangle(hdc, (int)rect.left, (int)rect.top, (int)rect.left + (int)rect.width, (int)rect.top + (int)rect.height);
	}


	//DrawTrangle

	void static drawPentagon(HDC hdc, int cx, int cy, int size, int RGB){
		HPEN MyPen, OldPen;
		int rgb[3];
		for (int i = 0; i < 3; i++)rgb[i] = getRGB(RGB, i);

		MyPen = CreatePen(PS_SOLID, 3, RGB(rgb[0], rgb[1], rgb[2]));
		OldPen = (HPEN)SelectObject(hdc, MyPen);

		MoveToEx(hdc, cx, cy - size, nullptr);
		LineTo(hdc, cx + size, cy);

		MoveToEx(hdc, cx + size, cy, nullptr);
		LineTo(hdc, cx + size, cy + size);

		MoveToEx(hdc, cx + size, cy + size, nullptr);
		LineTo(hdc, cx - size, cy + size);

		MoveToEx(hdc, cx - size, cy + size, nullptr);
		LineTo(hdc, cx - size, cy);

		MoveToEx(hdc, cx - size, cy, nullptr);
		LineTo(hdc, cx, cy - size);

		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);
		DeleteObject(OldPen);
	}



	void static ytRand_rgb_bri(int rgb[]){
		int imsi = 1;// (rand() % 255)* 0.1;
		rgb[0] = (int)((rand() % 255) *1.5);
		rgb[1] = (int)((rand() % 255) *1.2);
		rgb[2] = (int)((rand() % 255) *0.9);
	}

	int static random(int start, int end){
		if (end <= start) {
			return (rand() % (start - end) + start);
		}
		return (rand() % (end - start) + start);
	}


	int  static randomDir(int start, int end){
		int dir;
		if (rand() % 2 == 0)
			dir = 1;
		else dir = -1;

		if (end <= start) {
			return (rand() % (start - end) + start) * dir;
		}
		return (rand() % (end - start) + start) * dir;
	}

	static int getRGB(int RGB, int num){
		int rgb[3] = { 0 };
		if (RGB == Color_2D::RANDOM_2D)
			RGB = rand() % (Color_2D::WHITE_2D - 1);
		switch (RGB){
		case Color_2D::RED_2D:
			rgb[0] = 255; rgb[1] = 0; rgb[2] = 0;
			break;
		case Color_2D::BLUE_2D:
			rgb[0] = 0; rgb[1] = 0; rgb[2] = 255;
			break;
		case Color_2D::GREEN_2D:
			rgb[0] = 0; rgb[1] = 255; rgb[2] = 0;
			break;
		case Color_2D::YELLOW_2D:
			rgb[0] = 255; rgb[1] = 255; rgb[2] = 0;
			break;
		case Color_2D::WHITE_2D:
			rgb[0] = 255; rgb[1] = 255; rgb[2] = 255;
			break;
		case Color_2D::BLACK_2D:
			rgb[0] = 0; rgb[1] = 0; rgb[2] = 0;
			break;
		case Color_2D::BRIGHT_BLACK_2D:
			rgb[0] = 130; rgb[1] = 130, rgb[2] = 130;
			break;
		case Color_2D::CYAN_2D:
			rgb[0] = 255; rgb[1] = 0; rgb[2] = 255;
			break;
		case Color_2D::MAGENTA_2D:
			rgb[0] = 255; rgb[1] = 0; rgb[2] = 255;
			break;
		case Color_2D::CHANGE_2D:
			ytRand_rgb_bri(rgb);
			break;
		case Color_2D::ORANGE_2D:
			rgb[0] = 230; rgb[1] = 130; rgb[2] = 5;
			break;
		case Color_2D::BRUSH_2D:
			rgb[0] = 248; rgb[1] = 248; rgb[2] = 248;
			break;
		case Color_2D::BRUSH2_2D:
			rgb[0] = 240; rgb[1] = 240; rgb[2] = 240;
			break;


		case Color_2D::BRIGHT_RED_1_2D:
			rgb[0] = 238; rgb[1] = 20; rgb[2] = 45;
			break;
		case Color_2D::BRIGHT_RED_2_2D:
			rgb[0] = 229; rgb[1] = 52; rgb[2] = 165;
			break;
		case Color_2D::BRIGHT_RED_3_2D:
			rgb[0] = 231; rgb[1] = 15; rgb[2] = 165;
			break;
		case Color_2D::BRIGHT_RED_4_2D:
			rgb[0] = 246; rgb[1] = 122; rgb[2] = 218;
			break;
		case Color_2D::BRIGHT_RED_5_2D:
			rgb[0] = 238; rgb[1] = 5; rgb[2] = 33;
			break;
		case Color_2D::BRIGHT_RED_6_2D:
			rgb[0] = 243; rgb[1] = 258; rgb[2] = 60;


		case Color_2D::BRIGHT_YELLOW_1_2D:
			rgb[0] = 235; rgb[1] = 249; rgb[2] = 81;
			break;
		case Color_2D::BRIGHT_YELLOW_2_2D:
			rgb[0] = 244; rgb[1] = 229; rgb[2] = 109;
			break;
		case Color_2D::BRIGHT_YELLOW_3_2D:
			rgb[0] = 244; rgb[1] = 213; rgb[2] = 28;
			break;
		case Color_2D::BRIGHT_YELLOW_4_2D:
			rgb[0] = 253; rgb[1] = 218; rgb[2] = 76;
			break;
		case Color_2D::BRIGHT_YELLOW_5_2D:
			rgb[0] = 228; rgb[1] = 234; rgb[2] = 92;
			break;
		case Color_2D::BRIGHT_YELLOW_6_2D:
			rgb[0] = 234; rgb[1] = 228; rgb[2] = 106;


		case Color_2D::BRIGHT_GREEN_1_2D:
			rgb[0] = 115; rgb[1] = 225; rgb[2] = 121;
			break;
		case Color_2D::BRIGHT_GREEN_2_2D:
			rgb[0] = 102; rgb[1] = 250; rgb[2] = 82;
			break;
		case Color_2D::BRIGHT_GREEN_3_2D:
			rgb[0] = 177; rgb[1] = 244; rgb[2] = 83;
			break;
		case Color_2D::BRIGHT_GREEN_4_2D:
			rgb[0] = 117; rgb[1] = 244; rgb[2] = 83;
			break;
		case Color_2D::BRIGHT_GREEN_5_2D:
			rgb[0] = 111; rgb[1] = 226; rgb[2] = 120;
			break;
		case Color_2D::BRIGHT_GREEN_6_2D:
			rgb[0] = 111; rgb[1] = 226; rgb[2] = 120;
			break;
		case Color_2D::BRIGHT_GREEN_7_2D:
			rgb[0] = 90; rgb[1] = 254; rgb[2] = 108;
			break;
		case Color_2D::BRIGHT_GREEN_8_2D:
			rgb[0] = 111; rgb[1] = 260; rgb[2] = 120;
			break;
		case Color_2D::BRIGHT_GREEN_9_2D:
			rgb[0] = 163; rgb[1] = 255; rgb[2] = 118;
			break;

		case Color_2D::BRIGHT_ORANGE_1_2D:
			rgb[0] = 241; rgb[1] = 132; rgb[2] = 18;
			break;
		case Color_2D::BRIGHT_ORANGE_2_2D:
			rgb[0] = 244; rgb[1] = 129; rgb[2] = 83;
			break;

		case Color_2D::BRIGHT_BLUE_1_2D:
			rgb[0] = 11; rgb[1] = 201; rgb[2] = 216;
			break;
		case Color_2D::BRIGHT_BLUE_2_2D:
			rgb[0] = 77; rgb[1] = 94; rgb[2] = 209;
			break;
		case Color_2D::BRIGHT_BLUE_3_2D:
			rgb[0] = 30; rgb[1] = 168; rgb[2] = 194;
			break;
		case Color_2D::BRIGHT_BLUE_4_2D:
			rgb[0] = 84; rgb[1] = 216; rgb[2] = 177;
			break;

		case Color_2D::REMOVE_COLOR_2D:
			rgb[0] = 255; rgb[1] = 0; rgb[2] = 255;
			break;

		case Color_2D::RANDOBJECT_COLOR_2D:
			rgb[0] = 0; rgb[1] = 162; rgb[2] = 232;
			break;

		case Color_2D::REMOVE_STONEBALL_2D:
			rgb[0] = 239; rgb[1] = 255; rgb[2] = 255;
			break;

		case Color_2D::REMOVE_STARPIX_2D:
			rgb[0] = 239; rgb[1] = 255; rgb[2] = 255;
			break;

		case Color_2D::REMOVE_SOLARPIX_2D:
			rgb[0] = 207; rgb[1] = 255; rgb[2] = 207;
			break;

		case Color_2D::REMOVE_RUNAPIX_2D:
			rgb[0] = 255; rgb[1] = 239; rgb[2] = 255;
			break;


		case Color_2D::REMOVE_GHOSTPIX_2D:
			rgb[0] = 239; rgb[1] = 255; rgb[2] = 255;
			break;
		case Color_2D::REMOVE_BUTTON_2D:
			rgb[0] = 51; rgb[1] = 102; rgb[2] = 221;
			break;

		case Color_2D::REMOVE_BUTTON2_2D:
			rgb[0] = 51; rgb[1] = 102; rgb[2] = 221;
			break;;
		}
#pragma endregion
		return rgb[num];
	}

};

