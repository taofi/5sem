#include <iostream>
#include <windows.h>

int main() {
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();
    for (int i = 0; i < 50; ++i) {
        std::cout << "Iteration: " << i + 1 << " Process ID: " << processId << " Thread ID: " << threadId << std::endl;
        Sleep(1000); 
    }

    return 0;
}