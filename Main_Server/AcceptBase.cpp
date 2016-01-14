#include "AcceptBase.h"


cAcceptBase::cAcceptBase()
{
	_ListenSocket = INVALID_SOCKET;
}


cAcceptBase::~cAcceptBase()
{
	closesocket(_ListenSocket);
	_ListenSocket = INVALID_SOCKET;
}

bool cAcceptBase::Init(int nType, int nProtocol, SOCKADDR_IN addr, int nWaitSocket /*= SOMAXCONN*/)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	_Addr = addr;
	_ListenSocket = WSASocket(AF_INET, nType, nProtocol, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (_ListenSocket == INVALID_SOCKET)
		return false;
	if (bind(_ListenSocket, (SOCKADDR*)&_Addr, sizeof(_Addr)) == SOCKET_ERROR)
		return false;
	if (listen(_ListenSocket, nWaitSocket) == SOCKET_ERROR)
		return false;
	return true;
}
