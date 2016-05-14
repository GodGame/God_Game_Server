#pragma once
#include "stdafx.h"
#include "Protocol.h"
class CPlayer;

class CServerModule
{
private:
	vector<CPlayer*> m_vpPlayerList;
	int m_ID;
	SOCKET m_clientSocket;
	XMFLOAT3 m_xv3PlayerPosition;
public:
	WSABUF		m_send_wsaBuf;
	char		m_sendBuffer[1024];
	WSABUF		m_recv_wsaBuf;
	char		m_recvBuffer[1024];
	char		m_packetBuffer[1024];
	DWORD		m_inPacketSize;
	int			m_savedPacketSize;
	SOCKET		m_serverSocket;
public:
	void Initialize();
	void ReadPacket(SOCKET sock);
	void ProcessPacket(char* ptr);
	void ProcessSocketMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void error_display(char * msg, int err_no);
	void SetPlayer(CPlayer* _pPlayer);
	CPlayer* GetPlayer(int _id);
	int GetClientID();
public:
	CServerModule();
	~CServerModule();
public:
	static CServerModule & GetInstance();
};

