#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Mappingd.h"

struct Student* pStudents;
int studentCount = MAX_STUDENTS;

__attribute__((visibility("default")))
void createSampleFile(const char* filePath) {
    struct Student students[MAX_STUDENTS] = { 0 };
    for (int i = 0; i < 10; i++) {
        snprintf(students[i].Name, sizeof(students[i].Name), "Student%d", i);
        snprintf(students[i].Surname, sizeof(students[i].Surname), "Surname%d", i);
        students[i].Course = (unsigned char)(i % 5 + 1);
        students[i].Group = (unsigned char)(i % 3 + 1);
        snprintf(students[i].ID, sizeof(students[i].ID), "%04d", i + 1);
    }

    int fd = open(filePath, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }

    ftruncate(fd, sizeof(struct Student) * MAX_STUDENTS);
    write(fd, students, sizeof(struct Student) * MAX_STUDENTS);
    printf("Written %lu bytes to the file.\n", sizeof(struct Student) * MAX_STUDENTS);
    close(fd);
}

__attribute__((visibility("default")))
void openMapping(const char* filePath) {
    int fd = open(filePath, O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    pStudents = mmap(NULL, sizeof(struct Student) * MAX_STUDENTS, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pStudents == MAP_FAILED) {
        perror("Failed to map view of file");
        close(fd);
        return;
    }
    close(fd);
}

__attribute__((visibility("default")))
void addRow(struct Student row, int pos) {
    if (pos < 0) {
        pos += studentCount;
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

__attribute__((visibility("default")))
int checkPos(int pos) {
    if (pos < 0) {
        pos += studentCount;
    }

    if (pos < 0 || pos >= studentCount) {
        printf("Wrong position: %d\n", pos);
        return 0;
    }

    return (strlen(pStudents[pos].Name) == 0);
}

__attribute__((visibility("default")))
void remRow(int pos) {
    if (pos < 0) {
        pos += studentCount;
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

__attribute__((visibility("default")))
void printRow(int pos) {
    if (pos < 0) {
        pos += studentCount;
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
    printf("Name: %s, LastName: %s, Course: %d, Group: %d, ID: %s\n",
        student.Name, student.Surname, student.Course, student.Group, student.ID);
}

__attribute__((visibility("default")))
void printRows() {
    for (int i = 0; i < studentCount; i++) {
        if (strlen(pStudents[i].Name) != 0) {
            printRow(i);
        }
    }
}

__attribute__((visibility("default")))
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

__attribute__((visibility("default")))
void closeMapping() {
    munmap(pStudents, sizeof(struct Student) * MAX_STUDENTS);
}
