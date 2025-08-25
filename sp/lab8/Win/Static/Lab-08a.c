#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "D:\5sem\sp\lab8\Win\Static\library\include\Mappings.h"




int main() {
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
            OpenMapping(filePath);
            open = 1;
            break;
        }
        case 2:
            if (!open) {
                printf("File is not open\n");
                break;
            }
            printf("Position: ");
            scanf("%d", &pos);
            if (!ChekPos(pos)) {
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

            AddRow(student, pos);
            break;

        case 3:
            if (!open) {
                printf("File is not open\n");
                break;
            }
            printf("Position: ");
            scanf("%d", &pos);
            RemRow(pos);
            break;
        case 4:
            if (!open) {
                printf("File is not open\n");
                break;
            }
            printf("Position: ");
            scanf("%d", &pos);
            PrintRow(pos);
            break;
        case 5:
            if (!open) {
                printf("File is not open\n");
                break;
            }
            PrintRows();
            break;
        case 6:
            if (!open) {
                printf("File is not open\n");
                break;
            }
            CloseMapping();
            printf("File closed. Returning to main menu...\n");
            open = 0;
            break;
        case 7:
            printf("Exiting the program...\n");
            if (open) {
                CloseMapping(); 
                printf("File closed before exiting.\n");
            }
            return 0;
        default:
            printf("Wrong option. Try again...\n");
        }
    } while (choice != 7);

    return 0;
}



