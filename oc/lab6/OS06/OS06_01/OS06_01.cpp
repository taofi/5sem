#include <iostream>
#include <ctime>
#include <Windows.h>

using namespace std;
bool section;


HANDLE createThread(LPTHREAD_START_ROUTINE func, char* thread_name)
{
	DWORD thread_id = NULL;
	HANDLE thread = CreateThread(NULL, 0, func, thread_name, 0, &thread_id);
	if (thread == NULL)
		throw "[ERROR] CreateThread";
	return thread;
}


void EnterCriticalSectionAssembly()
{
	_asm
	{
	CriticalSection:
		lock bts section, 0;
		jc CriticalSection
	}
}


void LeaveCriticalSectionAssembly() 
{
	_asm lock btr section, 0 
}


void WINAPI loop(char* displayed_name)
{
	int tid = GetCurrentThreadId();

	for (int i = 1; i <= 60; ++i)
	{
		if (i == 15)
			EnterCriticalSectionAssembly();

		printf("%d.\tTID = %u\tsection: %d\t%s\n", i, tid, section, displayed_name);

		if (i == 40)
			LeaveCriticalSectionAssembly();
		if (i >= 40) {
			EnterCriticalSectionAssembly();
			LeaveCriticalSectionAssembly();
		}

		Sleep(100);
	}

	cout << "\n" << displayed_name << " is over" << "\n";
}


int main()
{
	const int size = 2;
	HANDLE threads[size];

	threads[0] = createThread((LPTHREAD_START_ROUTINE)loop, (char*)"FirstT");
	threads[1] = createThread((LPTHREAD_START_ROUTINE)loop, (char*)"SecondT");

	WaitForMultipleObjects(size, threads, TRUE, INFINITE);
	for (int i = 0; i < size; i++)
		CloseHandle(threads[i]);

	return 0;
}