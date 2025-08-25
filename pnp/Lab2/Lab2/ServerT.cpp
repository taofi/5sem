#include <iostream>
#include "errors.h"
#include "Winsock2.h"              
#pragma comment(lib, "WS2_32.lib") 
#pragma warning (disable: 4996)

using namespace std;


int main()
{
    setlocale(LC_ALL, "rus");

    WSADATA wsaData;
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw  SetErrorMsgText("Startup:", WSAGetLastError());
        SOCKET  sS;
        if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw  SetErrorMsgText("socket:", WSAGetLastError());

        SOCKADDR_IN serv{};                    // параметры  сокета sS
        serv.sin_family = AF_INET;           // используется IP-адресация  
        serv.sin_port = htons(2000);         // порт 2000
        serv.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw  SetErrorMsgText("bind:", WSAGetLastError());

        if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
            throw  SetErrorMsgText("listen:", WSAGetLastError());

        SOCKET cS;	                 // сокет для обмена данными с клиентом 
        SOCKADDR_IN clnt;             // параметры  сокета клиента
        memset(&clnt, 0, sizeof(clnt)); // обнулить память
        int lclnt = sizeof(clnt);    // размер SOCKADDR_IN

        if ((cS = accept(sS, (sockaddr*)&clnt, &lclnt)) == INVALID_SOCKET)
            throw  SetErrorMsgText("accept:", WSAGetLastError());
        cout << "Client connected: IP = " << inet_ntoa(clnt.sin_addr) << ", Port = " << htons(clnt.sin_port) << endl;

        char ibuf[50],                     //буфер ввода 
            obuf[50] = "sever: receive ";  //буфер вывода
        int  libuf = 0,                    //количество принятых байт
            lobuf = 0;                    //количество отправленных байь 

        while (true) {
            if ((libuf = recv(cS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
                throw  SetErrorMsgText("recv:", WSAGetLastError());
            if (ibuf[0] == '\0') break;
           
            cout << ibuf << endl;

            if ((lobuf = send(cS, obuf, strlen(obuf) + 1, NULL)) == SOCKET_ERROR)
                throw  SetErrorMsgText("send:", WSAGetLastError());

        }

        if (closesocket(sS) == SOCKET_ERROR)
            throw  SetErrorMsgText("closesocket:", WSAGetLastError());

        if (WSACleanup() == SOCKET_ERROR)
            throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << endl << errorMsgText;
    }
    system("pause");
}
