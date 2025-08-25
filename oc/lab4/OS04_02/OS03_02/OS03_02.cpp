#include <iostream>
#include <windows.h>
#include <thread>

void OS04_02_T1() {
    DWORD threadId = GetCurrentThreadId();
    DWORD processId = GetCurrentProcessId();
    for (int i = 0; i < 50; ++i) {
        std::cout << "Thread OS04_02_T1: Process ID = " << processId << ", Thread ID = " << threadId << ", Iteration = " << i + 1 << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void OS04_02_T2() {
    DWORD threadId = GetCurrentThreadId();
    DWORD processId = GetCurrentProcessId();
    for (int i = 0; i < 125; ++i) {
        std::cout << "Thread OS04_02_T2: Process ID = " << processId << ", Thread ID = " << threadId << ", Iteration = " << i + 1 << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    DWORD processId = GetCurrentProcessId();

    // Запуск потоков
    std::thread thread1(OS04_02_T1);
    std::thread thread2(OS04_02_T2);

    // Основной процесс
    for (int i = 0; i < 100; ++i) {
        std::cout << "Main Process OS04_02: Process ID = " << processId << ", Iteration = " << i + 1 << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Ожидание завершения потоков
    thread1.join();
    thread2.join();

    return 0;
}
