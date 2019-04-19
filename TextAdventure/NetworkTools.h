#pragma once
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
#pragma warning(disable:4996)

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT 27015

class Server {
public:
	Server();
	~Server();
	int __cdecl CreateServer();
	int __cdecl CreateClient(std::string IP = "127.0.0.1");
	std::string Receive();
	int __cdecl Send(std::string mes);
	int __cdecl Shutdown();
	char isConnected() { return connected; };
private:
	char connected;
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET TCPSocket = INVALID_SOCKET;

	sockaddr_in clientService;
	int iSendResult;
};