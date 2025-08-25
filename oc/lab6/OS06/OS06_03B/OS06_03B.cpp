#include <iostream>
#include <Windows.h>


int main()
{
	int pid = GetCurrentProcessId();
	HANDLE mutex = OpenMutex(SYNCHRONIZE, FALSE, L"OS06_03");

	for (int i = 1; i <= 90; ++i)
	{
		if (i == 30)
			WaitForSingleObject(mutex, INFINITE);

		else if (i == 60)
			ReleaseMutex(mutex);
		else if (i > 60)
		{
			WaitForSingleObject(mutex, INFINITE);
			ReleaseMutex(mutex);
		}
		printf("<OS06_03B>\t%d.  PID = %d\n", i, pid);
		Sleep(100);
	}

	CloseHandle(mutex);
	system("pause");

}