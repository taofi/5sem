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
            std::cerr << "Usage: ClientU.exe <port>" << std::endl;
           // return 1; // Ошибка, если параметр не передан
        }

       // int port = std::atoi(argv[1]);

        int port;
        cin >> port;

        /*if (port <= 0) {
            cin >> port;
            std::cerr << "Invalid port number: " << argv[1] << std::endl;
            return 1; 
        }*/
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw  SetErrorMsgText("Startup:", WSAGetLastError());

        cout << port << endl;
        SOCKET  cC;                          // серверный сокет
        if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw  SetErrorMsgText("socket:", WSAGetLastError());

        SOCKADDR_IN serv;              // параметры  сокета сервера
        serv.sin_family = AF_INET;    // используется ip-адресация  
        serv.sin_port = htons(port);   // порт 2000
        serv.sin_addr.s_addr = inet_addr("192.168.110.255"); // адрес сервера  

        char ibuf[50]{},                      //буфер ввода 
            obuf[50]{};                       //буфер вывода
        int  libuf = 0,                     //количество принятых байт
            lobuf = 0,                      //количество отправленных байт
            count = 0;                      //количество иттераций
        int lserv = sizeof(serv);

        cout << "Enter a number of itterations: ";
        cin >> count;
        //count = 100000;

        char num[10],
            in[50];
        int t = clock();

        do
        {
            char out[50] = "Hellosrdfgasdasdf";
            _itoa(count, num, 10);
            strcat(out, num);


            if ((lobuf = sendto(cC, out, strlen(out) + 1, NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR) 
                throw  SetErrorMsgText("send:", WSAGetLastError());

            /*if ((libuf = recvfrom(cC, in, sizeof(in), NULL, (sockaddr*)&serv, &lserv)) == SOCKET_ERROR)
                throw  SetErrorMsgText("recv:", WSAGetLastError());
            cout << in << endl;*/
            cout << "sended " << out << endl;


        } while (--count);
        char out[50] = "\0";
        if ((lobuf = sendto(cC, out, strlen(out) + 1, NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR) throw  SetErrorMsgText("send:", WSAGetLastError());
        cout << "Ticks: " << clock() - t << endl;
        cout << "\nThe End" << endl;
        
        if (closesocket(cC) == SOCKET_ERROR) throw  SetErrorMsgText("closesocket:", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << endl << errorMsgText;
    }
    system("pause");

}
