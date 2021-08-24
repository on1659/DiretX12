#pragma once

#include "stdafx.h"
#include "SpriteObject_2D.h"
#include "Timer.h"
///SpriteObejct


CSpriteObject_2D::CSpriteObject_2D() : CGraphicObject_2D()
{
	this->Init();
}

CSpriteObject_2D::CSpriteObject_2D(std::string name) : CGraphicObject_2D(name)
{
	CSpriteObject_2D::Init(name);

}

CSpriteObject_2D::CSpriteObject_2D(std::string name, float cx, float cy) : CGraphicObject_2D(name, cx, cy)
{
	CSpriteObject_2D::Init(name, cx, cy);
}



CSpriteObject_2D::~CSpriteObject_2D()
{
};

void CSpriteObject_2D::Init()
{
	m_name.clear();

	m_width = m_imgWidth;
	m_height = m_imgHeight;

	m_frameFPS = 1.0f;
	m_frameCount = 1.0f;
	m_frameLeft = 0.0f;
	m_frameTop = 0.0f;
	m_frameWidth = 0.0f;
	m_frameCountHeight = 0.0f;

	m_Rcolor = RGB(255, 0, 255);
	m_imgWidth = 0.0f;
	m_imgHeight = 0.0f;

	m_width = m_imgWidth;
	m_height = m_imgHeight;


	m_BoundingBox.SetBoxCenter(0, 0, m_width, m_height);
}

void CSpriteObject_2D::Init(std::string name)
{
	m_name = name;

	m_frameFPS = 1;
	m_frameCount = 1;
	m_frameLeft = 0;
	m_frameTop = 0;
	m_frameWidth = 0;
	m_frameCountHeight = 0;

	m_Rcolor = RGB(255, 0, 255);
	m_imgWidth = (float)CRenderManager_2D::GetInstance()->GetImage(name)->GetWidth();
	m_imgHeight = (float)CRenderManager_2D::GetInstance()->GetImage(name)->GetHeight();


	m_width = m_imgWidth;
	m_height = m_imgHeight;

	m_BoundingBox.SetBoxCenter(0, 0, m_width, m_height);
}

void CSpriteObject_2D::Init(std::string name, float cx, float cy)
{
	m_name = name;

	m_frameFPS = 1;
	m_frameCount = 1;
	m_frameLeft = 0;
	m_frameTop = 0;
	m_frameWidth = 0;
	m_frameCountHeight = 0;

	m_Rcolor = RGB(255, 0, 255);

	m_imgWidth = (float)CRenderManager_2D::GetInstance()->GetImage(name)->GetWidth();
	m_imgHeight = (float)CRenderManager_2D::GetInstance()->GetImage(name)->GetHeight();

	m_cx = cx;
	m_cy = cy;

	m_width = m_imgWidth;
	m_height = m_imgHeight;


	m_BoundingBox.SetBoxCenter(0, 0, m_width, m_height);
}

/*
cx,cy,width,height 는 캐릭터의 위치.
frameFPS    : 애니메이션 전환 속도입니다.
frameCount  : 스프라이트의 가로의 이미지 총 크기입니다.
frameHeight : 스프라이트의 세로 이미지 개수 입니다.
*/
void CSpriteObject_2D::OnCreate(float cx, float cy, float width, float height)
{
	CGraphicObject_2D::OnCreate(cx, cy, width, height);
	m_BoundingBox.SetBoxCenter(m_cx, m_cy, m_width, m_height);
}

void CSpriteObject_2D::OnCreate(float cx, float cy, float width, float height, float frameFPS, int frameCount, int frameCountHeight)
{
	m_cx = cx; m_cy = cy; m_width = width; m_height = height;
	m_frameFPS = 1 / frameFPS;
	m_frameCount = frameCount;
	m_imgWidth = m_imgWidth / frameCount;
	m_imgHeight = m_imgHeight / frameCountHeight;
	
	m_BoundingBox.SetBoxCenter(m_cx, m_cy, m_width, m_height);
}

