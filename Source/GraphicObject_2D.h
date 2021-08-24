#pragma once

#include <windows.h>
#include "MyFunction.h"

class BoundingBox_2D;

class CGraphicObject_2D
{
public:
	//처음한번
	std::string				m_name;

public:
	//상시로
	float					m_cx;
	float					m_cy;

	//처음한번
	float					m_width;
	float					m_height;

	float					m_imgWidth;
	float					m_imgHeight;

	//상시
	BoundingBox_2D			m_BoundingBox;

protected:
	COLORREF				m_Rcolor;


public:


	CGraphicObject_2D();

	CGraphicObject_2D(std::string name);

	CGraphicObject_2D(std::string name, float cx, float cy);

	virtual ~CGraphicObject_2D();

	virtual void Init();

	virtual void Init(std::string name);

	virtual void Init(std::string name, float cx, float cy);

	virtual void OnCreate(float cx, float cy, float width, float height);

	virtual void OnDraw(HDC hdc);

	virtual void OnDrawAlpha(HDC hdc, int alpha);

	virtual void OnUpdate(float fTimeElapsed);

	virtual void SetPosition(float cx, float cy, float width, float height);



	//Set

	void SetPosition(float cx, float cy);

	void SetX(float cx){ m_cx = cx; }

	void SetY(float cy){ m_cy = cy; }

	void SetWidth(float width){ m_width = width; }

	void SetHeight(float height){ m_height = height; }

	void SetRemoveColor(int r, int g, int b);


	//Get

	float GetX(){ return m_cx; }

	float GetY(){ return m_cy; }

	float GetWidth() { return m_width; }

	float GetHeight() { return m_height; }

	virtual BoundingBox_2D GetBoundingBox();
};
