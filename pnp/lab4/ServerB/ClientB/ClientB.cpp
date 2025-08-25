#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "errors.h"
#include <iostream>
#include <ctime>
#include "Winsock2.h"  
#pragma comment(lib, "WS2_32.lib")   // экспорт  WS2_32.dll

using namespace std;

bool  GetServerByName(const char* name, const char* call, struct sockaddr* from, int* flen);


//SOCKET sock;

int main()
{

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    WSADATA wsaData;
    SOCKADDR_IN from;
    char name[] = "ServerName";
    memset(&from, 0, sizeof(from));
    int lfrom = sizeof(from);
    int optval = 1;
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());
        cout << "CLIENT" << endl;
        short port = 2000;

       /* if ((sock = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket: ", WSAGetLastError());*/

        if (GetServerByName("HOME", name, (sockaddr*)&from, &lfrom))
        {
            cout << "Connected server: " << endl;
            cout << "IP: " << inet_ntoa(from.sin_addr) << endl;
            cout << "Port: " << htons(from.sin_port) << endl;
        }

        /*if (closesocket(sock) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket: ", WSAGetLastError());*/
        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << "WSAGetLastError: " << errorMsgText << endl;
    }


}


bool GetServerByName(const char* name, const char* call, struct sockaddr* from, int* flen)
{
    SOCKET sS;

    if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
        throw SetErrorMsgText("socket: ", WSAGetLastError());

    hostent* host = gethostbyname(name);
    if (!host)
        throw SetErrorMsgText("gethostbyname:", WSAGetLastError());

    char* ip_addr = inet_ntoa(*(in_addr*)(host->h_addr));

    cout << "Имя хоста: " << host->h_name << endl;
    cout << "IP сервера: " << ip_addr << endl;

    ((SOCKADDR_IN*)from)->sin_family = AF_INET;
    ((SOCKADDR_IN*)from)->sin_port = htons(2000);
    ((SOCKADDR_IN*)from)->sin_addr.s_addr = inet_addr(ip_addr);
    int lbuf;
    char ibuf[50];
    int optval = 1;


    if (setsockopt(sS, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval)) == SOCKET_ERROR)
        throw SetErrorMsgText("opt: ", WSAGetLastError());

    if (lbuf = sendto(sS, call, strlen(call) + 1, NULL, from, *flen) == SOCKET_ERROR)
        throw SetErrorMsgText("sendto: ", WSAGetLastError());
    while (true) {


        if (lbuf = recvfrom(sS, ibuf, sizeof(ibuf), NULL, from, flen) == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT)
                return false;
            else
                throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
        }
        cout << ibuf << endl;
    }
    closesocket(sS);

    return true;

}