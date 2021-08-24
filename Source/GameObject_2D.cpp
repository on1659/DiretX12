#include "stdafx.h"
#include "GameObject_2D.h"
#include "MyFunction.h"
#include "GameFrameWork_2D.h"


CGameObject_2D::CGameObject_2D()
{
	//m_State = AnimState_STOP;
	m_fSpeed = 0.0f;
	m_fDirX = 0.0f;
	m_fDirY = 0.0f;

	m_nLastSpriteHeight = (int)m_imgHeight;

	m_WindowsWidth = FRAMEWORK_2D->GetWindowWidth();
	m_WindowsHeight = FRAMEWORK_2D->GetWindowHeight();

}

CGameObject_2D::CGameObject_2D(std::string name)
{
	CSpriteObject_2D::Init(name);

	//m_State = AnimState_STOP;
	m_fSpeed = 0.0f;
	m_fDirX = 0.0f;
	m_fDirY = 0.0f;

	m_nLastSpriteHeight = (int)m_imgHeight;

	m_WindowsWidth = FRAMEWORK_2D->GetWindowWidth();
	m_WindowsHeight = FRAMEWORK_2D->GetWindowHeight();

}

CGameObject_2D::~CGameObject_2D()
{
}

void CGameObject_2D::Init()
{
	CSpriteObject_2D::Init();
	//m_State = AnimState_STOP;
	m_fSpeed = 0.0f;
	m_fDirX = 0.0f;
	m_fDirY = 0.0f;


	m_WindowsWidth = FRAMEWORK_2D->GetWindowWidth();
	m_WindowsHeight = FRAMEWORK_2D->GetWindowHeight();


}

void CGameObject_2D::Init(std::string name)
{
	CSpriteObject_2D::Init(name);

	//m_State = AnimState_STOP;
	m_fSpeed = 0.0f;
	m_fDirX = 0.0f;
	m_fDirY = 0.0f;

	m_WindowsWidth = FRAMEWORK_2D->GetWindowWidth();
	m_WindowsHeight = FRAMEWORK_2D->GetWindowHeight();

}

void CGameObject_2D::OnCreate(){ }

void CGameObject_2D::OnCreate(float cx, float cy, float width, float height, float frameFPS)
{
	CSpriteObject_2D::OnCreate(cx, cy, width, height, frameFPS, 1, 1);
	CSpriteObject_2D::OnCreate(cx, cy, width, height);
	m_BoundingBox.SetBoxCenter(m_cx, m_cy, m_width, m_height);
}

void CGameObject_2D::OnCreate(float cx, float cy, float width, float height, float frameFPS, int frameCount, int frameCountHeight)
{
	CSpriteObject_2D::OnCreate(cx, cy, frameFPS, frameCount, frameCountHeight);
	CSpriteObject_2D::OnCreate(cx, cy, width, height);
	m_BoundingBox.SetBoxCenter(m_cx, m_cy, m_width, m_height);
}

void CGameObject_2D::OnUpdate(float fTime)
{
	CSpriteObject_2D::OnSpriteUpdate();
	m_BoundingBox.Update((int)m_cx, (int)m_cy);
}

void CGameObject_2D::OnDraw(HDC hdc)
{
	CSpriteObject_2D::OnDraw(hdc);

#ifdef __BOUNDINGBOX__
	Draw_2D::drawRect(hdc, m_BoundingBox.left, m_BoundingBox.top, m_BoundingBox.w, m_BoundingBox.h, WHITE, RED);
#endif
}

void CGameObject_2D::OnCollision()
{

}

bool CGameObject_2D::OnCollision(BoundingBox_2D col)
{
	return m_BoundingBox.Collision(col);
}

bool CGameObject_2D::OnCollision(CGameObject_2D *object)
{
	return m_BoundingBox.Collision(object->m_BoundingBox);
}


int CGameObject_2D::OnCollision(int n, CGameObject_2D* ...)
{
	return 999;
}

//Set
void CGameObject_2D::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

//Move

void CGameObject_2D::OnMove(float fTimeElapsed)
{
	//m_cx += m_fDirX * m_fSpeed * fTimeElapsed;
	//m_cy += m_fDirY * m_fSpeed * fTimeElapsed;
}

void CGameObject_2D::MoveLeft(float fTimeElapsed)
{
	//m_cx -= m_fSpeed * fTimeElapsed;
}

void CGameObject_2D::MoveRight(float fTimeElapsed)
{
	//m_cx += m_fSpeed * fTimeElapsed;
}

void CGameObject_2D::MoveUp(float fTimeElapsed)
{
	//m_cy -= m_fSpeed * fTimeElapsed;
}

void CGameObject_2D::MoveDown(float fTimeElapsed)
{
	//m_cy += m_fSpeed * fTimeElapsed;
}
