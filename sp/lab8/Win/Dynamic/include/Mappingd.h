#pragma once
#include "D:\5sem\sp\lab8\Win\Dynamic\include\str.h"


void createSampleFile(const char* filePath);
void OpenMapping(const char* filePath);

void AddRow(struct Student row, int pos);
int ChekPos(int pos);
void RemRow(int pos);
void PrintRow(int pos);
void PrintRows();
void CloseMapping();
int getValidInput(const char* prompt, int* value);
struct Student* GetPStudents();


