

class Event
{
private:

public:

	/*
	Ŭ���� ���ο��� ���ǵ� �Լ� - (�߰�ȣ{ }�� ���ǵ� ����Լ�, cpp���� ::�� �ƴ�)
	�� �ڵ����� inline�Լ��� �ȴ�
	*/

	HANDLE hReadEvent, hWriteEvent; // �̺�Ʈ
	HANDLE hGetRoomInfoEvent, hGetUserInfoEvent; // �̺�Ʈ
	//HANDLE hRoomQuitEvent; // �̺�Ʈ

	int InitEvent()
	{
		// �̺�Ʈ ����
		hReadEvent = CreateEvent(nullptr, FALSE, TRUE, nullptr);
		if (hReadEvent == nullptr) return 1;
		hWriteEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (hWriteEvent == nullptr) return 1;

		hGetRoomInfoEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
		if (hGetRoomInfoEvent == nullptr) return 1;
		hGetUserInfoEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
		if (hGetUserInfoEvent == nullptr) return 1;

		//hRoomQuitEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		//if (hRoomQuitEvent == nullptr) return 1;

		return 0;
	}


	void DeleteEvent()
	{
		CloseHandle(hReadEvent);
		CloseHandle(hWriteEvent);

		CloseHandle(hGetRoomInfoEvent);
		CloseHandle(hGetUserInfoEvent);

		//CloseHandle(hRoomQuitEvent);
	}
};