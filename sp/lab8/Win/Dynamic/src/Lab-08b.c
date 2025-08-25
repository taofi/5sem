#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "D:\5sem\sp\lab8\Win\Dynamic\include\str.h"

typedef void (*CreateSampleFileType)(const char*);
typedef void (*OpenMappingType)(const char*);
typedef void (*AddRowType)(struct Student, int);
typedef int (*ChekPosType)(int);
typedef void (*RemRowType)(int);
typedef void (*PrintRowType)(int);
typedef void (*PrintRowsType)();
typedef void (*CloseMappingType)();
typedef int (*GetValidInputType)(const char*, int*);
typedef void (*SetSharedStudentsType)(struct Student*);
typedef struct Student* (*GetPStudentsType)();

int main() {
    HMODULE hLib = LoadLibraryA("Mappingd.dll");
    HMODULE hLib2 = LoadLibraryA("Mappingd-2.dll");
    if (!hLib || !hLib2) {
        printf("Failed to load dll: %lu\n", GetLastError());
        return 1;
    }

    CreateSampleFileType createSampleFile = (CreateSampleFileType)GetProcAddress(hLib, "createSampleFile");
    OpenMappingType openMapping = (OpenMappingType)GetProcAddress(hLib, (LPCSTR)MAKEINTRESOURCE(1));
    AddRowType addRow = (AddRowType)GetProcAddress(hLib, "AddRow");
    ChekPosType chekPos = (ChekPosType)GetProcAddress(hLib, "ChekPos");
    RemRowType remRow = (RemRowType)GetProcAddress(hLib, "RemRow");
    PrintRowType printRow = (PrintRowType)GetProcAddress(hLib, "PrintRow");
    PrintRowsType printRows = (PrintRowsType)GetProcAddress(hLib, "PrintRows");
    GetPStudentsType getPStudents = (GetPStudentsType)GetProcAddress(hLib, "GetPStudents");

    CloseMappingType closeMapping = (CloseMappingType)GetProcAddress(hLib2, "CloseMapping");
    GetValidInputType getValidInput = (GetValidInputType)GetProcAddress(hLib2, "getValidInput");
    SetSharedStudentsType setSharedStudents = (SetSharedStudentsType)GetProcAddress(hLib2, "SetSharedStudents");
    if (!setSharedStudents) {
        printf("Failed to get function SetSharedStudents: %lu\n", GetLastError());
        FreeLibrary(hLib2);
        return 1;
    }

    if (!createSampleFile || !openMapping || !addRow || !remRow || !printRow || !printRows || !closeMapping || !getValidInput || !setSharedStudents) {
        printf("Failed to get function address from Mappingd.dll: %lu\n", GetLastError());
        FreeLibrary(hLib);
        FreeLibrary(hLib2);
        return 1;
    }

    char filePath[260];
    int pos;
    struct Student student;
    int open = 0;

    printf("Enter a filePath: ");
    scanf("%s", filePath);

    int choice;
    do {
        system("pause");

        system("cls");
        if (!getValidInput("\n1. Open file \n 2. Add student \n 3. Rem student \n 4. Print row \n 5. Print file \n 6. Close file \n 7. Exit\n----------------------------------------------------------------------------------------------------------------\nchoice: ",
            &choice)) {
            continue;
        }

        switch (choice) {
        case 1: {
            if (open) {
                printf("File is already open. Close it first.\n");
                break;
            }
            createSampleFile(filePath);
            openMapping(filePath);
            struct Student* pStudentsPtr = getPStudents();
            if (pStudentsPtr == NULL) {
                printf("Failed to get pStudents pointer.\n");
                break;
            }
            open = 1;
            setSharedStudents(pStudentsPtr);
            break;
        }
        case 2:
            if (!open) {
                printf("File is not open\n");
                break;
            }
            printf("Position: ");
            scanf("%d", &pos);
            if (!chekPos(pos)) {
                break;
            }

            do {
                printf("Name: ");
                scanf("%s", student.Name);
                int valid = 1;
                for (int i = 0; student.Name[i] != '\0'; i++) {
                    if (!isalpha(student.Name[i])) {
                        valid = 0;
                        break;
                    }
                }
                if (!valid) {
                    printf("Name must contain only letters. Try again.\n");
                }
                else {
                    break;
                }
            } while (1);

            do {
                printf("Last name: ");
                scanf("%s", student.Surname);
                int valid = 1;
                for (int i = 0; student.Surname[i] != '\0'; i++) {
                    if (!isalpha(student.Surname[i])) {
                        valid = 0;
                        break;
                    }
                }
                if (!valid) {
                    printf("Last name must contain only letters. Try again.\n");
                }
                else {
                    break;
                }
            } while (1);
            do {
                printf("Course (1-5): ");
                if (scanf("%hhu", &student.Course) == 1 && student.Course >= 1 && student.Course <= 5) {
                    break;
                }
                else {
                    printf("Course must be a number between 1 and 5. Try again.\n");
                    while (getchar() != '\n');
                }
            } while (1);

            do {
                printf("Group (1-10): ");
                if (scanf("%hhu", &student.Group) == 1 && student.Group >= 1 && student.Group <= 10) {
                    break;
                }
                else {
                    printf("Group must be a number between 1 and 10. Try again.\n");
                    while (getchar() != '\n');
                }
            } while (1);

            do {
                printf("ID (exactly 5 digits): ");
                scanf("%s", student.ID);
                int valid = 1;
                if (strlen(student.ID) == 5) {
                    for (int i = 0; i < 5; i++) {
                        if (!isdigit(student.ID[i])) {
                            valid = 0;
                            break;
                        }
                    }
                }
                else {
                    valid = 0;
                }
                if (!valid) {
                    printf("ID must contain exactly 5 digits. Try again.\n");
                }
                else {
                    break;
                }
            } while (1);

            addRow(student, pos);
            break;
        case 3:
            if (!open) {
                printf("File is not open\n");
                break;
            }
            printf("Position: ");
            scanf("%d", &pos);
            remRow(pos);
            break;
        case 4:
            if (!open) {
                printf("File is not open\n");
                break;
            }
            printf("Position: ");
            scanf("%d", &pos);
            printRow(pos);
            break;
        case 5:
            if (!open) {
                printf("File is not open\n");
                break;
            }
            printRows();
            break;
        case 6:
            if (!open) {
                printf("File is not open\n");
                break;
            }
            closeMapping();
            open = 0; 
            printf("Mapping is closed\n");
            break;
        case 7:
            printf("Exiting the program...\n");
            if (open) {
                closeMapping();
            }
            return 0;
        default:
            printf("Wrong option. Try again...\n");
        }
    } while (choice != 7);

    FreeLibrary(hLib);
    return 0;
}