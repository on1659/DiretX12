#pragma once

#include "GraphicObject_2D.h"

class CSpriteObject_2D :public CGraphicObject_2D
{
protected:
	//ó��
	float						m_frameFPS;

	//ó��
	int							m_frameCount;

	//ó��
	float						m_frameWidth;

public:
	//���
	float						m_frameCountHeight;

protected:
	//���
	float						m_frameLeft;

	//���
	float						m_frameTop;

public:

	CSpriteObject_2D();

	CSpriteObject_2D(std::string name);

	CSpriteObject_2D(std::string name, float cx, float cy);

	virtual ~CSpriteObject_2D();

	void Init();

	void Init(std::string name);

	void Init(std::string name, float cx, float cy);

	void OnCreate(std::string name);

	void OnCreate(float cx, float cy, float width, float height);

	void OnCreate(float cx, float cy, float frameFPS, int frameCount, int frameCountHeight);

	void OnCreate(float cx, float cy, float width, float height, float frameFPS, int frameCount, int frameCountHeight);

	void OnDraw(HDC hdc);

	void OnDraw(std::string name, HDC hdc);

	void OnDrawAlpha(HDC hdc, int alpha);

	void OnSpriteUpdate();

	void SetSpriteHeight(int n);

	void SetSprite(float frameFPS, int frameCount, int frameCountHeight);

	void SetName(std::string name);


};