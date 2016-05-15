#include "GameManager.h"



void CGameManager::Initialize()
{
	//if (m_eGameState == STATE_ROUND_END)
		//m_eGameState = STATE_ROUND_CHANGE;
	//else
		//m_eGameState = STATE_READY;
	m_eGameState = STATE_TOTAL_NUM;
	m_nRoundTime = LIMIT_ROUND_TIME;//0;
	m_nReadyTime = 5;
	m_nConnectCount = 0;
	m_nEndTime = 5;
}


int CGameManager::GetRoundTime()
{
	return m_nRoundTime;
}

int CGameManager::GetEndTime()
{
	return m_nEndTime;
}

int CGameManager::GetConnectPlayerNum()
{
	return m_nConnectCount;
}

int CGameManager::GetGameRound()
{
	return m_nGameRound;
}

int CGameManager::GetReadyTime()
{
	return m_nReadyTime;
}

SERVER_GAME_STATE CGameManager::GetGameState()
{
	return m_eGameState;
}

CGameManager::CGameManager()
{
	m_nRoundTime = LIMIT_ROUND_TIME;
	m_nReadyTime = 4;
	m_nConnectCount = 0;
	m_eGameState = STATE_TOTAL_NUM;
	m_nGameRound = 1;
	m_nEndTime = 5;
}


CGameManager::~CGameManager()
{
}
CGameManager & CGameManager::GetInstance()
{
	static CGameManager instance;
	return instance;
}
