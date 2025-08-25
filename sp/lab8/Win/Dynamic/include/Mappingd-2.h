#pragma once
#include <windows.h>
#include "D:\5sem\sp\lab8\Win\Dynamic\include\str.h"



__declspec(dllexport) void createSampleFile(const char* filePath);
__declspec(dllexport) void OpenMapping(const char* filePath);
__declspec(dllexport) void AddRow(struct Student row, int pos);
__declspec(dllexport) int ChekPos(int pos);
__declspec(dllexport) void RemRow(int pos);
__declspec(dllexport) void PrintRow(int pos);
__declspec(dllexport) void PrintRows();
__declspec(dllexport) void CloseMapping();
__declspec(dllexport) int getValidInput(const char* prompt, int* value);
__declspec(dllexport) void SetSharedStudents(struct Student* sharedStudents);

