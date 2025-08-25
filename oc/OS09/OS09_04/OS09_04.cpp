#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>

using namespace std;
#define FILE_PATH L"D:/5sem/oc/OS09/OS09_01.txt"




DWORD getLineCount(LPWSTR fileName) {
    ifstream file(fileName);
    DWORD lineCount = 0;
    string line;
    while (getline(file, line)) {
        lineCount++;
    }
    return lineCount;
}

BOOL printWatchRowFileTxt(LPWSTR fileName, DWORD mlsec) {
    DWORD initialLineCount = getLineCount(fileName);
    cout << "Initial line count: " << initialLineCount << endl;

    DWORD startTime = GetTickCount();

    while (true) {

        Sleep(1000);

        DWORD newLineCount = getLineCount(fileName);

        if (newLineCount != initialLineCount) {
            cout << "Line count changed: " << newLineCount << " (was " << initialLineCount << ")" << endl;
            initialLineCount = newLineCount;
        }


        if (GetTickCount() - startTime >= mlsec) {
            cout << "Monitoring time expired." << endl;
            break;
        }
    }

    return TRUE;
}

int main() {
    LPWSTR fileName = (LPWSTR)FILE_PATH;
    DWORD monitorTime = 15000;

    printWatchRowFileTxt(fileName, monitorTime);

    return 0;
}
