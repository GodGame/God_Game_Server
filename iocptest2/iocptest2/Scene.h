#pragma once
#include "Common.h"
#include "Struct.h"
class CScene
{
protected:
	HANDLE m_hIOCP;
	OVERLAPPED_EX* m_overlap;
	DWORD m_dwKey;
	DWORD m_dwSize;

public:
	//virtual void Progress();
	//virtual void SendPacket();
public:
	CScene();
	virtual ~CScene();
};

