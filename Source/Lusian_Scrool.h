#pragma once

//	����Ͻ� ��ũ�� Ŭ������ �����ϼ���.
#define Lu_Scroll	scroll_Win32

//---------------------------------------------------------------
//	basic_scroll�� ����� �ڽ�Ŭ������ �̿��ؼ� ���α׷����ϼ���.
//---------------------------------------------------------------

#pragma region	 default setting part

#pragma endregion

/*
* ��ũ���� ����ϱ� ���� ���Ŭ�����Դϴ�.
* ��ũ�ѿ� ���� �⺻���� �ڵ尡 �����˴ϴ�.
* ����, ���� ��ũ�� ����� �����ؼ� ����� �� �ֽ��ϴ�.
* �ʱ�ȭ(initialize)�� �� �� ����ؾ� �մϴ�.
* �� Ŭ������ �̿��� ��ũ�� Ŭ������ ���콺 Ŭ��, �׸��� �Լ��� �����ؾ� �մϴ�.
*/
class basic_scroll{

public:

	/*
	* ��ũ���� Ÿ���Դϴ�.
	* SCR_H : ����������� �����Ǵ� ��ũ���Դϴ�.
	* SCR_V : ������������ �����Ǵ� ��ũ���Դϴ�.
	*/
	typedef enum{ SCR_H, SCR_V }SCRType;

protected:
	//	���� ��ũ���� Ÿ���Դϴ�.
	SCRType	m_scType;

	//	���� ��ũ���� �����Դϴ�.
	float	m_flScroll;
	//	���� ��ũ�ѿ��� ������ �� �ִ� ������ �����Դϴ�.
	float	m_flThumb;
	//	���� ��ũ�ѿ��� ������ �� �ִ� ������ �߽��� ��ġ�Դϴ�.
	float	m_fnowScrollpos;

	//	��ũ���� ��� �������� �����Դϴ�.
	float	m_flSrc;
	//	��� �����Ͱ� �������� ȭ���� �����Դϴ�.
	float	m_flSrcView;

	//	��ũ���� �ʺ��Դϴ�.
	float	m_flThickness;

	//	��ũ���� ���� ������ ��ġ�Դϴ�.
	float	m_fLastGrapPosition;
	bool	m_bGrap;

	//	��ũ���� �ٱ����� ��ġ�� �ʵ��� �ϴ� �Լ��Դϴ�.
	float Lus_onlyuse_SideStore(float min, float value, float max){ if (min > value) value = min;if(value > max) value = max; return value; }
public:

	basic_scroll()
	{
		m_bGrap = false;
		m_fLastGrapPosition = 0.f;
		m_fnowScrollpos = 0;
	}

	/*
	*	�ʱ�ȭ �Լ��Դϴ�.
	*	SCRType : ��ũ�� Ÿ���Դϴ�. scroll::SCRType::SCR_H �� ���� ������� �ۼ��˴ϴ�.
	*	ScrLength : ��ũ���� ���� ������ �����Դϴ�.
	*	ScrViewLength : ȭ�鿡 ���̴� ������ �����Դϴ�.
	*	ScrollLength : ��ũ�ѹ��� �����Դϴ�.
	*	ThumbLength( = 0.f) : ��ũ�� ���� �̵���ų �� �ִ� ���� ����(1/2)�Դϴ�. 0�� �Է��ϸ� �ڵ����� ����մϴ�.
			�� ThumbLength�� ���̸� �ڵ����� ������ �� �ִ� ���� : ScrollLength > ThumbLength * 2
	*	thickness : ��ũ���� �����Դϴ�.
	*/
	virtual void initialize(SCRType type, float ScrLength, float ScrViewLength, float ScrollLength,
		float ThumbLength = 0.f, float thickness = 5.0f)
	{
		m_bGrap = false;
		m_fLastGrapPosition = 0.f;
		m_fnowScrollpos = 0;

		m_scType = type;

		m_flSrc = ScrLength;
		m_flSrcView = ScrViewLength;
		if (ThumbLength != 0)
			m_flThumb = ThumbLength;
		else
			m_flThumb = max(5, ScrollLength * m_flSrcView / m_flSrc);

		m_flScroll = ScrollLength - 2 * m_flThumb;

		m_flThickness = thickness;
	}

