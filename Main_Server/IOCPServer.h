#pragma once
#include "ServerDefine.h"
#include "AcceptBase.h"
class cIOCPServer	: public cAcceptBase
{
private:


public:
	cIOCPServer();
	~cIOCPServer();

public:
	virtual bool	StartServer();
	virtual	bool	CloseServer();
	virtual	void	MainRun();


};

