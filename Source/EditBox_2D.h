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
	//EditBox ����
	std::vector<EditBoxStruct>	m_vEditBox;

	//Msg ��ġ
	int							m_nMsgPivot;

	HWND						m_hWnd;

	bool						m_bStart;

	//JJY '16.05.10 plus
	/*
		����Ʈ �ڽ��� ���� ��Ʈ
	*/
	//-----------------------
	HFONT m_font, m_oldFont;


public:
	CEditBox_2D();
	
	~CEditBox_2D();
	
	//ĳ���� ��ġ�� �����ؾ� �ϱ� ������ hwnd�� �ʼ�
	void Load(HWND hwnd);

	/*
		��ġ�� ������. ĳ���� ����� �����̶� �ڽ��� ũ�⸦ ������ �� ����.
		cx, cy, width, ����, �׵θ���, ���İ�, �ձ۰� �� ������ �����Ѵ�.
	*/
	void PushEditBoxCenterPosition(int cx, int cy, int width, std::wstring wstr = L"", int backColor = Color_2D::WHITE_2D,int penColor = Color_2D::BLACK_2D, int alpha = 255, int round = 0);

	void PushEditBoxLTRD(int l, int t, int r, std::wstring wstr = L"", int backColor = Color_2D::WHITE_2D, int penColor = Color_2D::BLACK_2D, int alpha = 255, int round = 0);
	
	void OnDraw(HDC hdc);

	void Input(WPARAM wParam);

	//���� �� ��ȯ�� �� Ŭ����!
	void Clear() { HideCaret(m_hWnd); m_vEditBox.clear(); m_bStart = false; }

	bool IsStart() const { return m_bStart; }

	//Text ����
	std::string GetText(int nReadboxNumber = -1) const
	{ 
		if (nReadboxNumber == -1) nReadboxNumber = m_nMsgPivot;
		std::string str;
		str.assign(m_vEditBox[nReadboxNumber].msg.begin(), m_vEditBox[nReadboxNumber].msg.end());
		return str;
	}

};