	/*
	* ���콺�� Thumb(��ũ�� ���� �̵���ų �� �ִ� ����) ������ ������ �� ȣ���մϴ�. ���� ���콺�� ���� ��ġ�� ����մϴ�.
	* ����� ������ ���ÿ� �������� �����Ƿ� ȣ��ÿ� ����� ��ǥ���� �����ؾ� �մϴ�.
	* �ش� �Լ��� �������̵��� �����մϴ�.
	*/
	virtual void SetGrip(float pos)	{ m_fLastGrapPosition = pos; m_bGrap = true; }

	//	���콺�� ������ �� ȣ���մϴ�.
	void ReleaseGrip(){ m_bGrap = false; }

	/*
	* SetGrip�� ȣ���ϰ� ���콺�� �巡���� �� ȣ���մϴ�.
	* Thumb�� ��ġ�� ���콺�� �̵��� ��ŭ �����մϴ�. �� ��ġ�� ��ũ�� �ٸ� Ƣ��� �� �����ϴ�.
	* ���� SetGrip�� ȣ������ �ʾ��� ��� false���� ��ȯ�ϸ�, �⺻������ true�� ��ȯ�մϴ�.
	*/
	virtual bool Drag(float pos)
	{
		if (!m_bGrap) return false;
		float moveLen = pos - m_fLastGrapPosition;
		m_fnowScrollpos = Lus_onlyuse_SideStore(0, m_fnowScrollpos + moveLen, m_flScroll);
		m_fLastGrapPosition = pos;
		return true;
	}

	//	Thumb�� ��ġ�� ���Ƿ� �����մϴ�. Thumb�� �̿����� �ʰ� ��ũ�� �ٸ� ������ �� ����մϴ�.
	void SetnowScrollPosition(float len)
	{
		m_fLastGrapPosition = 0;
		m_bGrap = false;
		m_fnowScrollpos = Lus_onlyuse_SideStore(0, m_fnowScrollpos + len, m_flScroll);
	}

	//	���� ��ũ���� ����Ű�� ���� �������� ���̸� ��ȯ�մϴ�.
	virtual float GetNowSrcPosition(){ return m_flSrc * m_fnowScrollpos / m_flScroll; }

};

//---------------------------------------------------------------

//	�� ���� ���ο� �ʿ��� ����� �����մϴ�.
#pragma region	 user setting part

#pragma comment(lib,"msimg32")
#include <windows.h>

#pragma endregion

/*
* basic_scroll�� ��ӹ��� Win32API���� ����� ������ scroll�Դϴ�.
* OnProcessingMouseMessage�� �̿��Ͽ� ��ũ���� �̿��� �� �ʿ��� ���콺�Է��� ������ ����� �� �ֵ��� �߽��ϴ�.
* ��ũ���� ����� ������ initialize�� �� �� �� ���� OnProcessingMouseMessage�� ȣ�����ְ�, WM_PAINT���� Draw �Լ��� ȣ���� �ִ� ������ ����� �� �ֽ��ϴ�.
* ��ũ���� �̿��Ͽ� ������ �����ʹ� GetNowSrcPosition�� �̿��� ���� ���� �� �ֽ��ϴ�.
* �Ʒ� Ŭ������ �����Ͽ� �ٸ� �׷��ȶ��̺귯������ �����ϴ� ��ũ�� Ŭ������ ���� �� �ֽ��ϴ�.
*/
class scroll_Win32 : public basic_scroll{

protected:
	//	��ũ���� �����Դϴ�.
	int		m_iOpacity;
	//	��ũ�� �����Դϴ�. �� ������� �׸��ϴ�.
	RECT	m_rcScroll;
	//	Thumb�� �����Դϴ�. �� ������� �׸��ϴ�.
	RECT	m_rcThumb;
	//	���� ���콺�� ���ȴ����� ��ȯ�մϴ�.
	bool	m_bOnClick;
	//	��ũ���� �׷����� ���� ��ǥ�Դϴ�. �� ��ǥ�� �������� �Ͽ� ��ũ���� �׷����ϴ�.
	POINT	m_ptWorldPosition;

public:

