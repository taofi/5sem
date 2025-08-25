#include <iostream>
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <tchar.h>
#include "Winsock2.h"
#include "Errors.h"
#include <string>
#include <list>
#include <time.h>
#include <iostream>
#define AS_SQUIRT 10

SOCKET sS;
int serverPort;
char dllName[50];
char namedPipeName[10];

DWORD WINAPI AcceptServer(LPVOID pPrm);  // прототипы функций 
DWORD WINAPI ConsolePipe(LPVOID pPrm);
DWORD WINAPI GarbageCleaner(LPVOID pPrm);

CRITICAL_SECTION scListContact;



HANDLE hAcceptServer,    // дескриптор потока AcceptServer
hConsolePipe,     // дескриптор потока ConsolePipe 
hGarbageCleaner;   // дескриптор потока GarbageCleaner


enum TalkersCommand
{
    START, STOP, EXIT, STATISTICS, WAIT, SHUTDOWN, GETCOMMAND, LOAD_LIB, UNLOAD_LIB
};
volatile TalkersCommand  previousCommand = GETCOMMAND;

#pragma region Contact
struct Contact
{
    enum TE
    {
        EMPTY,
        ACCEPT,
        CONTACT
    }    type;
    enum ST
    {
        WORK,
        ABORT,
        TIMEOUT,
        FINISH
    }      sthread;

    SOCKET      s;
    SOCKADDR_IN prms;
    int         lprms;
    HANDLE      hthread;
    HANDLE      htimer;
    HANDLE		serverHThtead;

    char msg[50];
    char srvname[15];

    Contact(TE t = EMPTY, const char* namesrv = "")
    {
        ZeroMemory(&prms, sizeof(SOCKADDR_IN));
        lprms = sizeof(SOCKADDR_IN);
        type = t;
        strcpy(srvname, namesrv);
        msg[0] = 0x00;
    };

    void SetST(ST sth, const char* m = "")
    {
        sthread = sth;
        strcpy(msg, m);
    }
};
typedef list<Contact> ListContact;
#pragma endregion
ListContact contacts;

bool AcceptCycle(int squirt)
{
    bool rc = false;
    Contact c(Contact::ACCEPT, "EchoServer");

    while (squirt-- > 0 && rc == false)
    {
        if ((c.s = accept(sS, (sockaddr*)&c.prms, &c.lprms)) == INVALID_SOCKET)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
                throw  SetErrorMsgText("accept:", WSAGetLastError());
        }
        else
        {
            rc = true;               // подключился
            EnterCriticalSection(&scListContact);
            contacts.push_front(c);
            LeaveCriticalSection(&scListContact);
        }
    }
    return rc;
};

void CommandsCycle(TalkersCommand& cmd)      // цикл обработки команд
{
    int  squirt = 0;
    while (cmd != EXIT)     // цикл обработки команд консоли и подключений
    {
        switch (cmd)
        {
            //.........................................................
        case START: cmd = GETCOMMAND; // возобновить подключение клиентов
            squirt = AS_SQUIRT; 
            break;
        case STOP:  cmd = GETCOMMAND; // остановить подключение клиентов   
            squirt = 0;
            break;
            //........................................................
        };
        if (AcceptCycle(squirt))   //цикл  запрос/подключение (accept) 
        {
            cmd = GETCOMMAND;
            //.... запуск потока EchoServer.......................
            //.... установка ожидающего таймера для процесса EchoServer ...      
        }
        else SleepEx(0, TRUE);    // выполнить асинхронные процедуры 
    };
};

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        serverPort = atoi(argv[1]);
        strcpy(namedPipeName, "Tube");
    }
    else
    {
        serverPort = 3000;
        strcpy(namedPipeName, "Tube");
    }


    printf("Server port %d\n", serverPort);
    InitializeCriticalSection(&scListContact);

    volatile TalkersCommand  cmd = START;      // команды сервера 
    hAcceptServer = CreateThread(NULL, NULL, AcceptServer,
        (LPVOID)&cmd, NULL, NULL),
        hConsolePipe = CreateThread(NULL, NULL, ConsolePipe,
            (LPVOID)&cmd, NULL, NULL),
        hGarbageCleaner = CreateThread(NULL, NULL, GarbageCleaner,
            (LPVOID)NULL, NULL, NULL);

    WaitForSingleObject(hAcceptServer, INFINITE);
    CloseHandle(hAcceptServer);                        WaitForSingleObject(hConsolePipe, INFINITE);
    CloseHandle(hConsolePipe);
    WaitForSingleObject(hGarbageCleaner, INFINITE);   CloseHandle(hConsolePipe);
    DeleteCriticalSection(&scListContact);

    return 0;
};


DWORD WINAPI AcceptServer(LPVOID pPrm)
{
    DWORD rc = 0;
    WSADATA wsaData;

    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw  SetErrorMsgText("Startup:", WSAGetLastError());

        u_long nonblk;
        if (ioctlsocket(sS, FIONBIO, &(nonblk = 1)) == SOCKET_ERROR)
            throw SetErrorMsgText("ioctlsocket:", WSAGetLastError());
        CommandsCycle(*((TalkersCommand*)pPrm));

        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup:", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        printf("\n%s\n", errorMsgText.c_str());
    }
    puts("shutdown acceptServer");
    ExitThread(rc);
}
DWORD WINAPI ConsolePipe(LPVOID pPrm)
{

}
DWORD WINAPI GarbageCleaner(LPVOID pPrm);
{

}