#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>
#include <errno.h>

#define MAX_STUDENTS 100

struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};

struct Student* pStudents;
int studentCount = MAX_STUDENTS;
struct Student students[MAX_STUDENTS];
int fileIsOpen = 0;
int fd;
size_t fileSize = MAX_STUDENTS * sizeof(struct Student);

void createSampleFile(const char* filePath) {
    for (int i = 0; i < 10; i++) {
        snprintf(students[i].Name, sizeof(students[i].Name), "name%d", i);
        snprintf(students[i].Surname, sizeof(students[i].Surname), "surname%d", i);
        students[i].Course = (unsigned char)(i % 4 + 1);
        students[i].Group = (unsigned char)(i % 10 + 1);
        snprintf(students[i].ID, sizeof(students[i].ID), "%05d", i);
    }

    fd = open(filePath, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("Failed to open file");
        return;
    }

    if (ftruncate(fd, fileSize) != 0) {
        perror("Failed to set file size");
        close(fd);
        return;
    }

    if (write(fd, students, fileSize) != fileSize) {
        perror("Failed to write to file");
        close(fd);
        return;
    }

    close(fd);
}

void OpenMapping(const char* filePath) {
    fd = open(filePath, O_RDWR);
    if (fd < 0) {
        perror("Failed to open file");
        return;
    }

    pStudents = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pStudents == MAP_FAILED) {
        perror("Failed to map file");
        close(fd);
        return;
    }

    fileIsOpen = 1;
}

void AddRow(struct Student row, int pos) {
    if (pos < 0) {
        pos = studentCount + pos;
    }

    if (pos < 0 || pos >= studentCount) {
        printf("Wrong position: %d\n", pos);
        return;
    }

    if (strlen(pStudents[pos].Name) != 0) {
        printf("Position %d is already busy.\n", pos);
        return;
    }

    pStudents[pos] = row;
}

void RemRow(int pos) {
    if (pos < 0) {
        pos = studentCount + pos;
    }

    if (pos < 0 || pos >= studentCount) {
        printf("Wrong position: %d\n", pos);
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
        pos = studentCount + pos;
    }

    if (pos < 0 || pos >= studentCount) {
        printf("Wrong position: %d\n", pos);
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

void PrintRows() {
    int hasStudents = 0;

    for (int i = 0; i < studentCount; i++) {
        if (strlen(pStudents[i].Name) != 0) {
            PrintRow(i);
            hasStudents = 1;
        }
    }

    if (!hasStudents) {
        printf("No students found in the list.\n");
    }
}

void CloseMapping() {
    if (!fileIsOpen) {
        printf("No file is currently open.\n");
        return;
    }

    if (munmap(pStudents, fileSize) != 0) {
        perror("Failed to unmap file");
    }

    close(fd);
    fileIsOpen = 0;
}

int getValidInput(const char* prompt, int* value) {
    printf("%s", prompt);
    if (scanf("%d", value) == 1) {
        return 1;
    } else {
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

int main() {
    char filePath[256];
    int pos;
    int choice;
    struct Student student;

    do {
        if (!getValidInput("-------------------------------------------------------------------------------\n1. Open file ~ 2. Add student ~ 3. Rem student ~ 4. Print row ~ 5. Print file ~ 6. Close file ~ 7. Exit\n-------------------------------------------------------------------------------\nchoice: ",
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

            if (access(filePath, F_OK) == -1) {
                createSampleFile(filePath);
            }
            OpenMapping(filePath);
            break;
        case 2:
            if (!fileIsOpen) {
                printf("File not opened. Open it first.\n");
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
                printf("Wrong input, enter a number from 1 to 4.\n");
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
                printf("Wrong input, enter a number from 1 to 10.\n");
            }

            while (1) {
                printf("ID (5 nums): ");
                scanf("%s", student.ID);
                if (isDigitsOnly(student.ID) && strlen(student.ID) == 5) {
                    break;
                }
                printf("Wrong input, it should be 5 numbers.\n");
            }

            if (!getValidInput("Position: ", &pos)) {
                break;
            }

            AddRow(student, pos);
            break;
        case 3:
            if (!fileIsOpen) {
                printf("File not opened. Open it first.\n");
                break;
            }

            if (!getValidInput("Position: ", &pos)) {
                break;
            }

            RemRow(pos);
            break;
        case 4:
            if (!fileIsOpen) {
                printf("File not opened. Open it first.\n");
                break;
            }

            if (!getValidInput("Position: ", &pos)) {
                break;
            }

            PrintRow(pos);
            break;
        case 5:
            if (!fileIsOpen) {
                printf("File not opened. Open it first.\n");
                break;
            }
            PrintRows();
            break;
        case 6:
            if (!fileIsOpen) {
                printf("No file is currently open.\n");
            } else {
                CloseMapping();
                printf("File closed successfully.\n");
            }
            break;
        case 7:
            if (fileIsOpen) {
                CloseMapping();
            }
            printf("Exiting program...\n");
            break;
        default:
            printf("Wrong option. Try again...\n");
        }
    } while (choice != 7);

    return 0;
}