	/*
	* �ʱ�ȭ �Լ��Դϴ�.
	*	SCRType : ��ũ�� Ÿ���Դϴ�. scroll::SCRType::SCR_H �� ���� ������� �ۼ��˴ϴ�.
	*	ScrLength : ��ũ���� ���� ������ �����Դϴ�.
	*	ScrViewLength : ȭ�鿡 ���̴� ������ �����Դϴ�.
	*	ScrollLength : ��ũ�ѹ��� �����Դϴ�.
	*	worldPosition : ��ũ���� �׷��� ������ ���� ���� ��ǥ�Դϴ�.
	*	Opacity : ��ũ���� �����Դϴ�. 0���� 255������ �����Դϴ�.
	*	ThumbLength( = 0.f) : ��ũ�� ���� �̵���ų �� �ִ� ���� ����(1/2)�Դϴ�. 0�� �Է��ϸ� �ڵ����� ����մϴ�.
			�� ThumbLength�� ���̸� �ڵ����� ������ �� �ִ� ���� : ScrollLength > ThumbLength * 2
	*	thickness : ��ũ���� �����Դϴ�.
	*/
	virtual void initialize(SCRType type, float ScrLength, float ScrViewLength, float ScrollLength, POINT WorldPosition,
		float Opacity = 255.0f, float ThumbLength = 0.f, float thickness = 5.0f)
	{
		basic_scroll::initialize(type, ScrLength, ScrViewLength, ScrollLength, ThumbLength, thickness);
		m_ptWorldPosition = WorldPosition;
		m_iOpacity = Opacity;
		RenewalThumbRect();
		if (m_scType == SCRType::SCR_H)
			m_rcScroll = {
			m_ptWorldPosition.x,
			m_ptWorldPosition.y,
			m_ptWorldPosition.x + (LONG)m_flScroll + 2 * (LONG)m_flThumb,
			m_ptWorldPosition.y + (LONG)m_flThickness };
		else
			m_rcScroll = {
			m_ptWorldPosition.x,
			m_ptWorldPosition.y,
			m_ptWorldPosition.x + (LONG)m_flThickness,
			m_ptWorldPosition.y + (LONG)m_flScroll + 2 * (LONG)m_flThumb };

	}

	/*
	* ���콺�� �������� ���� ��Ȳ�� ó���մϴ�.
	*/
	void TouchCheck(POINT pt, float ScrollmoveLength = 10.f)
	{
		auto pos = ((m_scType == SCRType::SCR_H) ? pt.x - m_ptWorldPosition.x : pt.y - m_ptWorldPosition.y);
		if (PtInRect(&m_rcThumb, pt))
		{
			SetGrip(pos);
			m_bOnClick = true;
		}
		else if (PtInRect(&m_rcScroll, pt))
		{
			if (pos > m_flThumb + m_fnowScrollpos)
				SetnowScrollPosition(ScrollmoveLength);
			else
				SetnowScrollPosition(-ScrollmoveLength);
			RenewalThumbRect();
			m_bOnClick = true;
		}
	}

	/*
	* SetGrip�� ȣ���ϰ� ���콺�� �巡���� �� ȣ���մϴ�.
	* Thumb�� ��ġ�� ���콺�� �̵��� ��ŭ �����մϴ�. �� ��ġ�� ��ũ�� �ٸ� Ƣ��� �� �����ϴ�.
	* ���� SetGrip�� ȣ������ �ʾ��� ��� false���� ��ȯ�ϸ�, �⺻������ true�� ��ȯ�մϴ�.
	* (�߰�) �巡�� �� Thumb�� �����մϴ�.
	*/
	virtual bool Drag(float pos)
	{
		bool bReturn = basic_scroll::Drag(pos);
		RenewalThumbRect();
		return bReturn;
	}

