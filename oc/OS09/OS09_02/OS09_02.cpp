#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define FILE_PATH L"D:/5sem/oc/OS09/OS09_01.txt"

#define READ_BYTES 500

BOOL delRowFileTxt(LPWSTR fileName, DWORD row) {
    HANDLE hFile = CreateFile(
        fileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "Error: Cannot open file for reading" << endl;
        return FALSE;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        cout << "Error: Cannot get file size" << endl;
        return FALSE;
    }

    char* buffer = new char[fileSize + 1];
    DWORD bytesRead;
    if (!ReadFile(hFile, buffer, fileSize, &bytesRead, NULL)) {
        CloseHandle(hFile);
        delete[] buffer;
        cout << "Error: Cannot read file" << endl;
        return FALSE;
    }
    buffer[bytesRead] = '\0';
    CloseHandle(hFile);

    vector<string> lines;
    string currentLine;
    for (DWORD i = 0; i < bytesRead; i++) {
        if (buffer[i] == '\n') {
            if (!currentLine.empty() && currentLine.back() == '\r') {
                currentLine.pop_back();
            }
            lines.push_back(currentLine);
            currentLine.clear();
        }
        else {
            currentLine += buffer[i];
        }
    }
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }
    delete[] buffer;

    if (row > lines.size() || row == 0) {
        cout << "Error: Row number exceeds file size" << endl;
        return FALSE;
    }

    hFile = CreateFile(
        fileName,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "Error: Cannot open file for writing" << endl;
        return FALSE;
    }

    for (size_t i = 0; i < lines.size(); i++) {
        if (i + 1 != row) {
            string lineWithNewline = lines[i] + "\r\n";
            DWORD bytesWritten;
            if (!WriteFile(hFile, lineWithNewline.c_str(), lineWithNewline.length(), &bytesWritten, NULL)) {
                CloseHandle(hFile);
                cout << "Error: Cannot write to file" << endl;
                return FALSE;
            }
        }
    }

    CloseHandle(hFile);
    return TRUE;
}


BOOL printFileTxt(LPWSTR path)
{
    HANDLE file = CreateFile(
        path,
        GENERIC_READ,
        NULL,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (file == INVALID_HANDLE_VALUE) {
        cout << "Cannot open file" << endl;
        return FALSE;
    }

    // BOM (Byte Order Mark)
    unsigned char bom[3];
    DWORD bytesRead;
    ReadFile(file, bom, 3, &bytesRead, NULL);

    // в начало файла
    SetFilePointer(file, 0, NULL, FILE_BEGIN);


    wchar_t* buf = new wchar_t[READ_BYTES];
    ZeroMemory(buf, READ_BYTES * sizeof(wchar_t));

    BOOL b = ReadFile(
        file,
        buf,
        READ_BYTES * sizeof(wchar_t),
        &bytesRead,
        NULL);

    if (!b) {
        delete[] buf;
        CloseHandle(file);
        throw "[ERROR] ReadFile threw exception.";
    }

    //  широкие символы
    int wideLength = MultiByteToWideChar(CP_UTF8, 0, (char*)buf, bytesRead, NULL, 0);
    wchar_t* wideBuf = new wchar_t[wideLength + 1];
    MultiByteToWideChar(CP_UTF8, 0, (char*)buf, bytesRead, wideBuf, wideLength);
    wideBuf[wideLength] = L'\0';

    cout << "\n\n\tPrint file (" << bytesRead << " bytes)\n";
    wcout << wideBuf << endl;

    delete[] buf;
    delete[] wideBuf;
    CloseHandle(file);
    return TRUE;
}



int main()
{

    setlocale(LC_ALL, "Russian");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    LPWSTR name = (LPWSTR)FILE_PATH;
    printFileTxt(name);

    delRowFileTxt(name, 1);
    printFileTxt(name);
    cout << "\n\n...................................................";
    delRowFileTxt(name, 3);
    printFileTxt(name);

    cout << "\n\n...................................................";
    delRowFileTxt(name, 8);
    printFileTxt(name);
    cout << "\n\n...................................................";
    delRowFileTxt(name, 10);
    printFileTxt(name);
    cout << "\n\n...................................................";


}