#include <iostream>
#include "errors.h"
#include "Winsock2.h"              
#pragma comment(lib, "WS2_32.lib") 
#pragma warning (disable: 4996)

using namespace std;


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");

    WSADATA wsaData;
    try
    {
        if (argc != 2) {
            std::cerr << "Usage: ServerU.exe <port>" << std::endl;
            //return 1; // Ошибка, если параметр не передан
        }

        //int port = std::atoi(argv[1]);
        int port;
        cin >> port;

        
       /* if (port <= 0) {
            std::cerr << "Invalid port number: " << argv[1] << std::endl;
            return 1;
        }*/
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw  SetErrorMsgText("Startup:", WSAGetLastError());

        cout << port << endl;
        SOCKET  sS;
        if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw  SetErrorMsgText("socket:", WSAGetLastError());
        int bufSize = 1;  
        if (setsockopt(sS, SOL_SOCKET, SO_RCVBUF, (char*)&bufSize, sizeof(bufSize)) == SOCKET_ERROR)
        {
            throw SetErrorMsgText("setsockopt SO_RCVBUF:", WSAGetLastError());
        }

        if (setsockopt(sS, SOL_SOCKET, SO_SNDBUF, (char*)&bufSize, sizeof(bufSize)) == SOCKET_ERROR)
        {
            throw SetErrorMsgText("setsockopt SO_SNDBUF:", WSAGetLastError());
        }


        SOCKADDR_IN serv;              // параметры  сокета sS
        serv.sin_family = AF_INET;     // используется IP-адресация  
        serv.sin_port = htons(port);    // порт 2000 
        serv.sin_addr.s_addr = INADDR_ANY; // адрес сервера 
        if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw  SetErrorMsgText("bind:", WSAGetLastError());

        SOCKADDR_IN clnt;               // параметры  сокета клиента
        memset(&clnt, 0, sizeof(clnt));   // обнулить память
        int lclnt = sizeof(clnt);    // размер SOCKADDR_IN

        
        char ibuf[50]{},                      //буфер ввода 
            obuf[50]{};                       //буфер вывода
        int  libuf = 0,                     //количество принятых байт
            lobuf = 0;
        int count = 0;
        while (true)
        {
            if ((libuf = recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clnt, &lclnt)) == SOCKET_ERROR)
                throw  SetErrorMsgText("recv:", WSAGetLastError());
            count++;
            Sleep(200);
            if (ibuf[0] == '\0')
            {
                cout << "----------  " << count << "  ----------" << endl;
                count = 0;
            }


            cout << ibuf << endl;

            //strcpy(obuf, ibuf); 
            //if (lobuf = sendto(sS, obuf, strlen(obuf) + 1, NULL, (sockaddr*)&clnt, lclnt) == SOCKET_ERROR)
            //    throw SetErrorMsgText("send:", WSAGetLastError());
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