	/*
	* Thumb�� ��ġ�� �����մϴ�.
	*/
	void RenewalThumbRect()
	{
		if (m_scType == SCRType::SCR_H)
			m_rcThumb = {
			m_ptWorldPosition.x + (LONG)m_fnowScrollpos,
			m_ptWorldPosition.y - 1,
			m_ptWorldPosition.x + 2 * (LONG)m_flThumb + (LONG)m_fnowScrollpos,
			m_ptWorldPosition.y + (LONG)m_flThickness + 1 };
		else
			m_rcThumb = {
			m_ptWorldPosition.x - 1,
			m_ptWorldPosition.y + (LONG)m_fnowScrollpos,
			m_ptWorldPosition.x + (LONG)m_flThickness + 1,
			m_ptWorldPosition.y + 2 * (LONG)m_flThumb + (LONG)m_fnowScrollpos };
	}

	/*
	* ���콺�� �̿��� ��ũ���� ����� ���ϰ� ��������ϴ�.
	* ���콺 Ŭ���� ���� Thumb�� �̵�, ��ũ�ѹٸ� Ŭ���Ͽ� Thumb�� �̵��� ������ �ֽ��ϴ�.
	* true�� �����Ͽ� ���μ����� �� ���������� ���θ� ������ �� �ֽ��ϴ�.
	*/
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:

			m_bOnClick = false;
			TouchCheck(pt);
			break;
		case WM_MOUSEMOVE:
			if (m_bOnClick)
			{
				if (!Drag(((m_scType == SCRType::SCR_H) ? pt.x - m_ptWorldPosition.x : pt.y - m_ptWorldPosition.y)))
					if (PtInRect(&m_rcScroll, pt))
					{
						if (((m_scType == SCRType::SCR_H) ? pt.x - m_ptWorldPosition.x : pt.y - m_ptWorldPosition.y) > m_flThumb + m_fnowScrollpos)
							SetnowScrollPosition(10.f);
						else
							SetnowScrollPosition(-10.f);
						RenewalThumbRect();
					}
			}
			break;
		case WM_LBUTTONUP:
			ReleaseGrip();
			m_bOnClick = false;
			break;
		}
		return false;
	}

	/*
	* ��ũ���� �׷��ݴϴ�.
	* �𼭸��� �ձ� �簢���� �⺻���̸�, ���������� �����Ͽ� ����Ͻ� �� �ֽ��ϴ�.
	* ������ �̿��Ͽ� �������� ��ũ���� ������ �� �ֽ��ϴ�.
	*/
	virtual void Draw(HDC hDC, RECT DrawAreaSize, COLORREF color = RGB(0,0,0))
	{

		BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.AlphaFormat = 0;
		HDC LayDC = CreateCompatibleDC(hDC);
		HBITMAP Lay = CreateCompatibleBitmap(hDC, DrawAreaSize.right, DrawAreaSize.bottom);
		SelectObject(LayDC, Lay);

		bf.SourceConstantAlpha = m_iOpacity;
		BitBlt(LayDC, 0, 0, DrawAreaSize.right, DrawAreaSize.bottom, hDC, 0, 0, SRCCOPY);

		SetDCBrushColor(LayDC, RGB(255, 255, 255));
		SelectObject(LayDC, (HBRUSH)GetStockObject(DC_BRUSH));

		RoundRect(LayDC, m_rcScroll.left, m_rcScroll.top, m_rcScroll.right, m_rcScroll.bottom, m_flThickness * 0.5f, m_flThickness * 0.5f);

		SetDCPenColor(LayDC, color);
		SetDCBrushColor(LayDC, color);
		SelectObject(LayDC, (HBRUSH)GetStockObject(DC_BRUSH));
		SelectObject(LayDC, (HPEN)GetStockObject(DC_PEN));
		RoundRect(LayDC, m_rcThumb.left, m_rcThumb.top, m_rcThumb.right, m_rcThumb.bottom, m_flThickness * 0.5f, m_flThickness * 0.5f);

		AlphaBlend(hDC, DrawAreaSize.left, DrawAreaSize.top, DrawAreaSize.right - DrawAreaSize.left, DrawAreaSize.bottom - DrawAreaSize.top,
			LayDC, DrawAreaSize.left, DrawAreaSize.top, DrawAreaSize.right - DrawAreaSize.left, DrawAreaSize.bottom - DrawAreaSize.top, bf);

		DeleteDC(LayDC);
		DeleteObject(Lay);
	}

};
