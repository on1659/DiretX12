#pragma once


class CriticalSection
{
private:
	CRITICAL_SECTION cs;

public:

	/*
	클래스 내부에서 정의된 함수 - (중괄호{ }로 정의된 멤버함수, cpp파일 ::이 아닌)
	는 자동으로 inline함수가 된다
	*/

	void InitCS()
	{
		InitializeCriticalSection(&cs);
	}

	void EnterCS()
	{
		EnterCriticalSection(&cs);
	}
	void LeaveCS()
	{
		LeaveCriticalSection(&cs);
	}

	void DeleteCS()
	{
		DeleteCriticalSection(&cs);
	}

};