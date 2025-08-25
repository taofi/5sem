#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void PrintInfo(LPSTR filename);

int IsTextFile(HANDLE hFile) {
    DWORD bytesRead;
    char buffer[512];
    BOOL isText = TRUE;

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
        for (DWORD i = 0; i < bytesRead; i++) {
            if (buffer[i] < 32 && buffer[i] != 9 && buffer[i] != 10 && buffer[i] != 13) {
                isText = FALSE;
                break;
            }
        }
        if (!isText) break;
    }

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    return isText;
}


void FindType(DWORD dwFileType) {
    switch (dwFileType) {
    case FILE_TYPE_UNKNOWN:
        fputs("Unknown type.\n", stdout);
        break;
    case FILE_TYPE_DISK:
        fputs("Disk type.\n", stdout);
        break;
    case FILE_TYPE_CHAR:
        fputs("Char type.\n", stdout);
        break;
    case FILE_TYPE_PIPE:
        fputs("Pipe type.\n", stdout);
        break;
    default:
        break;
    }
}


void PrintText(const char* fileName) {
    HANDLE hFile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("---Error opening file: %s---\n", fileName);
        return;
    }
    DWORD bytesRead;
    char buffer[512];

    if (!IsTextFile(hFile)) {
        printf("---The file is not a text file: %s---\n", fileName);
        CloseHandle(hFile);
        return;
    }

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    while (ReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }

    CloseHandle(hFile);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Invalid arguments\n");
        return 1;
    }

    LPSTR filename = argv[1];

    PrintInfo(filename);

    printf("\n");

    PrintText(filename);

    return 0;
}

void PrintInfo(LPSTR filename) {
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;

    if (!GetFileAttributesEx(filename, GetFileExInfoStandard, &fileInfo)) {
        printf("GetFileAttributesEx error: %lu\n", GetLastError());
        return;
    }

    
    HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("---Error opening file: %s---\n", filename);
        return;
    }
    FILETIME creationTime = fileInfo.ftCreationTime;
    FILETIME accessTime = fileInfo.ftLastAccessTime;
    FILETIME writeTime = fileInfo.ftLastWriteTime;

    SYSTEMTIME st;
    if (FileTimeToSystemTime(&creationTime, &st))
        printf("Creation time: %02d-%02d-%d %02d:%02d:%02d\n", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
    if (FileTimeToSystemTime(&accessTime, &st))
        printf("Access time: %02d-%02d-%d %02d:%02d:%02d\n", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
    if (FileTimeToSystemTime(&writeTime, &st))
        printf("Write time: %02d-%02d-%d %02d:%02d:%02d\n", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

    LARGE_INTEGER fileSize;
    fileSize.LowPart = fileInfo.nFileSizeLow;
    fileSize.HighPart = fileInfo.nFileSizeHigh;

    printf("Size: %lld bytes (%.2f KB, %.2f MB)\n",
        fileSize.QuadPart,
        (double)fileSize.QuadPart / 1024,
        (double)fileSize.QuadPart / (1024 * 1024));

    printf("Name: %s\n", filename);
    DWORD dwFileType = GetFileType(hFile);
    FindType(dwFileType);
    wchar_t wideFileName[MAX_PATH];
    if (MultiByteToWideChar(CP_ACP, 0, filename, -1, wideFileName, MAX_PATH) == 0) {
        fprintf(stderr, "---Error converting---\n");
        return;
    }

    DWORD dwBinaryType;
    if (GetBinaryTypeW(wideFileName, &dwBinaryType)) {
        switch (dwBinaryType) {
        case SCS_32BIT_BINARY:
            fputs("32-bit Windows application.\n", stdout);
            break;
        case SCS_64BIT_BINARY:
            fputs("64-bit Windows application.\n", stdout);
            break;
        case SCS_DOS_BINARY:
            fputs("An MS-DOS application.\n", stdout);
            break;
        case SCS_OS216_BINARY:
            fputs("A 16-bit OS/2 application.\n", stdout);
            break;
        case SCS_PIF_BINARY:
            fputs("A PIF file that executes an MS-DOS application.\n", stdout);
            break;
        case SCS_POSIX_BINARY:
            fputs("A POSIX application.\n", stdout);
            break;
        case SCS_WOW_BINARY:
            fputs("A 16-bit Windows application.\n", stdout);
            break;
        default:
            fputs("Mystery application.\n", stdout);
            break;
        }
    }
    else {
        printf("---This file is not binary---\n");
    }
}

