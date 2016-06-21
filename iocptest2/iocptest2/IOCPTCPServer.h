#pragma once
#include "Protocol.h"
#include "Common.h"
#include "Struct.h"
#include "Timer.h"
#include "enum.h"
#include "TimeEvent.h"
#include "Lobby.h"

#define ROUND_END_TIME 100
class CGameObject;
class CGameTimer;
class CGameManager;
class CGameObjectTest;
class CIOCPTCPServer
{

	bool _bIsShutdown;
	HANDLE _hLogicProcessCp;
	HANDLE _hWorkCp;
	bool m_bTimerSwitch;
	mutex global_lock;
	atomic_int m_UserID;
	//vector<CGameObject*> _Player;
	CGameObject* _pGameObject;
	CGameTimer* _pTimer;
	CGameManager* _pGameManager;
	float m_fFrameTime;
	default_random_engine randomEngine;
	uniform_int_distribution<int> randomX;
	uniform_int_distribution<int> randomZ;

	CGameObjectTest* m_pGameObjectTest;
	//float randomX;
	//float randomY;

public:
	CTimeEvent m_TimeEvent;
	HANDLE _hIOCP;

public:
	int m_nRoundTime;

public:
	int GetNewClientID();
	bool StartServer();
	void InitServer();
	void AcceptThread();
	void WorkerThread();
	void TimeThread();
	void Timer_Thread();
	HANDLE GetIOCP() { return _hIOCP; }

	bool ServerRunCheck();
	void ShutdownServer();
	void SetShutdown(bool _flag) { _bIsShutdown = _flag; }
public:
	void SendPacket(unsigned char* packet, int id);
	void ProcessPacket(unsigned char* packet, int id);
public:
	void SendGameState(SERVER_GAME_STATE eGamestate, int id);
	void SendTime(int time, int round ,int id);
public:
	void Process_Event(event_type curr_event);


	void RoundTimer(int id);
public:
	
	void SetFrameTime(float _timeElapsed) { m_fFrameTime = _timeElapsed; };
public:
	void Process_Move_Packet(unsigned char* packet, int id);
	void Process_StateSet_Packet(unsigned char* packet, int id);
	void Process_Damage_Packet(unsigned char* packet, int id);
	//void Process_Calc_Packet(unsigned char* packet, int id);
	void error_display(char *msg, int err_no);

	static CIOCPTCPServer & GetInstance();
public:
	CIOCPTCPServer();
	~CIOCPTCPServer();
};

//CS_CHAR_DAMAGE