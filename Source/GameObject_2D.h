#pragma once

#include <iostream>
#include <vector>

class CSpriteObject_2D;
class BoundingBox_2D;

class CGameObject_2D : public CSpriteObject_2D
{
public:
	//까먹ㅇ
	int						m_State;
	//처음
	float					m_fSpeed;
	//상시
	float					m_fDirX;
	float					m_fDirY;
	//처음
	int						m_Color;

	//상시 거의안쓰
	int						m_nLastSpriteHeight;



	//화면의 가로세로
	int						m_WindowsWidth;
	int						m_WindowsHeight;

public:

	CGameObject_2D();

	CGameObject_2D(std::string name);

	virtual ~CGameObject_2D();

	virtual void Init();

	virtual void Init(std::string name);

	virtual void OnCreate();

	virtual void OnCreate(float cx, float cy, float width, float height, float frameFPS);

	virtual void OnCreate(float cx, float cy, float width, float height, float frameFPS, int frameCount, int frameCountHeight);

	virtual void OnKeyBoard(){}

	virtual void OnUpdate(float fTime);

	virtual void OnDraw(HDC hdc);

	virtual void OnCollision();

	virtual bool OnCollision(BoundingBox_2D col);

	virtual bool OnCollision(CGameObject_2D *object);

	virtual int OnCollision(int n, CGameObject_2D* ...);

	//Move

	virtual void OnMove(float fTimeElapsed);

	virtual void MoveLeft(float fTimeElapsed);

	virtual void MoveRight(float fTimeElapsed);

	virtual void MoveUp(float fTimeElapsed);

	virtual void MoveDown(float fTimeElapsed);

	//Set
	void SetSpeed(float speed);

	float GetSpeed(){ return m_fSpeed; }

};
