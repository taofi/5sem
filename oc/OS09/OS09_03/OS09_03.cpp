#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
using namespace std;
#define FILE_PATH L"D:/5sem/oc/OS09/OS09_01.txt"


#define READ_BYTES 500

BOOL insRowFileTxt(LPWSTR fileName, LPWSTR str, DWORD row) {
    try {

        HANDLE hFile = CreateFile(
            fileName,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (hFile == INVALID_HANDLE_VALUE) {
            throw "Cannot open file for reading";
        }

        //BOM
        unsigned char bom[3];
        DWORD bytesRead;
        ReadFile(hFile, bom, 3, &bytesRead, NULL);

        bool isUtf8 = (bytesRead == 3 && bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF);
        SetFilePointer(hFile, 0, NULL, FILE_BEGIN);  // в начало 

        // чтение файла в буфер
        DWORD fileSize = GetFileSize(hFile, NULL);
        if (fileSize == INVALID_FILE_SIZE) {
            CloseHandle(hFile);
            throw "Cannot get file size";
        }

        char* buffer = new char[fileSize + 1];
        if (!ReadFile(hFile, buffer, fileSize, &bytesRead, NULL)) {
            CloseHandle(hFile);
            delete[] buffer;
            throw "Cannot read file";
        }
        buffer[bytesRead] = '\0';
        CloseHandle(hFile);

        // разбиваем на строки
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

        //  LPWSTR str в UTF-8 или ANSI
        string newLine;
        int sizeNeeded;
        if (isUtf8) {
            sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
            newLine.resize(sizeNeeded - 1);
            WideCharToMultiByte(CP_UTF8, 0, str, -1, &newLine[0], sizeNeeded - 1, NULL, NULL);
        }
        else {
            sizeNeeded = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
            newLine.resize(sizeNeeded - 1);
            WideCharToMultiByte(CP_ACP, 0, str, -1, &newLine[0], sizeNeeded - 1, NULL, NULL);
        }

        // вставляем
        if (row == -1) {
            lines.push_back(newLine);
        }
        else if (row == 0) {
            lines.insert(lines.begin(), newLine);
        }
        else if (row >= lines.size()) {
            lines.push_back(newLine);
        }
        else {
            lines.insert(lines.begin() + row, newLine);
        }

        //запись
        hFile = CreateFile(
            fileName,
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS, 
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (hFile == INVALID_HANDLE_VALUE) {
            throw "Cannot open file for writing";
        }


        DWORD bytesWritten;
        if (isUtf8) {
            const char* utf8BOM = "\xEF\xBB\xBF";
            WriteFile(hFile, utf8BOM, 3, &bytesWritten, NULL);
        }


        for (const auto& line : lines) {
            string lineWithNewline = line + "\r\n";
            if (!WriteFile(hFile, lineWithNewline.c_str(), lineWithNewline.length(), &bytesWritten, NULL)) {
                CloseHandle(hFile);
                throw "Cannot write to file";
            }
        }

        CloseHandle(hFile);
        return TRUE;
    }
    catch (const char* err) {
        cout << "Error: " << err << endl;
        return FALSE;
    }
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

    //   широкие символы
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

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    LPWSTR name = (LPWSTR)FILE_PATH;
    LPWSTR str = (LPWSTR)L"Inserted new line";
    insRowFileTxt(name, str, 0);
    insRowFileTxt(name, str, -1);
    insRowFileTxt(name, str, 5);
    insRowFileTxt(name, str, 7);
    printFileTxt(name);
    return 0;
}
