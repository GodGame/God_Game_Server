#pragma once
#include "Protocol.h"

class CGameManager
{
public:
	int m_nRoundTime;
	int m_nReadyTime;
	int m_nConnectCount;
	int m_nGameRound;
	int m_nEndTime;
	int m_nDominatePlayer;
public:
	SERVER_GAME_STATE m_eGameState;


public:
	void Initialize();
public:
	int GetRoundTime();
	int GetEndTime();
	int GetConnectPlayerNum();
	int GetGameRound();
	int GetReadyTime();
	SERVER_GAME_STATE GetGameState();

public:
	CGameManager();
	~CGameManager();
public:
	static CGameManager & GetInstance();
};

