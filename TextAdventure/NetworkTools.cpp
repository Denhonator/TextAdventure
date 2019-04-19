#include "NetworkTools.h"

Server::Server()
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
	}
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("1.1.1.1");
	clientService.sin_port = htons(DEFAULT_PORT);

	iSendResult = 0;
	connected = 0;
}

Server::~Server()
{
}

int __cdecl Server::CreateServer()
{
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	iResult = bind(ListenSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	TCPSocket = accept(ListenSocket, NULL, NULL);
	if (TCPSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ListenSocket);
	connected = 's';
	return 0;
}

int __cdecl Server::CreateClient(std::string IP)
{
	TCPSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	clientService.sin_addr.s_addr = inet_addr(IP.c_str());

	iResult = connect(TCPSocket, (SOCKADDR*)& clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		closesocket(TCPSocket);
		TCPSocket = INVALID_SOCKET;
	}

	if (TCPSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	connected = 'c';
	return 0;
}

std::string Server::Receive()
{
	char recvbuf[DEFAULT_BUFLEN];
	iResult = recv(TCPSocket, recvbuf, DEFAULT_BUFLEN, 0);
	if(iResult>0)
		return recvbuf;
	Shutdown();
	return ":Disconnected";
}

int __cdecl Server::Send(std::string mes)
{
	iSendResult = send(TCPSocket, mes.c_str(), DEFAULT_BUFLEN, 0);
	return iSendResult;
}

int __cdecl Server::Shutdown()
{
	connected = false;
	iResult = shutdown(TCPSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(TCPSocket);
		WSACleanup();
		return 1;
	}

	closesocket(TCPSocket);
	WSACleanup();
	return 0;
}