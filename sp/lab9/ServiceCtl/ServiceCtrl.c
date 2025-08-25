#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

void CreateServiceHandler(const wchar_t* serviceName, const wchar_t* exePath);
void StartServiceHandler(const wchar_t* serviceName, const wchar_t* watchDir, const wchar_t* logDir);
void StopServiceHandler(const wchar_t* serviceName);
void DeleteServiceHandler(const wchar_t* serviceName);
void PauseServiceHandler(const wchar_t* serviceName);
void ContinueServiceHandler(const wchar_t* serviceName);
void InfoServiceHandler(const wchar_t* serviceName);

int wmain(int argc, wchar_t* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    if (argc < 3) {
        fprintf(stdout, "Использование: ServiceCtl <Имя Сервиса> <Операция> [Дополнительные параметры]\n");
        return 1;
    }

    const wchar_t* serviceName = argv[1];
    const wchar_t* operation = argv[2];

    if (_wcsicmp(operation, L"Create") == 0 && argc >= 4) {
        CreateServiceHandler(serviceName, argv[3]);
    }
    else if (_wcsicmp(operation, L"Start") == 0 && argc >= 5) {
        StartServiceHandler(serviceName, argv[3], argv[4]);
    }
    else if (_wcsicmp(operation, L"Stop") == 0) {
        StopServiceHandler(serviceName);
    }
    else if (_wcsicmp(operation, L"Delete") == 0) {
        DeleteServiceHandler(serviceName);
    }
    else if (_wcsicmp(operation, L"Pause") == 0) {
        PauseServiceHandler(serviceName);
    }
    else if (_wcsicmp(operation, L"Continue") == 0) {
        ContinueServiceHandler(serviceName);
    }
    else if (_wcsicmp(operation, L"Info") == 0) {
        InfoServiceHandler(serviceName);
    }
    else {
        fprintf(stdout, "Неизвестная операция: %ws\n", operation);
        return 1;
    }
    return 0;
}

void CreateServiceHandler(const wchar_t* serviceName, const wchar_t* exePath) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!hSCManager) {
        fprintf(stdout, "Не удалось открыть диспетчер управления сервисами\n");
        return;
    }

    SC_HANDLE hService = CreateServiceW(
        hSCManager,
        serviceName,//vnutr name service(SCM)
        serviceName,//vneshn name service
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL,
        exePath,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    );

    if (!hService) {
        fprintf(stdout, "Не удалось создать сервис\n");
    }
    else {
        fprintf(stdout, "Сервис успешно создан\n");
        CloseServiceHandle(hService);
    }

    CloseServiceHandle(hSCManager);
}

void StartServiceHandler(const wchar_t* serviceName, const wchar_t* watchDir, const wchar_t* logDir) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        fprintf(stdout, "Не удалось открыть диспетчер управления сервисами\n");
        return;
    }
    SC_HANDLE hService = OpenServiceW(hSCManager, serviceName, SERVICE_ALL_ACCESS);
    if (!hService) {
        fprintf(stdout, "Не удалось открыть сервис\n, %lu", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }


    const wchar_t* params[] = { logDir, watchDir };
    if (!StartServiceW(hService, 2, params)) {
        fprintf(stdout, "Не удалось запустить сервис\n");
        CloseServiceHandle(hSCManager);
        CloseServiceHandle(hService);
        return;
    }

    fprintf(stdout, "Сервис успешно запущен\n");
    CloseServiceHandle(hSCManager);
    CloseServiceHandle(hService);
}

void StopServiceHandler(const wchar_t* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        fprintf(stdout, "Не удалось открыть диспетчер управления сервисами\n");
        return;
    }

    SC_HANDLE hService = OpenServiceW(hSCManager, serviceName, SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (!hService) {
        fprintf(stdout, "Не удалось открыть сервис\n");
        CloseServiceHandle(hSCManager);
        return;
    }

    SERVICE_STATUS serviceStatus;
    if (ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus)) {
        fprintf(stdout, "Сервис успешно остановлен\n");
    }
    else {
        fprintf(stdout, "Не удалось остановить сервис\n");
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

void DeleteServiceHandler(const wchar_t* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        fprintf(stdout, "Не удалось открыть диспетчер управления сервисами\n");
        return;
    }

    SC_HANDLE hService = OpenServiceW(hSCManager, serviceName, DELETE);
    if (!hService) {
        fprintf(stdout, "Не удалось открыть сервис\n");
        CloseServiceHandle(hSCManager);
        return;
    }

    if (DeleteService(hService)) {
        fprintf(stdout, "Сервис успешно удален\n");
    }
    else {
        fprintf(stdout, "Не удалось удалить сервис\n");
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

void PauseServiceHandler(const wchar_t* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        fprintf(stdout, "Не удалось открыть диспетчер управления сервисами\n");
        return;
    }

    SC_HANDLE hService = OpenServiceW(hSCManager, serviceName, SERVICE_PAUSE_CONTINUE | SERVICE_QUERY_STATUS);
    if (!hService) {
        fprintf(stdout, "Не удалось открыть сервис\n");
        CloseServiceHandle(hSCManager);
        return;
    }

    SERVICE_STATUS serviceStatus;
    if (ControlService(hService, SERVICE_CONTROL_PAUSE, &serviceStatus)) {
        fprintf(stdout, "Сервис успешно приостановлен\n");
    }
    else {
        fprintf(stdout, "Не удалось приостановить сервис\n");
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

void ContinueServiceHandler(const wchar_t* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        fprintf(stdout, "Не удалось открыть диспетчер управления сервисами\n");
        return;
    }

    SC_HANDLE hService = OpenServiceW(hSCManager, serviceName, SERVICE_PAUSE_CONTINUE | SERVICE_QUERY_STATUS);
    if (!hService) {
        fprintf(stdout, "Не удалось открыть сервис\n");
        CloseServiceHandle(hSCManager);
        return;
    }

    SERVICE_STATUS serviceStatus;
    if (ControlService(hService, SERVICE_CONTROL_CONTINUE, &serviceStatus)) {
        fprintf(stdout, "Сервис успешно возобновлен\n");
    }
    else {
        fprintf(stdout, "Не удалось возобновить сервис\n");
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}

void InfoServiceHandler(const wchar_t* serviceName) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        fprintf(stdout, "Не удалось открыть диспетчер управления сервисами\n");
        return;
    }

    SC_HANDLE hService = OpenServiceW(hSCManager, serviceName, SERVICE_QUERY_STATUS);
    if (!hService) {
        fprintf(stdout, "Не удалось открыть сервис\n");
        CloseServiceHandle(hSCManager);
        return;
    }

    SERVICE_STATUS serviceStatus;
    if (QueryServiceStatus(hService, &serviceStatus)) {
        fprintf(stdout, "Статус сервиса:\n");
        fprintf(stdout, "  Тип сервиса: %lu\n", serviceStatus.dwServiceType);
        fprintf(stdout, "  Текущее состояние: %lu\n", serviceStatus.dwCurrentState);
        fprintf(stdout, "  Принимаемые команды: %lu\n", serviceStatus.dwControlsAccepted);
        fprintf(stdout, "  Код завершения Win32: %lu\n", serviceStatus.dwWin32ExitCode);
        fprintf(stdout, "  Специфический код завершения: %lu\n", serviceStatus.dwServiceSpecificExitCode);
        fprintf(stdout, "  Контрольная точка: %lu\n", serviceStatus.dwCheckPoint);
        fprintf(stdout, "  Время ожидания: %lu\n", serviceStatus.dwWaitHint);
    }
    else {
        fprintf(stdout, "Не удалось получить статус сервиса\n");
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
}
