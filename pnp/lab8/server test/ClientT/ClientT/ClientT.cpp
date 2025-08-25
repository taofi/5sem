

#include "stdafx.h"
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib") 
#include <string>
#include <iostream>
#include <ctime>
#include <chrono>

#define IP_SERVER "192.168.220.251"

using namespace std;
using namespace chrono;
SOCKET  cC;
SOCKET  cS;

bool ICAN = true;



string  GetErrorMsgText(int code)
{
	string msgText;
	switch (code)
	{
	case WSAEINTR:          msgText = "WSAEINTR";         break;
	case WSAEACCES:         msgText = "WSAEACCES";        break;
	case WSAEFAULT:          msgText = "WSAEFAULT";         break;
	case WSAEINVAL:         msgText = "WSAEINVAL";        break;
	case WSAEMFILE:          msgText = "WSAEMFILE";         break;
	case WSAEWOULDBLOCK:         msgText = "WSAEWOULDBLOCK";        break;
	case WSAEINPROGRESS:          msgText = "WSAEINPROGRESS";         break;
	case WSAEALREADY:         msgText = "WSAEALREADY";        break;
	case WSAENOTSOCK:          msgText = "WSAENOTSOCK";         break;
	case WSAEDESTADDRREQ:         msgText = "WSAEDESTADDRREQ";        break;
	case WSAEMSGSIZE:          msgText = "WSAEMSGSIZE";         break;
	case WSAEPROTOTYPE:         msgText = "WSAEPROTOTYPE";        break;
	case WSAENOPROTOOPT:          msgText = "WSAENOPROTOOPT";         break;
	case WSAEPROTONOSUPPORT:         msgText = "WSAEPROTONOSUPPORT";        break;
	case WSAESOCKTNOSUPPORT:          msgText = "WSAESOCKTNOSUPPORT";         break;
	case WSAEOPNOTSUPP:         msgText = "WSAEOPNOTSUPP";        break;
	case WSAEPFNOSUPPORT:          msgText = "WSAEPFNOSUPPORT";         break;
	case WSAEAFNOSUPPORT:         msgText = "WSAEAFNOSUPPORT";        break;
	case WSAEADDRINUSE:          msgText = "WSAEADDRINUSE";         break;
	case WSAEADDRNOTAVAIL:         msgText = "WSAEADDRNOTAVAIL";        break;
	case WSAENETDOWN:          msgText = "WSAENETDOWN";         break;
	case WSAENETUNREACH:         msgText = "WSAENETUNREACH";        break;
	case WSAENETRESET:          msgText = "WSAENETRESET";         break;
	case WSAECONNABORTED:         msgText = "WSAECONNABORTED";        break;
	case WSAECONNRESET:          msgText = "WSAECONNRESET";         break;
	case WSAENOBUFS:         msgText = "WSAENOBUFS";        break;
	case WSAEISCONN:          msgText = "WSAEISCONN";         break;
	case WSAENOTCONN:         msgText = "WSAENOTCONN";        break;
	case WSAESHUTDOWN:          msgText = "WSAESHUTDOWN";         break;
	case WSAETIMEDOUT:         msgText = "WSAETIMEDOUT";        break;
	case WSAECONNREFUSED:          msgText = "WSAECONNREFUSED";         break;
	case WSAEHOSTDOWN:         msgText = "WSAEHOSTDOWN";        break;
	case WSAEHOSTUNREACH:          msgText = "WSAEHOSTUNREACH";         break;
	case WSAEPROCLIM:         msgText = "WSAEPROCLIM";        break;
	case WSASYSNOTREADY:          msgText = "WSASYSNOTREADY";         break;
	case WSAVERNOTSUPPORTED:         msgText = "WSAVERNOTSUPPORTED";        break;
	case WSANOTINITIALISED:          msgText = "WSANOTINITIALISED";         break;
	case WSAEDISCON:         msgText = "WSAEDISCON";        break;
	case WSATYPE_NOT_FOUND:          msgText = "WSATYPE_NOT_FOUND";         break;
	case WSAHOST_NOT_FOUND:         msgText = "WSAHOST_NOT_FOUND";        break;
	case WSATRY_AGAIN:          msgText = "WSATRY_AGAIN";         break;
	case WSANO_RECOVERY:         msgText = "WSANO_RECOVERY";        break;
	case WSANO_DATA:          msgText = "WSANO_DATA";         break;
	case WSA_INVALID_HANDLE:         msgText = "WSA_INVALID_HANDLE";        break;
	case WSA_INVALID_PARAMETER:          msgText = "WSA_INVALID_PARAMETER";         break;
	case WSA_IO_INCOMPLETE:         msgText = "WSA_IO_INCOMPLETE";        break;
	case WSA_IO_PENDING:          msgText = "WSA_IO_PENDING";         break;
	case WSA_NOT_ENOUGH_MEMORY:         msgText = "WSA_NOT_ENOUGH_MEMORY";        break;
	case WSA_OPERATION_ABORTED:          msgText = "WSA_OPERATION_ABORTED";         break;
	case WSAEINVALIDPROCTABLE:         msgText = "WSAEINVALIDPROCTABLE";        break;
	case WSAEINVALIDPROVIDER:          msgText = "WSAEINVALIDPROVIDER";         break;
	case WSAEPROVIDERFAILEDINIT:         msgText = "WSAEPROVIDERFAILEDINIT";        break;

	case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE"; break;
	default:                msgText = "***ERROR***";      break;
	};
	return msgText;
};

string  SetErrorMsgText(string msgText, int code)
{
	string error = msgText + GetErrorMsgText(code);
	error.append(" ").append(to_string(code));
	return error;
};

