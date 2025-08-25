#include <iostream>
#include <windows.h>

using namespace std;

CRITICAL_SECTION critical_section;

HANDLE createThread(LPTHREAD_START_ROUTINE func, char* thread_name) {
    DWORD thread_id = NULL;
    HANDLE thread = CreateThread(NULL, 0, func, thread_name, 0, &thread_id);

    if (thread == NULL)
        throw "[ERROR] CreateThread";

    return thread;
}

void WINAPI loop(char* displayed_name) {
    int tid = GetCurrentThreadId();

    for (int i = 1; i <= 90; ++i) {

        if (i == 30) {
            EnterCriticalSection(&critical_section);
        }

        printf("  <%s>\t    %d.  \tTID = %u\n", displayed_name, i, tid);

        if (i == 60) {
            LeaveCriticalSection(&critical_section);
        }
        if (i >= 60) {
            EnterCriticalSection(&critical_section);
            LeaveCriticalSection(&critical_section);
        }

        Sleep(100);
    }

    cout << "\n " << displayed_name << " is over" << "\n\n";
}

int main() {
    int tid = GetCurrentThreadId();
    const int size = 2;

    HANDLE threads[size];

    InitializeCriticalSection(&critical_section);

    threads[0] = createThread((LPTHREAD_START_ROUTINE)loop, (char*)"A");
    threads[1] = createThread((LPTHREAD_START_ROUTINE)loop, (char*)"B");

    loop((char*)"m");

    cout << "\nmain is over\n\n";
    WaitForMultipleObjects(size, threads, TRUE, INFINITE);
    for (int i = 0; i < size; ++i)
        CloseHandle(threads[i]);

    DeleteCriticalSection(&critical_section);
    return 0;
}
