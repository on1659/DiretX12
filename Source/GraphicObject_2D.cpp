
#include "stdafx.h"
#include "GraphicObject_2D.h"
#include "RenderManager_2D.h"

CGraphicObject_2D::CGraphicObject_2D()
{

	m_name.clear();;

	m_cx = 0;
	m_cy = 0;
	m_width = 0;
	m_height = 0;

	m_imgWidth = 0;
	m_imgHeight = 0;

	m_Rcolor = RGB(255, 0, 255);

}

CGraphicObject_2D::CGraphicObject_2D(std::string name)
{
	CGraphicObject_2D::Init(name);
}

CGraphicObject_2D::CGraphicObject_2D(std::string name, float cx, float cy)
{
	CGraphicObject_2D::Init(name, cx, cy);

}

CGraphicObject_2D::~CGraphicObject_2D()
{
	//delete m_image;
}

void CGraphicObject_2D::Init()
{
	m_name.clear();
	m_Rcolor = RGB(255, 0, 255);
	m_imgWidth = 0.0f;
	m_imgHeight = 0.0f;

	m_width = m_imgWidth;
	m_height = m_imgHeight;

	m_BoundingBox.SetBoxCenter(m_cx, m_cy, m_width, m_height);
}

void CGraphicObject_2D::Init(std::string name)
{
	m_name = name;
	m_Rcolor = RGB(255, 0, 255);
	m_imgWidth = (float)CRenderManager_2D::GetInstance()->GetImage(name)->GetWidth();
	m_imgHeight = (float)CRenderManager_2D::GetInstance()->GetImage(name)->GetHeight();


	m_width = m_imgWidth;
	m_height = m_imgHeight;

	m_BoundingBox.SetBoxCenter(m_cx, m_cy, m_width, m_height);
}

void CGraphicObject_2D::Init(std::string name, float cx, float cy)
{
	m_name = name;
	m_Rcolor = RGB(255, 0, 255);
	m_imgWidth = (float)CRenderManager_2D::GetInstance()->GetImage(name)->GetWidth();
	m_imgHeight = (float)CRenderManager_2D::GetInstance()->GetImage(name)->GetHeight();

	m_cx = cx;
	m_cy = cy;

	m_width = m_imgWidth;
	m_height = m_imgHeight;

	m_BoundingBox.SetBoxCenter(m_cx, m_cy, m_width, m_height);
}

void CGraphicObject_2D::OnCreate(float cx, float cy, float width, float height)
{
	m_cx = cx; m_cy = cy, m_width = width, m_height = height;
	m_BoundingBox.SetBoxCenter(m_cx, m_cy, m_width, m_height);
}


void CGraphicObject_2D::SetPosition(float cx, float cy, float width, float height)
{
	m_cx = cx; m_cy = cy, m_width = width, m_height = height;
	m_BoundingBox.SetBoxCenter(m_cx, m_cy, m_width, m_height);
}

void CGraphicObject_2D::OnDraw(HDC hdc)
{
	RENDERMGR_2D->Render(hdc, m_name, (m_cx - m_width / 2), (m_cy - m_height / 2), m_width, m_height, 0, 0, m_imgWidth, m_imgHeight, m_Rcolor);
}

void CGraphicObject_2D::OnDrawAlpha(HDC hdc, int alpha)
{
	RENDERMGR_2D->DrawImageSpriteAlphaBackground(hdc, m_name, (m_cx - m_width / 2), (m_cy - m_height / 2), m_width, m_height, 0, 0, m_imgWidth, m_imgHeight, m_Rcolor, alpha);
}

void CGraphicObject_2D::OnUpdate(float fTimeElapsed){};

void CGraphicObject_2D::SetPosition(float cx, float cy)
{
	m_cx = cx;
	m_cy = cy;
	m_BoundingBox.SetBoxCenter(m_cx, m_cy, m_width, m_height);
}

void CGraphicObject_2D::SetRemoveColor(int r, int g, int b)
{
	m_Rcolor = RGB(r, g, b);
}

BoundingBox_2D CGraphicObject_2D::GetBoundingBox()
{
	return m_BoundingBox;
}