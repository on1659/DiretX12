#pragma once

#include "stdafx.h"

//KYT '16.05.10
/*
 Edit Box
*/

class CEditBox_2D : public CSingleTonBase<CEditBox_2D>
{
private:
	struct EditBoxStruct
{
	BoundingBox_2D boundingbox;
	std::wstring	msg = L"";
	int			   color;
	int			  penColor;
	int				alpha = 255;
	int				round = 0;
	
};


private:
	//EditBox 정보
	std::vector<EditBoxStruct>	m_vEditBox;

	//Msg 위치
	int							m_nMsgPivot;

	HWND						m_hWnd;

	bool						m_bStart;

	//JJY '16.05.10 plus
	/*
		에디트 박스에 사용될 폰트
	*/
	//-----------------------
	HFONT m_font, m_oldFont;


public:
	CEditBox_2D();
	
	~CEditBox_2D();
	
	//캐럿의 위치를 변경해야 하기 때문에 hwnd가 필수
	void Load(HWND hwnd);

	/*
		위치를 정해줌. 캐럿의 사이즈가 고정이라서 박스의 크기를 조절할 수 없다.
		cx, cy, width, 배경색, 테두리색, 알파값, 둥글게 할 정도를 조절한다.
	*/
	void PushEditBoxCenterPosition(int cx, int cy, int width, std::wstring wstr = L"", int backColor = Color_2D::WHITE_2D,int penColor = Color_2D::BLACK_2D, int alpha = 255, int round = 0);

	void PushEditBoxLTRD(int l, int t, int r, std::wstring wstr = L"", int backColor = Color_2D::WHITE_2D, int penColor = Color_2D::BLACK_2D, int alpha = 255, int round = 0);
	
	void OnDraw(HDC hdc);

	void Input(WPARAM wParam);

	//다음 씬 전환시 꼭 클리어!
	void Clear() { HideCaret(m_hWnd); m_vEditBox.clear(); m_bStart = false; }

	bool IsStart() const { return m_bStart; }

	//Text 얻어옴
	std::string GetText(int nReadboxNumber = -1) const
	{ 
		if (nReadboxNumber == -1) nReadboxNumber = m_nMsgPivot;
		std::string str;
		str.assign(m_vEditBox[nReadboxNumber].msg.begin(), m_vEditBox[nReadboxNumber].msg.end());
		return str;
	}

};

