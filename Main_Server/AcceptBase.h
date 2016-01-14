#pragma once
#include "ServerDefine.h"
class cAcceptBase
{
protected:
	SOCKET	_ListenSocket;
	SOCKADDR_IN _Addr;
public:
	cAcceptBase();
	virtual ~cAcceptBase();
	//virtual	bool OnAccept()
protected:
	bool	Init(int nType, int nProtocol, SOCKADDR_IN addr, int nWaitSocket = SOMAXCONN);

};

