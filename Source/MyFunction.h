#pragma once

#include <windows.h>
#include <math.h>

class Click_2D
{

public:

	static bool contains(RECT a, float x, float y)
	{
		return a.left < a.right && a.top < a.bottom
			&& x >= a.left && x < a.right && y >= a.top && y < a.bottom;
	}

	static bool contain(RECT a, RECT b)
	{
		return a.left < a.right && a.top < a.bottom
			&& a.left <= b.left && a.top <= b.top && a.right >= b.right && a.bottom >= b.bottom;
	}


};

class BoundingBox_2D
{

public:
	float			left;
	float			right;
	float			top;
	float			bot;

	float			cx;
	float			cy;
	float			w;
	float			h;

public:

	void SetBoxLTRD(float _left,float _top, float _right,  float _bot)
	{
		left = _left; right = _right, top = _top; bot = _bot;
		w = right - left;
		h = bot - top;
		cx = left + w / 2;
		cy = top +  h / 2;
	}

	void SetBoxCenter(float cx, float cy, float _w, float _h)
	{
		w = _w;
		h = _h;
		BoundingBox_2D::cx = cx;
		BoundingBox_2D::cy = cy;
		left = cx - w / 2;
		right = cx + w / 2;
		top = cy - h / 2;
		bot = cy + h / 2;
	}

	bool Collision(BoundingBox_2D other)
	{
		if (left > other.right)return false;
		if (right < other.left)return false;
		if (top > other.bot)return false;
		if (bot < other.top)return false;
		return true;
	}

	bool Contains(float x, float y)
	{
		return left < right && top < bot
			&& x >= left && x < right && y >= top && y < bot;
	}

	void Update(int cx, int cy)
	{
		left = cx - w / 2;
		right = cx + w / 2;
		top = cy - h / 2;
		bot = cy + h / 2;
		BoundingBox_2D::cx = cx;
		BoundingBox_2D::cy = cy;
	}

};

