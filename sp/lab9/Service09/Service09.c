#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>

void WINAPI ServiceMain(DWORD, LPWSTR*);
void WINAPI ServiceCtrlHandler(DWORD);
void WatchDirectory(const wchar_t*);
void PrintLog(HANDLE, wchar_t* message);
void GetTimestamp(wchar_t* buffer, size_t bufferSize);
HANDLE CreateLogFile(wchar_t* dir, wchar_t* prefix);

#define SERVICE_NAME (wchar_t*)L"Service09"

SERVICE_STATUS_HANDLE hServiceStatus;
SERVICE_STATUS serviceStatus;
HANDLE hLog = INVALID_HANDLE_VALUE;
HANDLE hState = INVALID_HANDLE_VALUE;
wchar_t logBuffer[4096] = L"";
size_t logBufferIndex = 0;

void AddToLogBuffer(wchar_t* message) {
    if (logBufferIndex < sizeof(logBuffer) / sizeof(wchar_t) - 1) {
        wcscat(logBuffer, message);
        wcscat(logBuffer, L"\n");
        logBufferIndex += wcslen(message) + 1;
    }
}

int wmain(int argc, wchar_t* argv[]) {
    SERVICE_TABLE_ENTRYW serviceTable[] = {
        { SERVICE_NAME, ServiceMain },
        { NULL, NULL }
    };

    if (!StartServiceCtrlDispatcherW(serviceTable)) {
        return 0;
    }

    return 0;
}

void WINAPI ServiceMain(DWORD dwArgc, LPWSTR* lpszArgv) {
    hServiceStatus = RegisterServiceCtrlHandlerW(
        SERVICE_NAME,
        ServiceCtrlHandler
    );

    if (!hServiceStatus) {
        return;
    }

    serviceStatus.dwCurrentState = SERVICE_START_PENDING;
    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
    serviceStatus.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint = 0;
    serviceStatus.dwWaitHint = 5000;

    SetServiceStatus(hServiceStatus, &serviceStatus);

    if (dwArgc < 3) {
        AddToLogBuffer((wchar_t*)L"Попытка выполнения операции сервисом провалена! Недостаточно аргументов.");
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hServiceStatus, &serviceStatus);
        return;
    }

    DWORD attributes = GetFileAttributesW(lpszArgv[1]);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        if (!CreateDirectoryW(lpszArgv[1], NULL)) {
            AddToLogBuffer((wchar_t*)L"Попытка выполнения операции сервисом провалена! Не удалось создать каталог для лог-файлов");
            serviceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(hServiceStatus, &serviceStatus);
            return;
        }
        else {
            wchar_t message[MAX_PATH];
            swprintf(message, MAX_PATH, L"Успех! %s создал каталог %s", SERVICE_NAME, lpszArgv[1]);
            AddToLogBuffer(message);
        }
    }
    else if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
        wchar_t message[MAX_PATH];
        swprintf(message, MAX_PATH, L"Успех! %s обнаружил каталог %s", SERVICE_NAME, lpszArgv[1]);
        AddToLogBuffer(message);
    }
    else {
        AddToLogBuffer((wchar_t*)L"Попытка выполнения операции сервисом провалена! Не удалось создать каталог для лог-файлов");
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hServiceStatus, &serviceStatus);
        return;
    }

    hLog = CreateLogFile(lpszArgv[1], L"dir");
    hState = CreateLogFile(lpszArgv[1], L"srv");

    if (hLog == INVALID_HANDLE_VALUE || hState == INVALID_HANDLE_VALUE) {
        AddToLogBuffer((wchar_t*)L"Попытка выполнения операции сервисом провалена! Не удалось создать log файлы");
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hServiceStatus, &serviceStatus);
        return;
    }

    if (wcslen(logBuffer) > 0) {
        PrintLog(hState, logBuffer);
    }

    attributes = GetFileAttributesW(lpszArgv[2]);
    if (attributes == INVALID_FILE_ATTRIBUTES || !(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        wchar_t message[MAX_PATH];
        swprintf(message, MAX_PATH, L"Попытка запуска провалена! %s не смог обнаружить каталог %s", SERVICE_NAME, lpszArgv[2]);
        PrintLog(hState, message);
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hServiceStatus, &serviceStatus);
        return;
    }

    wchar_t message[MAX_PATH];
    swprintf(message, MAX_PATH, L"Успех! Сервис %s запущен с параметрами %s %s", SERVICE_NAME, lpszArgv[1], lpszArgv[2]);
    PrintLog(hState, message);

    serviceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hServiceStatus, &serviceStatus);

    SetCurrentDirectoryW(lpszArgv[2]);
    WatchDirectory(lpszArgv[2]);
}

