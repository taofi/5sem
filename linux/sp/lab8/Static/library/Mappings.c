#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Mappings.h"

int fd;
struct Student* pStudents;
int studentCount = MAX_STUDENTS;
struct Student students[MAX_STUDENTS];
int validPos = 1;

void createSampleFile(const char* filePath) {
    for (int i = 0; i < 10; i++) {
        snprintf(students[i].Name, sizeof(students[i].Name), "Student%d", i);
        snprintf(students[i].Surname, sizeof(students[i].Surname), "Surname%d", i);
        students[i].Course = (unsigned char)(i % 5 + 1);
        students[i].Group = (unsigned char)(i % 3 + 1);
        snprintf(students[i].ID, sizeof(students[i].ID), "%04d", i + 1);
    }

    fd = open(filePath, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }

    ftruncate(fd, sizeof(struct Student) * MAX_STUDENTS);
    write(fd, students, sizeof(struct Student) * MAX_STUDENTS);
    printf("Written %lu bytes to the file.\n", sizeof(struct Student) * MAX_STUDENTS);
}

void OpenMapping(const char* filePath) {
    fd = open(filePath, O_RDWR);
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
}

void AddRow(struct Student row, int pos) {
    if (pos < 0) {
        pos = pos + studentCount;
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

int CheckPos(int pos) {
    if (pos < 0) {
        pos = pos + studentCount;
    }

    if (pos < 0 || pos >= studentCount) {
        printf("Wrong position: %d\n", pos);
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
        pos = pos + studentCount;
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

void PrintRows() {
    for (int i = 0; i < studentCount; i++) {
        if (strlen(pStudents[i].Name) != 0) {
            PrintRow(i);
        }
    }
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

void CloseMapping() {
    munmap(pStudents, sizeof(struct Student) * MAX_STUDENTS);
    close(fd);
}