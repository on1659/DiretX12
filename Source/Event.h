

class Event
{
private:

public:

	/*
	클래스 내부에서 정의된 함수 - (중괄호{ }로 정의된 멤버함수, cpp파일 ::이 아닌)
	는 자동으로 inline함수가 된다
	*/

	HANDLE hReadEvent, hWriteEvent; // 이벤트
	HANDLE hGetRoomInfoEvent, hGetUserInfoEvent; // 이벤트
	//HANDLE hRoomQuitEvent; // 이벤트

	int InitEvent()
	{
		// 이벤트 생성
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