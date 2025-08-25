#include <iostream>
#include <windows.h>
#include <thread>
#include <bitset>

using namespace std;

string GetPriorityClassName(DWORD priorityClass) {
    switch (priorityClass) {
    case IDLE_PRIORITY_CLASS: return "Idle";
    case BELOW_NORMAL_PRIORITY_CLASS: return "Below Normal";
    case NORMAL_PRIORITY_CLASS: return "Normal";
    case ABOVE_NORMAL_PRIORITY_CLASS: return "Above Normal";
    case HIGH_PRIORITY_CLASS: return "High";
    case REALTIME_PRIORITY_CLASS: return "Realtime";
    default: return "Unknown";
    }
}

string GetThreadPriorityName(int priority) {
    switch (priority) {
    case THREAD_PRIORITY_LOWEST: return "Lowest";
    case THREAD_PRIORITY_BELOW_NORMAL: return "Below Normal";
    case THREAD_PRIORITY_NORMAL: return "Normal";
    case THREAD_PRIORITY_ABOVE_NORMAL: return "Above Normal";
    case THREAD_PRIORITY_HIGHEST: return "Highest";
    case THREAD_PRIORITY_TIME_CRITICAL: return "Time Critical";
    default: return "Unknown";
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    DWORD processId = GetCurrentProcessId();
    HANDLE hProcess = GetCurrentProcess();
    DWORD threadId = GetCurrentThreadId();
    HANDLE hThread = GetCurrentThread();

    DWORD priorityClass = GetPriorityClass(hProcess);
    int threadPriority = GetThreadPriority(hThread);

    DWORD_PTR processAffinityMask, systemAffinityMask;
    GetProcessAffinityMask(hProcess, &processAffinityMask, &systemAffinityMask);

    int cpuCount = bitset<sizeof(processAffinityMask) * 8>(processAffinityMask).count();
    DWORD currentProcessor = GetCurrentProcessorNumber();

    cout << "Идентификатор процесса: " << processId << endl;
    cout << "Идентификатор потока: " << threadId << endl;
    cout << "Приоритет процесса: " << GetPriorityClassName(priorityClass) << endl;
    cout << "Приоритет потока: " << GetThreadPriorityName(threadPriority) << endl;

    cout << "Маска процессоров (двоичный вид): " << bitset<sizeof(processAffinityMask) * 8>(processAffinityMask) << endl;
    cout << "Количество доступных процессоров: " << cpuCount << endl;
    cout << "Назначенный текущему потоку процессор: " << currentProcessor << endl;

    CloseHandle(hProcess);
    CloseHandle(hThread);

    return 0;
}