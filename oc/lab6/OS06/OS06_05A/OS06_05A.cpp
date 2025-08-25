#include <iostream>
#include <Windows.h>


int main()
{
	int pid = GetCurrentProcessId();
	HANDLE event = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"OS06_05");

	WaitForSingleObject(event, INFINITE);
	for (int i = 1; i <= 90; i++)
	{
		printf("<OS06_05A>\t %d.  PID = %d\n", i, pid);
		Sleep(100);
	}

	CloseHandle(event);
}