char* get_message(int msg)
{
	switch (msg)
	{
	case 1: 	return "Echo";
	case 2: 	return "Time"; 
	case 3: 	return "Random"; 
	case 4:		return "close";
	case 5:		return "exit";
	default:
		return "";
	}
}



bool GetServer(char* call, SOCKADDR_IN* from, int* flen, SOCKET* cC, SOCKADDR_IN* all) {
	char ibuf[200], obuf[200];
	int  libuf = 0, lobuf = 0;

	if ((lobuf = sendto(*cC, call, strlen(call) + 1, NULL, (sockaddr*)all, sizeof(*all))) == SOCKET_ERROR) throw  SetErrorMsgText("sendto:", WSAGetLastError());
	if ((libuf = recvfrom(*cC, ibuf, sizeof(ibuf), NULL, (sockaddr*)from, flen)) == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAETIMEDOUT) return false;
		else throw  SetErrorMsgText("recv:", WSAGetLastError());
	}
	if (strcmp(call, ibuf) == 0) return true;
	else return false;
}



BOOL WINAPI HandlerRountime(DWORD eventCode)
{
	ICAN = false;
	Sleep(3000);
	switch (eventCode)
	{
	case CTRL_CLOSE_EVENT:
		int lobuf;
		if ((lobuf = send(cC, get_message(5), 6, NULL)) == SOCKET_ERROR)
			cout << "������";
		Sleep(200);
		if (closesocket(cS) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
		return false;
		break;
	default:
		break;


	}

}

int _tmain(int argc, char* argv[])
{
	SetConsoleCtrlHandler(HandlerRountime, TRUE);
	WSADATA wsaData;
	setlocale(0, "rus");
	try
	{
		if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		if ((cS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());

		if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());

		SOCKADDR_IN serv;





		int connectionType = 0;
		char call[200];
		int port;
		cout << "1 - Connect automatically" << endl << "2 - Connect using server callname " << endl;
		cin >> connectionType;

		while (connectionType != 1 || connectionType != 2) {
			if (connectionType == 1) {
				serv.sin_family = AF_INET;
				serv.sin_port = htons(2000);
				serv.sin_addr.s_addr = inet_addr(IP_SERVER);
				if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
					throw  SetErrorMsgText("connect:", WSAGetLastError());
				break;
			}
			else if (connectionType == 2) {
				cout << "Enter callname: ";
				cin >> call;

				SOCKET SocketUDP;
				int optval = 1;
				if ((SocketUDP = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) throw SetErrorMsgText("Socket:", WSAGetLastError());
				if (setsockopt(SocketUDP, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR) throw SetErrorMsgText("Opt:", WSAGetLastError());

				SOCKADDR_IN all;
				all.sin_family = AF_INET;
				all.sin_port = htons(2000);
				all.sin_addr.s_addr = INADDR_BROADCAST;
				SOCKADDR_IN from;

				memset(&from, 0, sizeof(from));
				int lc = sizeof(from);

				bool bsr = GetServer(call, &from, &lc, &SocketUDP, &all);
				if (bsr == false) {
					throw "Server not found;";
				}
				else {
					serv.sin_addr.s_addr = from.sin_addr.s_addr;
					if (closesocket(SocketUDP) == SOCKET_ERROR) throw SetErrorMsgText("Closesocket:", WSAGetLastError());
					port = 2000;
					//cout << "Server port: ";
					//cin >> port;
				}


				serv.sin_family = AF_INET;
				serv.sin_port = htons(2000);
				if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR) throw "Connection error";

				break;
			}
			else {
				cout << "Enter correct connection type (1 or 2)\n";
				cin >> connectionType;

			}
		}

		




		while (true) {
			char message[50],
				obuf[50];
			int  libuf = 0,
				lobuf = 0;

			puts("Choose");
			int service;
			puts("1 - Echo\n2 - Time\n3 - Random\n4 - close socket");

			scanf("%d", &service);
			char* outMessage = new char[5];
			strcpy(outMessage, get_message(service));

			if ((lobuf = send(cC, outMessage, strlen(outMessage) + 1, NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("send:", WSAGetLastError());

			printf("sended: %s\n", outMessage);

			if ((libuf = recv(cC, message, sizeof(message), NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("recv:", WSAGetLastError());

			if (service == 4) {
				break;
			}

			if (!strcmp(message, "TimeOUT")) {
				puts("time out");
				return -1;
			}
			if (service == 1)
			{
				for (int j = 15; j >= 0; --j) {
					if (!ICAN) {
						if (closesocket(cS) == SOCKET_ERROR)
							throw  SetErrorMsgText("closesocket:", WSAGetLastError());
						if (WSACleanup() == SOCKET_ERROR)
							throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
					}
					Sleep(1000);
					sprintf(outMessage, "%d", j);
					if ((lobuf = send(cC, outMessage, strlen(outMessage) + 1, NULL)) == SOCKET_ERROR)
						throw  SetErrorMsgText("send:", WSAGetLastError());
					printf("send: %s\n", outMessage);
					if ((libuf = recv(cC, message, sizeof(message), NULL)) == SOCKET_ERROR)
						throw  SetErrorMsgText("recv:", WSAGetLastError());
					printf("receive: %s\n", message);
				}
				//if ((lobuf = send(cC, "\0", strlen("\0") + 1, NULL)) == SOCKET_ERROR)
					//throw  SetErrorMsgText("send:", WSAGetLastError());
				//return 0;
			}
			else if (service == 2 || service == 3) {
				printf("receive: %s\n", message);
				//if (!strcmp(message, "finish")) {
					//return 0;
				//}
			}
		}
		
		if (closesocket(cS) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		printf("\n%s", errorMsgText.c_str());
	}
	system("pause");
	return 0;
}
