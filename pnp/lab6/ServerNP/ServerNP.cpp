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
	cout << "ServerNP" << endl;

	HANDLE hPipe; // дескриптор канала
	DWORD ps;

	try
	{
		if ((hPipe = CreateNamedPipe(PIPE_NAME,
			PIPE_ACCESS_DUPLEX, //дуплексный канал
			PIPE_TYPE_MESSAGE | PIPE_WAIT, // сообщения|синхронный
			1, NULL, NULL, // максимум 1 экземпляр
			INFINITE, NULL)) == INVALID_HANDLE_VALUE)
			throw SetErrorMsgText("create:", GetLastError());
		while (true)
		{
			if (!ConnectNamedPipe(hPipe, NULL)) // ожидать клиента 
				throw SetErrorMsgText("connect:", GetLastError());
			char buf[50];
			while (ReadFile(hPipe, buf, sizeof(buf), &ps, NULL))
			{
				cout << buf << endl;

				if (!WriteFile(hPipe, buf, sizeof(buf), &ps, NULL))
					throw SetErrorMsgText("WriteFile: ", GetLastError());
			}

			if (!DisconnectNamedPipe(hPipe))
				throw SetErrorMsgText("Disconnect: ", GetLastError());
		}
		if (!CloseHandle(hPipe))
			throw SetErrorMsgText("Close: ", GetLastError());
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}
}