void WINAPI ServiceCtrlHandler(DWORD dwControl) {
    wchar_t message[MAX_PATH];
    switch (dwControl) {
    case SERVICE_CONTROL_STOP:
        swprintf(message, MAX_PATH, L"Успех! Сервис %s сменил состояние с %s на %s", SERVICE_NAME, L"Запущен", L"Остановлен");
        PrintLog(hState, message);

        CloseHandle(hLog);
        CloseHandle(hState);

        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hServiceStatus, &serviceStatus);
        break;
    case SERVICE_CONTROL_PAUSE:
        if (serviceStatus.dwCurrentState == SERVICE_RUNNING) {
            swprintf(message, MAX_PATH, L"Успех! Сервис %s сменил состояние с %s на %s", SERVICE_NAME, L"Запущен", L"Пауза");
            PrintLog(hState, message);
            serviceStatus.dwCurrentState = SERVICE_PAUSED;
            SetServiceStatus(hServiceStatus, &serviceStatus);
        }
        else {
            swprintf(message, MAX_PATH, L"Попытка выполнения сервисом операции провалена! Сервис %s не может сменить состояние с %s на %s", SERVICE_NAME, L"Пауза", L"Пауза");
            PrintLog(hState, message);
        }
        break;
    case SERVICE_CONTROL_CONTINUE:
        if (serviceStatus.dwCurrentState == SERVICE_PAUSED) {
            swprintf(message, MAX_PATH, L"Успех! Сервис %s сменил состояние с %s на %s", SERVICE_NAME, L"Пауза", L"Запущен");
            PrintLog(hState, message);

            serviceStatus.dwCurrentState = SERVICE_RUNNING;
            SetServiceStatus(hServiceStatus, &serviceStatus);
        }
        else {
            swprintf(message, MAX_PATH, L"Попытка выполнения сервисом операции провалена! Сервис %s не может сменить состояние с %s на %s", SERVICE_NAME, L"Запущен", L"Запущен");
            PrintLog(hState, message);
        }
        break;
    }
}

HANDLE CreateLogFile(wchar_t* dir, wchar_t* prefix) {
    wchar_t logPath[MAX_PATH];
    wchar_t timestamp[64];
    GetTimestamp(timestamp, sizeof(timestamp) / sizeof(wchar_t));
    swprintf(logPath, MAX_PATH, L"%s\\%s-%s.log", dir, timestamp, prefix);

    HANDLE hFile = CreateFileW(
        logPath,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, // Разрешить разделяемое чтение и запись
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );


    if (hFile == INVALID_HANDLE_VALUE) {
        wchar_t message[MAX_PATH];
        swprintf(message, MAX_PATH, L"Попытка выполнения операции сервисом провалена! Не удалось создать %s.log", prefix);
        AddToLogBuffer(message);
    }
    else {
        wchar_t message[MAX_PATH];
        swprintf(message, MAX_PATH, L"Успех! Создан файл %s", logPath);
        AddToLogBuffer(message);
    }

    return hFile;
}

void PrintLog(HANDLE hFile, wchar_t* message) {
    if (hFile == INVALID_HANDLE_VALUE) return;

    wchar_t timestamp[64];
    GetTimestamp(timestamp, sizeof(timestamp) / sizeof(wchar_t));

    wchar_t buffer[2048];
    swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%s %s\n", timestamp, message);

    DWORD bytesWritten;
    WriteFile(hFile, buffer, wcslen(buffer) * sizeof(wchar_t), &bytesWritten, NULL);
}

void GetTimestamp(wchar_t* buffer, size_t bufferSize) {
    time_t now = time(NULL);
    struct tm tm;
    localtime_s(&tm, &now);
    wcsftime(buffer, bufferSize, L"%Y-%m-%d %H.%M.%S", &tm);
}

void WatchDirectory(const wchar_t* directoryPath) {
    HANDLE hDir = CreateFileW(
        directoryPath,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        PrintLog(hState, L"Попытка выполнения операции сервисом провалена! Не удалось открыть директорию для отслеживания событий");
        return;
    }

    char buffer[1024];
    DWORD bytesReturned;
    FILE_NOTIFY_INFORMATION* notifyInfo;

    while (1) {
        if (ReadDirectoryChangesW(hDir, buffer, sizeof(buffer), TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
            FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |
            FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION |
            FILE_NOTIFY_CHANGE_LAST_ACCESS | FILE_NOTIFY_CHANGE_SECURITY,
            &bytesReturned, NULL, NULL)) {

            notifyInfo = (FILE_NOTIFY_INFORMATION*)buffer;

            while (1) {
                wchar_t fileName[MAX_PATH];
                wcsncpy(fileName, notifyInfo->FileName, notifyInfo->FileNameLength / sizeof(WCHAR));
                fileName[notifyInfo->FileNameLength / sizeof(WCHAR)] = L'\0';

                wchar_t logMessage[MAX_PATH];

                switch (notifyInfo->Action) {
                case FILE_ACTION_ADDED:
                    swprintf(logMessage, MAX_PATH, L"FILE_ACTION_ADDED: %s\n", fileName);
                    PrintLog(hLog, logMessage);
                    break;
                case FILE_ACTION_REMOVED:
                    swprintf(logMessage, MAX_PATH, L"FILE_ACTION_REMOVED: %s\n", fileName);
                    PrintLog(hLog, logMessage);
                    break;
                case FILE_ACTION_MODIFIED:
                    swprintf(logMessage, MAX_PATH, L"FILE_ACTION_MODIFIED: %s\n", fileName);
                    PrintLog(hLog, logMessage);
                    break;
                case FILE_ACTION_RENAMED_OLD_NAME:
                    swprintf(logMessage, MAX_PATH, L"FILE_ACTION_RENAMED_OLD_NAME: %s\n", fileName);
                    PrintLog(hLog, logMessage);
                    break;
                case FILE_ACTION_RENAMED_NEW_NAME:
                    swprintf(logMessage, MAX_PATH, L"FILE_ACTION_RENAMED_NEW_NAME: %s\n", fileName);
                    PrintLog(hLog, logMessage);
                    break;
                }

                if (notifyInfo->NextEntryOffset == 0) {
                    break;
                }

                notifyInfo = (FILE_NOTIFY_INFORMATION*)((char*)notifyInfo + notifyInfo->NextEntryOffset);
            }
        }
        else {
            PrintLog(hState, L"Попытка выполнения операции сервисом провалена! Не удалось считать изменения в директории");
        }
    }

    CloseHandle(hDir);
}

