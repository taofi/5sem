#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <wtypes.h>
#include <processthreadsapi.h>
#include <WinBase.h>
#include <windows.h>


void CreateChildProcess(const wchar_t* appName, const wchar_t* cmdLine, STARTUPINFO si, PROCESS_INFORMATION pi) {

    if (!CreateProcess(appName, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%d).\n", GetLastError());
    }
}

int main() {
    wchar_t currentDir[256];
    GetCurrentDirectory(256, currentDir);
    wprintf(L"Current directory: %ls\n", currentDir);


    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    wchar_t path[256] = L"..\\Lab-02x\\Lab-02x.exe";
    wchar_t buff[256] = L"";
    wchar_t buff1[256] = L"";
    wcscpy(buff, path);
    wchar_t path2[256] = L"..\\Lab-02x\\Lab-02x.exe 100";

    STARTUPINFO si1 = { sizeof(si1) };
    STARTUPINFO si2 = { sizeof(si2) };
    STARTUPINFO si3 = { sizeof(si3) };
    PROCESS_INFORMATION pi1, pi2, pi3;


    if (GetFileAttributes(path) == INVALID_FILE_ATTRIBUTES) {
        printf("File not found: %ls\n", path);
    }


    CreateChildProcess(path2, NULL, si1, pi1);

    CreateChildProcess(NULL, wcscat(buff, L" 90"), si2, pi2);

    CreateChildProcess(path, wcscat(buff1, L" 100"), si3, pi3);

    HANDLE processes[2] = { pi2.hThread, pi3.hThread };
    WaitForMultipleObjects(2, processes, TRUE, INFINITE);

    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);

    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    CloseHandle(pi3.hProcess);
    CloseHandle(pi3.hThread);

    return 0;
}