void CSpriteObject_2D::OnCreate(float cx, float cy, float frameFPS, int frameCount, int frameCountHeight)
{
	m_cx = cx; m_cy = cy;

	m_width = (float)CRenderManager_2D::GetInstance()->GetImage(m_name)->GetWidth() / frameCount;
	m_height = (float)CRenderManager_2D::GetInstance()->GetImage(m_name)->GetHeight() / frameCountHeight;

	m_frameFPS = 1 / frameFPS;
	m_frameCount = frameCount;
	m_imgWidth = m_imgWidth / frameCount;
	m_imgHeight = m_imgHeight / frameCountHeight;


	m_BoundingBox.SetBoxCenter(m_cx, m_cy, m_width, m_height);
}

void CSpriteObject_2D::OnCreate(std::string name)
{
	m_name = name;

	m_frameFPS = 1;
	m_frameCount = 1;
	m_frameLeft = 0;
	m_frameTop = 0;
	m_frameWidth = 0;
	m_frameCountHeight = 0;

	m_BoundingBox.SetBoxCenter(m_cx, m_cy, m_width, m_height);
}

void CSpriteObject_2D::SetSprite(float frameFPS, int frameCount, int frameCountHeight)
{
	m_width = m_imgWidth;//(float)CRenderManager_2D::GetInstance()->GetImage(m_name)->GetWidth() / frameCount;
	m_height = m_imgHeight;//(float)CRenderManager_2D::GetInstance()->GetImage(m_name)->GetHeight() / frameCountHeight;

	m_frameFPS = 1 / frameFPS;
	m_frameCount = frameCount;
	m_imgWidth = m_imgWidth / frameCount;
	m_imgHeight = m_imgHeight / frameCountHeight;

	m_width = m_imgWidth;
	m_height = m_imgHeight;

}

void CSpriteObject_2D::SetSpriteHeight(int
	n)
{
	m_frameCountHeight = n;
}

void CSpriteObject_2D::SetName(std::string name)
{
	m_name = name;
	m_frameFPS = 1;
	m_frameCount = 1;
	m_frameLeft = 0;
	m_frameTop = 0;
	m_frameWidth = 0;
	m_frameCountHeight = 0;

	m_Rcolor = RGB(255, 0, 255);
	m_imgWidth = (float)CRenderManager_2D::GetInstance()->GetImage(name)->GetWidth();
	m_imgHeight = (float)CRenderManager_2D::GetInstance()->GetImage(name)->GetHeight();

	m_width = m_imgWidth;
	m_height = m_imgHeight;

}

void CSpriteObject_2D::OnDraw(HDC hdc)
{
	OnSpriteUpdate();

	RENDERMGR_2D->Render
		(hdc, m_name, (m_cx - m_width / 2), (m_cy - m_height / 2), m_width, m_height,
		m_frameLeft, m_frameTop, m_imgWidth, m_imgHeight, m_Rcolor);
}

void CSpriteObject_2D::OnDraw(std::string name, HDC hdc)
{
	RENDERMGR_2D->Render
		(hdc, name, (m_cx - m_width / 2), (m_cy - m_height / 2), m_width, m_height,
		m_frameLeft, m_frameTop, m_imgWidth, m_imgHeight, m_Rcolor);
}

void CSpriteObject_2D::OnDrawAlpha(HDC hdc, int alpha)
{
	RENDERMGR_2D->DrawImageSpriteAlphaBackground
		(hdc, m_name, (m_cx - m_width / 2), (m_cy - m_height / 2), m_width, m_height, m_frameLeft, m_frameTop, m_imgWidth, m_imgHeight, m_Rcolor, alpha);
}


void CSpriteObject_2D::OnSpriteUpdate()
{
	m_frameLeft = m_imgWidth  * (int)m_frameWidth;
	m_frameTop = m_imgHeight * (int)m_frameCountHeight;


	m_frameWidth += m_frameFPS;

	if (m_frameWidth > m_frameCount)
	{
		m_frameWidth = 0;

	}
}