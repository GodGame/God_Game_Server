#include "Lobby.h"
#include "GameManager.h"
#include "GameObject.h"


void CLobby::Initialize()
{
	m_pGameManager = &CGameManager::GetInstance();
	m_nReadyCount = 0;
}

bool CLobby::IsAllPlayerReady()
{
	for (int i = 0; i < 5; i++)
	{
		if (m_pGameObject->m_Player[i].m_bISReady)
			m_nReadyCount += 1;
	}
	if ((m_nReadyCount % 5) == 0)
		return true;
	else
		return false;
}

void CLobby::SetGameObject(CGameObject * pGameObject)
{
	m_pGameObject = pGameObject;
}

CLobby::CLobby()
{
}


CLobby::~CLobby()
{
}
