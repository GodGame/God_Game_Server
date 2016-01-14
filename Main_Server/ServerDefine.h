#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>

#define SERVER_PORT	9000
#define BUFFER_128	128
#define BUFFER_256	256	
#define BUFFER_512	512
#define BUFFER_1024	1024

using namespace std;

struct SOCKETINFO
{
	OVERLAPPED	overlapped;
	SOCKET		sock;
	char szBuf[BUFFER_512 + 1];
	int nRecvBytes;
	int	nSendBytes;
	WSABUF	wsabuf;
};

