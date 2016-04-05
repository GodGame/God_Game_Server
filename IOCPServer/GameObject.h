#pragma once
#include "define.h"
class CGameObject
{
	player m_Player[10];

public:
	void InitPlayer(int id);
	int GetClientID();
	player& GetPlayer(int id);
public:
	CGameObject();
	~CGameObject();
};

