#pragma once

#include <iostream>
#include <vector>

class CSpriteObject_2D;
class BoundingBox_2D;

class CGameObject_2D : public CSpriteObject_2D
{
public:
	//��Ԥ�
	int						m_State;
	//ó��
	float					m_fSpeed;
	//���
	float					m_fDirX;
	float					m_fDirY;
	//ó��
	int						m_Color;

	//��� ���ǾȾ�
	int						m_nLastSpriteHeight;



	//ȭ���� ���μ���
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
