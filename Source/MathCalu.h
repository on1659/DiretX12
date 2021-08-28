#pragma once

#include <math.h>

class Vector2 
{

public:
	float x;
	float y;

	Vector2() : x(0.0f), y(0.0f)
	{
	}

	//생성자도 여러가지로 만들 수 있음
	Vector2(const float& x, const float& y) : x(x), y(y) {}

	~Vector2() {}

	void set(const float& x, const float& y)
	{
		Vector2::x = x; 	Vector2::y = y;
	}

	//const를 붙이는건 other은 수정하지 않는거니깐 붙이는거
	Vector2 operator+(const Vector2& other)
	{
		return Vector2(x + other.x, y + other.y);	//Vector2라는 객체를 생성해서 리턴함
	}
	void operator+=(const Vector2& other)
	{
		x += other.x; y += other.y;
	}

	Vector2 operator-(const Vector2& other)
	{
		return Vector2(x - other.x, y - other.y);
	}
	void operator-=(const Vector2& other)
	{
		x -= other.x; y -= other.y;
	}


	//하나는 벡터끼리 곱셈이고, 하나는 벡터*스칼라 곱셈
	Vector2 operator*(const Vector2& other) const
	{
		return Vector2(x * other.x, y * other.y);

	}
	void operator*=(const Vector2& other)
	{
		x *= other.x; y *= other.y;
	}

	Vector2 operator*(const float& a) const
	{
		return Vector2(x * a, y * a);
	}
	void operator*=(const float& a)
	{
		x *= a; y *= a;
	}


	Vector2 operator/(const Vector2& other) const
	{
		return Vector2(x / other.x, y / other.y);
	}
	void operator/=(const Vector2& other)
	{
		x /= other.x; y /= other.y;
	}

	Vector2 operator/(const float& a) const
	{
		return Vector2(x / a, y / a);
	}
	void operator/=(const float& a)
	{
		x /= a; y /= a;
	}

	/*
	게임프로그램이할때 쓰이는 기본적인 기하벡터쪽 함수들
	수식은 수학책에 다 나와있음..!
	*/

	//내적 값이 변하지 말라고 const를 써줌
	float InnerProduct(Vector2 other) const
	{
		return (x * other.x) + (y * other.y);
	}

	//벡터의 크기
	float scale() const	{ return sqrt((x * x) + (y * y)); }

	//단휘벡터 만들기 
	Vector2 normalize() const
	{
		float s = scale();
		return Vector2(x / s, y / s);
	}

	//두 벡터간의 거리.. (현재 이 벡터와 다른 벡터간의 거리
	float length(const Vector2& other) const
	{
		return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
	}

};

class MathCalu{

public:
	MathCalu(){};
	~MathCalu(){};

	static float Scale(Vector2 A){
		return sqrt((A.x * A.x) + (A.y * A.y));
	}

	static float Length(Vector2 A, Vector2 B)
	{
		return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
	}

	static Vector2 Normalize(Vector2 A)
	{
		Vector2 tmp;
		tmp.x = A.x / Scale(A);
		tmp.y = A.y / Scale(A);
		return tmp;
	}

	static float InnerProduct(float x, float y, float x2, float y2)
	{
		return (x * x2) + (y * y2);
	}

	static float VectorScale(float x, float y, float x2, float y2)
	{
		return sqrt(pow(x, 2.0f) + pow(y, 2.0f)) + sqrt(pow(x2, 2.0f) + pow(y2, 2.0f));
	}

	static float getACOS(float x, float y, float x2, float y2)
	{
		float dot    = InnerProduct(x, y, x2, y2);
		float length =  VectorScale(x, y, x2, y2);
		float radian = acos(dot / length);
		float degree = radian * 180 / PI;
		return degree;
	}

	static float getCos(float x, float y, float x2, float y2)
	{
		float a, b;
		a = -y;
		b = x;
		float tmp_dot = InnerProduct(a, b, x2, y2);
		float tmp_length = VectorScale(a, b, x2, y2);
		float tmp_radian = acos(tmp_dot / tmp_length);
		float tmp_degree = tmp_radian * 180 / PI;

		if (tmp_degree < 90)
			return getACOS(x, y, x2, y2);

		else
			return 360 - getACOS(x, y, x2, y2);
	}


	static float InnerProduct(Vector2 A, Vector2 B)
	{
		return (A.x * B.x) + (A.y * B.y);
	}

	static float VectorScale(Vector2 A, Vector2 B)
	{
		return sqrt(pow(A.x, 2.0f) + pow(A.y, 2.0f)) + sqrt(pow(B.x, 2.0f) + pow(B.y, 2.0f));
	}

	static float getACOS(Vector2 A, Vector2 B)
	{
		float dot = InnerProduct(A.x, A.y, B.x, B.y);
		float length = VectorScale(A.x, A.y, B.x, B.y);
		float radian = acos(dot / length);
		float degree = radian * 180 / PI;
		return degree;
	}

	static float getCos(XMFLOAT3 a, XMFLOAT3 b)
	{
		return getCos(Vector2(a.x, a.z), Vector2(b.x, b.z));
	}

	static float getCos(Vector2 A, Vector2 B)
	{
		float a, b;
		a = -A.y;
		b = B.x;
		float tmp_dot = InnerProduct(a, b, B.x, B.y);
		float tmp_length = VectorScale(a, b, B.x, B.y);
		float tmp_radian = std::acos(tmp_dot / tmp_length);
		float tmp_degree = tmp_radian * 180 / PI;

		return getACOS(A.x, A.y, B.x, B.y);
			return 360 - getACOS(A.x, A.y, B.x, B.y);

	}

	static void ADD_RECTnPOINT(RECT *a, XMVECTOR xm, float fSpeed)
	{
		xm *= fSpeed;
		XMFLOAT2 b;
		XMStoreFloat2(&b, xm);

		a->left = (a->left + b.x);
		a->right = (a->right + b.x);
		a->top = (a->top + b.y);
		a->bottom = (a->bottom + b.y);
	}


};
