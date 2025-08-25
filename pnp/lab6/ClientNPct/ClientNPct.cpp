#include <iostream>
#include "errors.h"
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib") 
#include <string>
#include <tchar.h>
using namespace std;


#define PIPE_NAME L"\\\\.\\pipe\\Tube"


int main()
{
    setlocale(LC_ALL, "Rus");
    cout << "ClientNPt" << endl;

    DWORD ps;
    try
    {

        int message_amount = 100;
        char obuf[50];
        char ibuf[50];

        for (int i = 1; i <= message_amount; i++)
        {

            string obufstr = "Hello from ClientNPct " + to_string(i);
            strcpy_s(obuf, obufstr.c_str());

            if (CallNamedPipe(PIPE_NAME,
                obuf, //буфер для отправки (записи)
                sizeof(obuf), // размер входного буфера
                ibuf, //буфер для получения (чтения)
                sizeof(ibuf), //размер выходного буфера
                &ps, //количество прочитанных байт
                NULL))  // параметр для асинхронного доступа
                cout << ibuf << endl;
            else
                throw SetErrorMsgText("CallNamedPipe: ", GetLastError());
        }

    }
    catch (string ErrorPipeText)
    {
        cout << endl << ErrorPipeText;
    }

}