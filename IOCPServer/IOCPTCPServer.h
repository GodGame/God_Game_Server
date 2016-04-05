#pragma once
#include "define.h"
class CGameObject;
class CIOCPTCPServer
{
	bool _bIsShutdown;
	HANDLE _hLogicProcessCp;
	HANDLE _hWorkCp;
	HANDLE _hIOCP;
	//vector<CGameObject*> _Player;
	CGameObject* _pPlayer;
public:
	bool StartServer();
	void InitServer();
	void AcceptThread();
	void WorkerThread();

	bool ServerRunCheck();
	void ShutdownServer();
	void SetShutdown(bool _flag) { _bIsShutdown = _flag; }
public:
	void SendPacket(void* packet, unsigned id);
	void ProcessPacket(char* packet, unsigned id);

public:
	void Process_Move_Packet(char* packet, unsigned id);
public:
	CIOCPTCPServer();
	~CIOCPTCPServer();
};

