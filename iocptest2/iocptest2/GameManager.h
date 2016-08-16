#pragma once
#include "Protocol.h"
#include <mutex>

class CGameManager
{
public:
	int m_nRoundTime;
	int m_nReadyTime;
	int m_nConnectCount;
	int m_nGameRound;
	int m_nEndTime;
	int m_nDominatePlayer;
	int m_nDeathMatchTime;
	int m_nStageNum;
	int m_nReadyNum;
	bool m_bIsDeathMatch;
	float m_nWaterHeight;
	std::mutex m_lock;
public:
	SERVER_GAME_STATE m_eGameState;


public:
	void Initialize();

public:
	void StartDeathMatch();
	void EndDeathMatch();
public:
	void SetStage(int nStage);
	void AddReadyNum() {
		m_lock.lock(); m_nReadyNum++; m_lock.unlock();
	};
public:
	int GetRoundTime();
	int GetEndTime();
	int GetConnectPlayerNum();
	int GetGameRound();
	int GetReadyTime();
	bool GetDeathMatchState() { return m_bIsDeathMatch; }
	int GetDeathMatchTime() { return m_nDeathMatchTime; }
	float GetWaterHeight() { return m_nWaterHeight; }
	int GetStage() { return m_nStageNum; }
	int GetReadyPlayerNum() { return m_nReadyNum; }
	SERVER_GAME_STATE GetGameState();

public:
	CGameManager();
	~CGameManager();
public:
	static CGameManager & GetInstance();
};

