#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    DWORD pageSize = sysInfo.dwPageSize;
    printf("Page size: %lu bytes\n", pageSize);

    size_t numPages = 256;
    size_t reserveSize = numPages * pageSize;
    LPVOID reservedMemory = VirtualAlloc(NULL, reserveSize, MEM_RESERVE, PAGE_NOACCESS);

    if (reservedMemory == NULL) {
        printf("error: %lu\n", GetLastError());
        return 1;
    }
    printf("1 address of virtual memory: %p\n", reservedMemory);
    system("pause");

    LPVOID committedMemory = VirtualAlloc((LPVOID)((size_t)reservedMemory + (numPages / 2) * pageSize), numPages / 2 * pageSize, MEM_COMMIT, PAGE_READWRITE);

    if (committedMemory == NULL) {
        printf("error: %lu\n", GetLastError());
        VirtualFree(reservedMemory, 0, MEM_RELEASE);
        return 1;
    }
    printf("2 address of commited memory: %p\n", committedMemory);
    system("pause");


    int* intArray = (int*)committedMemory;
    for (size_t i = 0; i < (numPages / 2) * (pageSize / sizeof(int)); i++) {
        intArray[i] = (int)i;
    }
    printf("3 commited memory is full of integers.\n");
    system("pause");


    DWORD oldProtect;
    if (VirtualProtect(committedMemory, numPages / 2 * pageSize, PAGE_READONLY, &oldProtect) == 0) {
        printf("error: %lu\n", GetLastError());
        VirtualFree(reservedMemory, 0, MEM_RELEASE);
        return 1;
    }
    printf("4 readonly protection\n");
    system("pause");


    if (VirtualFree(committedMemory, numPages / 2 * pageSize, MEM_DECOMMIT) == 0) {
        printf("error: %lu\n", GetLastError());
        VirtualFree(reservedMemory, 0, MEM_RELEASE);
        return 1;
    }
    printf("5 commited memory was freed\n");
    system("pause");


    if (VirtualFree(reservedMemory, 0, MEM_RELEASE) == 0) {
        printf("error: %lu\n", GetLastError());
        return 1;
    }
    system("pause");

    return 0;
}