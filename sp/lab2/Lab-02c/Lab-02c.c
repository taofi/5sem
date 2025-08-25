#include <stdio.h>
#include <stdlib.h>
#include <wtypes.h>
#include <processthreadsapi.h>
#include <WinBase.h>
#include <windows.h>


void StartProcess(const wchar_t* processName) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessW(NULL, (LPSTR)processName, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("StartProcess failed (%d).\n", GetLastError());
    }
    else {
        printf("%ls\n", processName);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    SetCurrentDirectoryW(L"C:\\");
    wchar_t currentDir[256];
    GetCurrentDirectory(256, currentDir);
    wprintf(L"Current directory: %ls\n", currentDir);
    wchar_t name[] = L"Lab-02hw-0.exe";

    for (int i = 0; i < 5; i++) {
        name[9] = i + 1 + '0';
        StartProcess(name);
    }

    system("pause");
    return 0;
}
