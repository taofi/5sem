#include <stdio.h>
#include <windows.h>

void StartChildProcess(wchar_t* processName, PROCESS_INFORMATION* pi) {
    STARTUPINFO si;

    ZeroMemory(&si, sizeof(si));//заполняет стуктуру 0 для инициализации
    si.cb = sizeof(si); //размер стуктуры в байтах 
    ZeroMemory(pi, sizeof(PROCESS_INFORMATION));

    // Создаем дочерний процесс 
    if (!CreateProcess(
        NULL,            // Нет имени модуля
        processName,     // Командная строка (изменяемая строка)
        NULL,            // Защита процесса
        NULL,            // Защита потока
        FALSE,           // Унаследование дескрипторов
        CREATE_NEW_CONSOLE, // Флаг создания новой консоли
        NULL,            // Использование переменных окружения родителя
        NULL,            // Использование текущей директории
        &si,             // Структура STARTUPINFO
        pi)) {           // Структура PROCESS_INFORMATION
        printf("CreateProcess failed (%lu).\n", GetLastError());
        return;
    }
}

int main() {
    DWORD processId = GetCurrentProcessId();

    printf("Main Process OS03_02. Process ID: %lu\n", processId);

    wchar_t processName1[] = L"OS03_02_1.exe";
    wchar_t processName2[] = L"OS03_02_1.exe";

    PROCESS_INFORMATION pi1, pi2;

    StartChildProcess(processName1, &pi1);
    StartChildProcess(processName2, &pi2);

    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    for (int i = 0; i < 100; i++) {
        printf("Main Process Iteration: %d\n", i + 1);
        Sleep(1000);
    }

    printf("Main Process OS03_02 finished.\n");
    return 0;
}