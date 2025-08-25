#include <windows.h>
#include <stdio.h>

int main() {
    char systemDir[MAX_PATH];
    char windowsDir[MAX_PATH];

    // ��������� ���� � ���������� ��������
    if (GetSystemDirectoryA(systemDir, MAX_PATH)) {
        printf("Lab-02hw-3.exe: %s\n", systemDir);
    }
    else {
        fprintf(stderr, "Error\n");
    }

    // ��������� ���� � ��������� �������� Windows
    if (GetWindowsDirectoryA(windowsDir, MAX_PATH)) {
        printf("Lab-02hw-4.exe: %s\n", windowsDir);
    }
    else {
        fprintf(stderr, "Error\n");
    }

    system("pause");
    return 0;
}
