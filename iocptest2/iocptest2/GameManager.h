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
	bool m_bIsDeathMatch;
	int m_nDeathMatchTime;
	float m_nWaterHeight;
public:
	SERVER_GAME_STATE m_eGameState;


public:
	void Initialize();

public:
	void StartDeathMatch();
	void EndDeathMatch();

public:
	int GetRoundTime();
	int GetEndTime();
	int GetConnectPlayerNum();
	int GetGameRound();
	int GetReadyTime();
	bool GetDeathMatchState() { return m_bIsDeathMatch; }
	int GetDeathMatchTime() { return m_nDeathMatchTime; }
	float GetWaterHeight() { return m_nWaterHeight; }
	SERVER_GAME_STATE GetGameState();

public:
	CGameManager();
	~CGameManager();
public:
	static CGameManager & GetInstance();
};

