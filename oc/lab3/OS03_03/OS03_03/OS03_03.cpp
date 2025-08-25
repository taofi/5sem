#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>

void ListRunningProcesses() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Error while creating process snapshot." << std::endl;
        return;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        std::cerr << "Error getting information about the first process." << std::endl;
        CloseHandle(hProcessSnap);
        return;
    }

    do {
        _tprintf(TEXT("\nProcess: %s (PID: %u)"), pe32.szExeFile, pe32.th32ProcessID);
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
}

int main() {
    ListRunningProcesses();
    system("pause");
    return 0;
}
