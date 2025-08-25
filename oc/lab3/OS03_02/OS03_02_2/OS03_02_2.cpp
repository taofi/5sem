#include <iostream>
#include <windows.h>

int main() {
    DWORD processID = GetCurrentProcessId();

    for (int i = 0; i < 125; ++i) {
        std::cout << "OS03_02_2  (PID: " << processID << ",  " << i + 1 << ")" << std::endl;
        Sleep(1000); 
    }

    return 0;
}