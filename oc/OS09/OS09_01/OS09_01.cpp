#include <Windows.h>
#include <iostream>

using namespace std;

#define FILE_PATH L"D:/5sem/oc/OS09/OS09_01.txt"

#define READ_BYTES 300

string getFileName(wchar_t* filePath)
{
    wstring ws(filePath);
    string filename(ws.begin(), ws.end());
    const size_t last_slash_idx = filename.find_last_of("\\/");
    if (string::npos != last_slash_idx)
        filename.erase(0, last_slash_idx + 1);
    return filename;
}

LPCWSTR getFileType(HANDLE file)
{
    switch (GetFileType(file))
    {
    case FILE_TYPE_UNKNOWN:
        return L"FILE_TYPE_UNKNOWN";
    case FILE_TYPE_DISK:
        return L"FILE_TYPE_DISK";
    case FILE_TYPE_CHAR:
        return L"FILE_TYPE_CHAR";
    case FILE_TYPE_PIPE:
        return L"FILE_TYPE_PIPE";
    case FILE_TYPE_REMOTE:
        return L"FILE_TYPE_REMOTE";
    default:
        return L"[ERROR]: WRITE FILE TYPE";
    }
}

BOOL printFileInfo(LPWSTR path)
{
    HANDLE file = CreateFile(
        path,
        GENERIC_READ,
        NULL,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    SYSTEMTIME sysTime;
    FILETIME localTime;
    BY_HANDLE_FILE_INFORMATION fi;
    BOOL fResult = GetFileInformationByHandle(file, &fi);
    if (fResult)
    {
        cout << "File name:\t" << getFileName((wchar_t*)FILE_PATH);
        wcout << "\nFile type:\t" << getFileType(file);
        cout << "\nFile size:\t" << fi.nFileSizeLow << " bytes";
        FileTimeToLocalFileTime(&fi.ftCreationTime, &localTime);
        FileTimeToSystemTime(&localTime, &sysTime);
        printf("\nCreate time:\t%02d.%02d.%04d %02dh %02dm %02ds", sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour % 24, sysTime.wMinute, sysTime.wSecond);
        FileTimeToLocalFileTime(&fi.ftLastWriteTime, &localTime);
        FileTimeToSystemTime(&localTime, &sysTime);
        printf("\nUpdate time:\t%02d.%02d.%04d %02dh %02dm %02ds", sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour % 24, sysTime.wMinute, sysTime.wSecond);
    }
    CloseHandle(file);
    return true;
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

    // Читаем содержимое файла
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

    // в широкие символы
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

    LPWSTR path = (LPWSTR)FILE_PATH;
    cout << "\n...................................................\n\n";
    printFileInfo(path);
    cout << "\n\n...................................................";
    printFileTxt(path);
    cout << "\n...................................................";

    return 0;
}