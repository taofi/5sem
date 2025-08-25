#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_STUDENTS 100

struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};

HANDLE hFileMapping;
struct Student* pStudents;
int studentCount = MAX_STUDENTS;
struct Student students[MAX_STUDENTS];
HANDLE hFile;
int fileIsOpen = 0; 


void OpenMapping(LPSTR filePath, LPINT listSize) {
    hFile = CreateFileA(filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("error CreateFileA: %d\n", GetLastError());
        return;
    }

    hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
    if (!hFileMapping) {
        printf("Failed to create file mapping %d\n", GetLastError());
        return;
    }

    pStudents = (struct Student*)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (!pStudents) {
        printf("Failed to map view of file %d\n", GetLastError());
        return;
    }

    fileIsOpen = 1;
}

void AddRow(HANDLE hFileMapping, struct Student row, INT pos) {
    if (hFileMapping == NULL) {
        printf("error: wrong handler.\n");
        return;
    }

    if (pos < 0) {
        pos = studentCount + pos;
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

void RemRow(HANDLE hFileMapping, INT pos) {
    if (hFileMapping == NULL) {
        printf("error: wrong handler.\n");
        return;
    }
    if (pos < 0 ) {
        pos = studentCount + pos;
    }

    if (pos >= studentCount) {
        printf("wrong position: %d\n", pos);
        return;
    }
    if (pos + studentCount < 0) {
        printf("wrong position: %d\n", pos);
        return;
    }

    if (strlen(pStudents[pos].Name) == 0) {
        printf("Position %d is empty.\n", pos);
        return;
    }

    memset(&pStudents[pos], 0, sizeof(struct Student));
}

void PrintRow(HANDLE hFileMapping, INT pos) {
    if (hFileMapping == NULL) {
        printf("Error: wrong file mapping handle.\n");
        return;
    }
    if (pos < 0) {
        pos = studentCount + pos;
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
    printf("Pos: %d, Name: %s, LastName: %s, Course: %d, Group: %d, ID: %s\n",
        pos, student.Name, student.Surname, student.Course, student.Group, student.ID);
}

void PrintRows(HANDLE hFileMapping) {
    if (hFileMapping == NULL) {
        printf("Error: wrong file mapping handle.\n");
        return;
    }

    int hasStudents = 0;

    for (int i = 0; i < studentCount; i++) {
        if (strlen(pStudents[i].Name) != 0) {
            PrintRow(hFileMapping, i);
            hasStudents = 1;
        }
    }

    if (!hasStudents) {
        printf("No students found in the list.\n");
    }
}

void CloseMapping(HANDLE hFileMapping) {
    if (hFileMapping == NULL) {
        printf("error: wrong handler\n");
        return;
    }

    UnmapViewOfFile(pStudents);
    CloseHandle(hFileMapping);
    hFileMapping = NULL;
    if (hFile != NULL) {
        CloseHandle(hFile);
        hFile = NULL;
    }
    fileIsOpen = 0;
}

int getValidInput(const char* prompt, int* value) {
    printf("%s", prompt);
    if (scanf("%d", value) == 1) {
        return 1;
    }
    else {
        printf("Please enter a valid number.\n");
        while (getchar() != '\n');
        return 0;
    }
}

int isDigitsOnly(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}

void createStartFile(const char* filePath) {
    for (int i = 0; i < 10; i++) {
        snprintf(students[i].Name, sizeof(students[i].Name), "name%d", i);
        snprintf(students[i].Surname, sizeof(students[i].Surname), "surname%d", i);
        students[i].Course = (unsigned char)(i % 4 + 1);
        students[i].Group = (unsigned char)(i % 10 + 1);
        snprintf(students[i].ID, sizeof(students[i].ID), "%05d", i);
    }

    hFile = CreateFileA(filePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file: %d\n", GetLastError());
        return;
    }

    DWORD bytesWritten;
    WriteFile(hFile, students, sizeof(struct Student) * MAX_STUDENTS, &bytesWritten, NULL);
    printf("Written %lu bytes to the file.\n", bytesWritten);

    CloseHandle(hFile);
}

int main() {
    char filePath[MAX_PATH];
    int pos;
    int listSize = 0;

    int choice;
    struct Student student;

    do {
        if (!getValidInput("\n1. Open file \n 2. Add student \n 3. Rem student \n 4. Print row \n 5. Print file \n 6. Close file \n 7. Exit\n----------------------------------------------------------------------------------------------------------------\nchoice: ",
            &choice)) {
            continue;
        }

        switch (choice) {
        case 1:
            if (fileIsOpen) {
                printf("File is already open. Close it first.\n");
                break;
            }
            printf("File path: ");
            scanf("%s", filePath);

            char* ext = strrchr(filePath, '.');

            if (GetFileAttributesA(filePath) == INVALID_FILE_ATTRIBUTES) {
                createStartFile(filePath);
            }
            OpenMapping(filePath, &listSize);
            break;
        case 2:
            if (!fileIsOpen) {
                printf("File not opened.Open it first.\n");
                break;
            }
            printf("Name: ");
            scanf("%s", student.Name);
            printf("Last name: ");
            scanf("%s", student.Surname);

            char tempInput[16];

            while (1) {
                printf("Course (1-4): ");
                scanf("%s", tempInput);
                if (isDigitsOnly(tempInput)) {
                    int course = atoi(tempInput);
                    if (course >= 1 && course <= 4) {
                        student.Course = (unsigned char)course;
                        break;
                    }
                }
                printf("wrong input, enter a number from 1 to 4.\n");
            }

            while (1) {
                printf("Group (1-10): ");
                scanf("%s", tempInput);
                if (isDigitsOnly(tempInput)) {
                    int group = atoi(tempInput);
                    if (group >= 1 && group <= 10) {
                        student.Group = (unsigned char)group;
                        break;
                    }
                }
                printf("wrong input, enter a number from 1 to 10.\n");
            }

            while (1) {
                printf("ID (5 nums): ");
                scanf("%s", student.ID);
                if (isDigitsOnly(student.ID) && strlen(student.ID) == 5) {
                    break;
                }
                printf("wrong input, it should be 5 numbers\n");
            }

            if (!getValidInput("Position: ", &pos)) {
                break;
            }


            AddRow(hFileMapping, student, pos);
            break;
        case 3:
            if (!fileIsOpen) {
                printf("File not opened.Open it first\n");
                break;
            }

            if (!getValidInput("Position: ", &pos)) {
                break;
            }

            RemRow(hFileMapping, pos);
            break;
        case 4:
            if (!fileIsOpen) {
                printf("File not opened.Open it first\n");
                break;
            }

            if (!getValidInput("Position: ", &pos)) {
                break;
            }

            PrintRow(hFileMapping, pos);
            break;
        case 5:
            if (!fileIsOpen) {
                printf("File not opened.Open it first\n");
                break;
            }
            PrintRows(hFileMapping);
            break;
        case 6:
            if (!fileIsOpen) {
                printf("No file is currently open.\n");
            }
            else {
                CloseMapping(hFileMapping);
                printf("File closed successfully.\n");
            }
            break;
        case 7:
            if (fileIsOpen) {
                CloseMapping(hFileMapping);
            }
            printf("Exiting program...\n");
            break;
        default:
            printf("Wrong option. Try again...\n");
        }
    } while (choice != 7);

    return 0;
}
