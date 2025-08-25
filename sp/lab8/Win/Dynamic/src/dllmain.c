
#include "framework.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Mappingd.h"



int validPos = 1;

void createSampleFile(const char* filePath) {
    for (int i = 0; i < 10; i++) {
        snprintf(students[i].Name, sizeof(students[i].Name), "Student%d", i);
        snprintf(students[i].Surname, sizeof(students[i].Surname), "Surname%d", i);
        students[i].Course = (unsigned char)(i % 5 + 1);
        students[i].Group = (unsigned char)(i % 3 + 1);
        snprintf(students[i].ID, sizeof(students[i].ID), "%04d", i + 1);
    }

    hFile = CreateFileA(filePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file: %d\n", GetLastError());
        return;
    }

    DWORD bytesWritten;
    WriteFile(hFile, students, sizeof(struct Student) * MAX_STUDENTS, &bytesWritten, NULL);
    printf("Written %lu bytes to the file.\n", bytesWritten);

    CloseHandle(hFile);
}

void OpenMapping(const char* filePath) {
    hFile = CreateFileA(filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("error CreateFileA: %d\n", GetLastError());
        return;
    }

    hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
    if (!hFileMapping) {
        printf("Failed to create file mapping %d\n", GetLastError());
        CloseHandle(hFile);
        return;
    }

    pStudents = (struct Student*)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (!pStudents) {
        printf("Failed to map view of file %d\n", GetLastError());
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return;
    }
}

void AddRow(struct Student row, int pos) {
    if (pos < 0) {
        pos = pos + studentCount;
    }

    if (pos < 0 || pos >= studentCount) {
        printf("wrong position: %d\n", pos);
        return;
    }

    if (strlen(pStudents[pos].Name) != 0) {
        printf("Position %d is already busy.\n", pos);
        return;
    }

    pStudents[pos] = row;
}


int ChekPos(int pos) {
    if (pos < 0) {
        pos = pos + studentCount;
    }

    if (pos < 0 || pos >= studentCount) {
        printf("wrong position: %d\n", pos);
        validPos = 0;
        return 0;
    }

    if (strlen(pStudents[pos].Name) != 0) {
        printf("Position %d is already busy.\n", pos);
        validPos = 0;
        return 0;
    }

    return 1;
}

void RemRow(int pos) {
    if (pos < 0) {
        pos = pos + studentCount;
    }

    if (pos < 0 || pos >= studentCount) {
        printf("wrong position: %d\n", pos);
        return;
    }

    if (strlen(pStudents[pos].Name) == 0) {
        printf("Position %d is empty.\n", pos);
        return;
    }

    memset(&pStudents[pos], 0, sizeof(struct Student));
}

void PrintRow(int pos) {
    if (pos < 0) {
        pos = pos + studentCount;
    }

    if (pos < 0 || pos >= studentCount) {
        printf("wrong position: %d\n", pos);
        return;
    }

    if (strlen(pStudents[pos].Name) == 0) {
        printf("Position %d is empty.\n", pos);
        return;
    }

    struct Student student = pStudents[pos];
    printf("Name: %s, LastName: %s, Course: %d, Group: %d, ID: %s\n",
        student.Name, student.Surname, student.Course, student.Group, student.ID);
}

void PrintRows() {


    for (int i = 0; i < studentCount; i++) {
        if (strlen(pStudents[i].Name) != 0) {
            PrintRow(i);
        }
    }
}


int getValidInput(const char* prompt, int* value) {
    printf("%s", prompt);
    if (scanf_s("%d", value) == 1) {
        return 1;
    }
    else {
        printf("Please enter a valid number.\n");
        while (getchar() != '\n');
        return 0;
    }
}


void CloseMapping() {
    UnmapViewOfFile(pStudents);
    CloseHandle(hFileMapping);
    hFileMapping = NULL;
    CloseHandle(hFile);
}

struct Student* GetPStudents() {
    return pStudents;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

