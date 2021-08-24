#include "stdafx.h"
#include "EditBox_2D.h"

//JJY '16.05.10 plus
/*
	����Ʈ �ڽ����� ����
	���� ������� �����ٶ�
*/
#define EDITBOX_HEIGHT 50

CEditBox_2D::CEditBox_2D()
{
	m_vEditBox.clear();
	m_nMsgPivot = 0;
}


CEditBox_2D::~CEditBox_2D()
{
	HideCaret(m_hWnd);
	DestroyCaret();
}

void CEditBox_2D::Load(HWND hwnd)
{
	m_hWnd = hwnd;
	CreateCaret(m_hWnd, NULL, 3, EDITBOX_HEIGHT);
	ShowCaret(m_hWnd);

	//Start
	m_bStart = false;

	//JJY '16.05.10 plus
	/*
		����Ʈ �ڽ��� ���� ��Ʈ ����
	*/
	//-----------------------
	m_font = CreateFont(EDITBOX_HEIGHT, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, L"����");
}

void CEditBox_2D::PushEditBoxCenterPosition(int cx, int cy, int width, std::wstring wstr, int color, int penColor, int alpha, int round)
{
	ShowCaret(m_hWnd);
	EditBoxStruct editbox;

	editbox.boundingbox.SetBoxCenter(cx, cy, width, EDITBOX_HEIGHT);
	editbox.color = color;
	editbox.msg = wstr;
	editbox.penColor = penColor;
	editbox.alpha = alpha;
	editbox.round = round;

	m_vEditBox.push_back(editbox);
	if (m_vEditBox.size() == 1) SetCaretPos(cx - width, cy - EDITBOX_HEIGHT / 2 + 3);
}

void CEditBox_2D::PushEditBoxLTRD(int l, int t, int r, std::wstring wstr, int color, int penColor, int alpha, int round)
{
	ShowCaret(m_hWnd);
	EditBoxStruct editbox;

	int b = t + 20;
	BoundingBox_2D boundingbox;
	editbox.boundingbox.SetBoxLTRD(l ,t, r, b);
	editbox.msg = wstr;
	editbox.color = color;
	editbox.penColor = penColor;
	editbox.alpha = alpha;
	editbox.round = round;

	m_vEditBox.push_back(editbox);
	if (m_vEditBox.size() == 1) SetCaretPos(l, t +3);
}

void CEditBox_2D::OnDraw(HDC hdc)
{
	SIZE string_size;

	for (auto &editbox : m_vEditBox)
	{
		Draw_2D::drawRectRoundAlpha(hdc, editbox.boundingbox.left, editbox.boundingbox.top, editbox.boundingbox.w, editbox.boundingbox.h,
			editbox.color, editbox.penColor, editbox.alpha, editbox.round);

		TextOut(hdc, editbox.boundingbox.left, editbox.boundingbox.top, editbox.msg.c_str(), editbox.msg.size());
	}

	#ifndef _DEBUG
		//JJY '16.05.10 plus
		/*
			����Ʈ �ڽ��� ���� ��Ʈ
		*/
		m_oldFont = (HFONT)SelectObject(hdc, m_font);
	
		//JJY '16.05.11 plus
		/*
			���ڿ��� ���� �о�ͼ� caret�� ��ġ�� ����
			��Ŀ�̵Ǵ� ����Ʈ �ڽ��� ���� ĳ�� ��ġ ����
		*/
		GetTextExtentPoint(hdc, m_vEditBox[m_nMsgPivot].msg.c_str(), m_vEditBox[m_nMsgPivot].msg.size(), &string_size);
	
		//int iCaretPosLeft = 1 + m_vEditBox[m_nMsgPivot].boundingbox.left + (10 * m_vEditBox[m_nMsgPivot].msg.size());	
		auto iCaretPosLeft = 1.0 + static_cast<long>(m_vEditBox[m_nMsgPivot].boundingbox.left) + string_size.cx;
		auto iCaretPosY = m_vEditBox[m_nMsgPivot].boundingbox.cy - (EDITBOX_HEIGHT / 2);
		SetCaretPos(iCaretPosLeft, iCaretPosY);
	#endif
}

void CEditBox_2D::Input(WPARAM wParam)
{
	if (m_vEditBox.size() <= 0)return;
	
	if (wParam == VK_BACK)
	{
		Loop:
		if(false == m_vEditBox[m_nMsgPivot].msg.empty())
			m_vEditBox[m_nMsgPivot].msg.pop_back();

		else if (m_nMsgPivot-- > 0)
			goto Loop;

		if (m_nMsgPivot < 0) m_nMsgPivot = 0;
	}
	else if (wParam == VK_RETURN)
	{
		//KYT '16.05.12
		/*
			���� Ű�� ������ GameStartFunc �Լ��� �����
		*/
		m_bStart = true;
	}
	else if (wParam == VK_TAB)
	{
		m_nMsgPivot++;


		//JJY '16.05.11 ����
		/*
			�� Ű �Է� ��, ����Ʈ�ڽ� ���� �ʰ��� 0��° ����Ʈ �ڽ��� ���°ɷ� ����
		*/
		//if (m_nMsgPivot >= m_vEditBox.size())
		//	m_nMsgPivot--;
		if (m_nMsgPivot >= m_vEditBox.size())
			m_nMsgPivot = 0;
	}

	else
		m_vEditBox[m_nMsgPivot].msg += wParam;

	//if ('a' <= wParam && wParam <= 'z' )
	//	m_vEditBox[m_nMsgPivot].msg += wParam;
	//
	//else if('A'<=wParam && wParam <= 'Z') 
	//	m_vEditBox[m_nMsgPivot].msg += wParam;
	//
	//else if(48 <= wParam && wParam <= 57)
	//	m_vEditBox[m_nMsgPivot].msg += wParam;

	if (6 * m_vEditBox[m_nMsgPivot].msg.size() > m_vEditBox[m_nMsgPivot].boundingbox.w)
		m_vEditBox[m_nMsgPivot].msg.pop_back();
}