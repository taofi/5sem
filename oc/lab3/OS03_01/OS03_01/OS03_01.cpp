#include <iostream>
#include <windows.h>

int main() {
    DWORD processID = GetCurrentProcessId();

    while (true) {
        std::cout << "PID: " << processID << "" << std::endl;
        Sleep(1000);
    }

    return 0; 
}