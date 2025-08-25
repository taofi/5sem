#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>
#include <dlfcn.h>
#include "Mappingd.h"

typedef void (*CreateSampleFileType)(const char*);
typedef void (*OpenMappingType)(const char*);
typedef void (*AddRowType)(struct Student, int);
typedef int (*CheckPosType)(int);
typedef void (*RemRowType)(int);
typedef void (*PrintRowType)(int);
typedef void (*PrintRowsType)();
typedef void (*CloseMappingType)();
typedef int (*GetValidInputType)(const char*, int*);

int main() {
    void* libHandle = dlopen("./libMappingd.so", RTLD_LAZY);
    if (!libHandle) {
        fprintf(stderr, "Failed to load library: %s\n", dlerror());
        return 1;
    }

    CreateSampleFileType createSampleFile = (CreateSampleFileType)dlsym(libHandle, "createSampleFile");
    OpenMappingType openMapping = (OpenMappingType)dlsym(libHandle, "openMapping");
    AddRowType addRow = (AddRowType)dlsym(libHandle, "addRow");
    CheckPosType checkPos = (CheckPosType)dlsym(libHandle, "checkPos");
    RemRowType remRow = (RemRowType)dlsym(libHandle, "remRow");
    PrintRowType printRow = (PrintRowType)dlsym(libHandle, "printRow");
    PrintRowsType printRows = (PrintRowsType)dlsym(libHandle, "printRows");
    CloseMappingType closeMapping = (CloseMappingType)dlsym(libHandle, "closeMapping");
    GetValidInputType getValidInput = (GetValidInputType)dlsym(libHandle, "getValidInput");

    if (!createSampleFile || !openMapping || !addRow || !remRow || !printRow ||
        !printRows || !closeMapping || !getValidInput) {
        fprintf(stderr, "Failed to get function address: %s\n", dlerror());
        dlclose(libHandle);
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
        printf("\nEnter, to continue...\n");
        getchar();
        getchar();

        system("clear");
        if (!getValidInput("\n1. Open file \n 2. Add student \n 3. Rem student \n 4. Print row \n 5. Print file \n 6. Close file \n 7. Exit\n----------------------------------------------------------------------------------------------------------------\nchoice: ",
            &choice)) {
            continue;
        }

        switch (choice) {
        case 1:
            if (open) {
                printf("File is already open. Close it first.\n");
                break;
            }
            createSampleFile(filePath);
            openMapping(filePath);
            open = 1;
            break;
        case 2:
            if (!open) {
                printf("File is not open\n");
                break;
            }
            printf("Position: ");
            scanf("%d", &pos);
            if (!checkPos(pos)) {
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
            printf("Exiting...\n");
            break;
        case 7:
            printf("Exiting the program...\n");
            if (open) {
                closeMapping();
                printf("File closed before exiting.\n");
            }
            dlclose(libHandle);
            return 0;
        default:
            printf("Wrong option. Try again...\n");
        }
    } while (choice != 7);

    dlclose(libHandle);
    return 0;
}