#pragma once


class CriticalSection
{
private:
	CRITICAL_SECTION cs;

public:

	/*
	Ŭ���� ���ο��� ���ǵ� �Լ� - (�߰�ȣ{ }�� ���ǵ� ����Լ�, cpp���� ::�� �ƴ�)
	�� �ڵ����� inline�Լ��� �ȴ�
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