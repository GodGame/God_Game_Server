#pragma once
class CGameManager;
class CGameObject;
class CLobby
{
	int m_nConnectNum;
	int m_nReadyCount;
	CGameObject* m_pGameObject;
	CGameManager* m_pGameManager;

public:
	void Initialize();
	bool IsAllPlayerReady();
	void SetGameObject(CGameObject* pGameObject);
public:
	CLobby();
	~CLobby();
};

