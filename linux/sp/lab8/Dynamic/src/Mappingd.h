#pragma once

#define MAX_STUDENTS 100

struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};

void createSampleFile(const char* filePath);
void openMapping(const char* filePath);
void addRow(struct Student row, int pos);
int checkPos(int pos);
void remRow(int pos);
void printRow(int pos);
void printRows();
void closeMapping();
int getValidInput(const char* prompt, int* value);