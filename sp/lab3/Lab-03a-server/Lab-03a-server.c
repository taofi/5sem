#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>

#define MAX_PROCESSES 10

void convertToWideChar(const char* input, wchar_t* output, int outputSize) {
    MultiByteToWideChar(CP_UTF8, 0, input, -1, output, outputSize);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");

    printf("Main process: start\n");

    // Parse args
    if (argc < 4) {
        printf("Usage: %s <process count> <lower bound> <upper bound> [mutex name]\n", argv[0]);
        return 1;
    }

    /*argc = 5;
    argv[1] = (char*)"5";
    argv[2] = (char*)"1";
    argv[3] = (char*)"1000";
    argv[4] = (char*)"Global\\DefaultMutex";*/

    int process_count = atoi(argv[1]);
    int lower_bound = atoi(argv[2]);
    int upper_bound = atoi(argv[3]);
    
    const char* defaultMutexName = "DefaultMutex";
    const char* mutex_name = defaultMutexName;

    wchar_t wideMutexName[256];

    if (argc > 4) {
        const char* mutex_name = argv[4];
        convertToWideChar(mutex_name, wideMutexName, sizeof(wideMutexName) / sizeof(wideMutexName[0]));
    }
    else {
        char buffer[256];
        DWORD bufferSize = sizeof(buffer);
        DWORD result = GetEnvironmentVariableA("MUTEX_NAME", buffer, bufferSize);

        
        if (result != 0) {  
            convertToWideChar(buffer, wideMutexName, sizeof(wideMutexName) / sizeof(wideMutexName[0]));
        }
    }


    // Create pipe
    HANDLE hWritePipe, hReadPipe;
    SECURITY_ATTRIBUTES sa;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;  // защита по умолчанию
    sa.bInheritHandle = TRUE;        // дескрипторы наследуемые

    if (!CreatePipe(
        &hReadPipe,  // дескриптор для чтения
        &hWritePipe, // дескриптор для записи
        &sa,   // атрибуты защиты по умолчанию, дескрипторы наследуемые
        0))    // размер буфера по умолчанию
    {
        printf("Create pipe failed.\n");
        return GetLastError();
    }

    // Create mutex
    HANDLE  hMutex;

    hMutex = CreateMutex(NULL, FALSE, wideMutexName);
    if (hMutex == NULL)
    {
        printf("Create mutex error");
        return GetLastError();
    }

    // Create processes
    HANDLE childProcesses[MAX_PROCESSES];
    int processCount = 0;

    for (size_t i = 0; i < process_count; i++) {
        int rangeSize = (upper_bound - lower_bound + 1) / process_count;
        int start = lower_bound + i * rangeSize;
        int end = (i == process_count - 1) ? upper_bound : (start + rangeSize - 1);

        wchar_t command_line[128];

        wsprintf(command_line, L"..\\Lab-03a-client\\Lab-03a-client %d %d %s", start, end, wideMutexName);

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.dwFlags = STARTF_USESTDHANDLES;
        si.cb = sizeof(STARTUPINFO);

        si.hStdInput = hReadPipe;
        si.hStdOutput = hWritePipe;
        si.hStdError = hWritePipe;

        if (!CreateProcess(
            NULL,
            command_line,
            NULL,
            NULL,
            TRUE,
            CREATE_NEW_CONSOLE,
            NULL,
            NULL,
            &si,
            &pi)) {
            printf("Create process error\n");
            return GetLastError();
        }

        childProcesses[processCount++] = pi.hProcess;
        CloseHandle(pi.hThread);
    }

    // Output
    for (int i = 0; i < processCount;)
    {
        char buff[1024];
        DWORD dwBytesRead;
        DWORD bytesAvailable = 0;
        if (PeekNamedPipe(hReadPipe, NULL, 0, NULL, &bytesAvailable, NULL) && bytesAvailable > 0) {
            if (!ReadFile(hReadPipe, buff, sizeof(buff), &dwBytesRead, NULL)) {
                printf("Read data error");
                return GetLastError();
            }
            buff[dwBytesRead] = '\0';
            printf("%s\n\n", buff);
            i++;
        }

    }

    for (int i = 0; i < processCount; i++) {
        CloseHandle(childProcesses[i]);
    }
    system("pause");

    CloseHandle(hMutex);

    return 0;
}