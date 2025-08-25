#include <iostream>
#include <windows.h>

int main() {
    DWORD processId = GetCurrentProcessId();  // Получение ID процесса
    DWORD threadId = GetCurrentThreadId();    // Получение ID потока

    for (int i = 0; i < 1000; i++) {
        std::cout << "Iteration: " << i + 1 <<  " Process ID: " << processId << " Thread ID: " << threadId << std::endl;
        Sleep(1000); 
    }

    return 0;
}
