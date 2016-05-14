#include "stdafx.h"
#include "ServerModule.h"
#include "Player.h"
void CServerModule::error_display(char *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("%s", msg);
	wprintf(L"에러 : %s\n", lpMsgBuf);
	LocalFree(lpMsgBuf);
}
void CServerModule::SetPlayer(CPlayer * _pPlayer)
{
	m_vpPlayerList.push_back(_pPlayer);
}
CPlayer * CServerModule::GetPlayer(int _id)
{
	return m_vpPlayerList[_id];
}
int CServerModule::GetClientID()
{
	return m_ID;
}
void CServerModule::Initialize()
{
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	m_clientSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(9000);
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int Result = WSAConnect(m_clientSocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr), NULL, NULL, NULL, NULL);
	if (SOCKET_ERROR == Result)
	{
		int error_no = WSAGetLastError();
		error_display(__FUNCTION__" : WSAConnect", error_no);
		//cout << "Socket" << endl;
	}
//	Result = WSAAsyncSelect(m_clientSocket, g_hWnd, WM_SOCKET, FD_CLOSE | FD_READ);
	if (SOCKET_ERROR == Result)
	{
		int error_no = WSAGetLastError();
		error_display(__FUNCTION__" : WSAAsyncSelect", error_no);
	}
	m_send_wsaBuf.buf = m_sendBuffer;
	m_send_wsaBuf.len = 1024;
	m_recv_wsaBuf.buf = m_recvBuffer;
	m_recv_wsaBuf.len = 1024;
}

void CServerModule::ReadPacket(SOCKET sock)
{
	DWORD iobyte, ioflag = 0;
//	if (m_pPlayer == nullptr)
//		return;
	int ret = WSARecv(m_clientSocket, &m_recv_wsaBuf, 1, &iobyte, &ioflag, NULL, NULL);
	if (ret == SOCKET_ERROR) {
		int err_code = WSAGetLastError();
		if (WSAGetLastError() != WSA_IO_PENDING)
			error_display("RecvError", err_code);
		//printf("Recv Error [%d]\n", err_code);
	}

	BYTE *ptr = reinterpret_cast<BYTE *>(m_recvBuffer);

	while (0 != iobyte) {
		if (0 == m_inPacketSize) m_inPacketSize = ptr[0];
		if (iobyte + m_savedPacketSize >= m_inPacketSize) {
			memcpy(m_packetBuffer + m_savedPacketSize, ptr, m_inPacketSize - m_savedPacketSize);
			ProcessPacket(m_packetBuffer);
			ptr += m_inPacketSize - m_savedPacketSize;
			iobyte -= m_inPacketSize - m_savedPacketSize;
			m_inPacketSize = 0;
			m_savedPacketSize = 0;
		}
		else {
			memcpy(m_packetBuffer + m_savedPacketSize, ptr, iobyte);
			m_savedPacketSize += iobyte;
			iobyte = 0;
		}
	}
}

void CServerModule::ProcessPacket(char * ptr)
{
	static bool first_time = true;
	switch (ptr[1])
	{
	case SC_PUT_PLAYER:
	{
		sc_packet_put_player *my_packet = reinterpret_cast<sc_packet_put_player *>(ptr);
		int id = my_packet->id;
		if (first_time) {
			first_time = false;
			m_ID = id;
		}
		if (id == m_ID) {
			//m_pPlayer->SetPosition();
			m_xv3PlayerPosition.x = my_packet->x;
			m_xv3PlayerPosition.y = my_packet->y;
			m_xv3PlayerPosition.z = my_packet->z;
			if (m_vpPlayerList[m_ID] != NULL)
				m_vpPlayerList[m_ID]->SetPosition(m_xv3PlayerPosition);
		
			cout << "SC_PUT_PLAYER " << "x : " << m_xv3PlayerPosition.x << "y : " << m_xv3PlayerPosition.y << "z : " << m_xv3PlayerPosition.z << endl;
			//m_pIngamePlayer
		}
		else
		{
			XMFLOAT3 OtherPlayer;
			OtherPlayer.x = my_packet->x;
			OtherPlayer.y = my_packet->y;
			OtherPlayer.z = my_packet->z;
			if (m_vpPlayerList[m_ID] != NULL)
				m_vpPlayerList[m_ID]->SetPosition(m_xv3PlayerPosition);
			/*	if (m_pPlayer[id] != NULL)
			m_pPlayer[id]->SetPosition(OtherPlayer);*/
		}

		break;
	}
	case SC_POS:
	{
		sc_packet_pos *my_packet = reinterpret_cast<sc_packet_pos *>(ptr);
		int other_id = my_packet->id;
		if (other_id == m_ID) {
			//	g_left_x = my_packet->x - 4;
			//	g_top_y = my_packet->y - 4;
			m_xv3PlayerPosition.x = my_packet->x;
			m_xv3PlayerPosition.y = my_packet->y;
			m_xv3PlayerPosition.z = my_packet->z;
			/*if (m_pPlayer[m_ID] != NULL)
			m_pPlayer[m_ID]->SetPosition(m_xv3PlayerPosition);*/
			if (m_vpPlayerList[other_id] != NULL)
				m_vpPlayerList[other_id]->SetPosition(m_xv3PlayerPosition);
		}
		else
		{
			XMFLOAT3 OtherPlayer;
			OtherPlayer.x = my_packet->x;
			OtherPlayer.y = my_packet->y;
			OtherPlayer.z = my_packet->z;
			/*if (m_pPlayer[other_id] != NULL)
			m_pPlayer[other_id]->SetPosition(OtherPlayer);*/
			if (m_vpPlayerList[other_id] != NULL)
				m_vpPlayerList[other_id]->SetPosition(OtherPlayer);
		}

		//cout << "SC_POS " << "x : " << m_xv3PlayerPosition.x << "z : " << m_xv3PlayerPosition.z << endl;
		cout << "myCLient ID : " << m_ID << endl;
		break;
	}

	case SC_REMOVE_PLAYER:
	{
		sc_packet_remove_player *my_packet = reinterpret_cast<sc_packet_remove_player *>(ptr);
		int other_id = my_packet->id;

		break;
	}
	case SC_CHAT:
	{
		/*	sc_packet_chat *my_packet = reinterpret_cast<sc_packet_chat *>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid) {
		wcsncpy(g_player.message, my_packet->message, 256);
		g_player.message_time = GetTickCount();
		}
		else if (other_id < NPC_START) {
		wcsncpy(skelaton[other_id].message, my_packet->message, 256);
		skelaton[other_id].message_time = GetTickCount();
		}
		else {
		wcsncpy(npc[other_id - NPC_START].message, my_packet->message, 256);
		npc[other_id - NPC_START].message_time = GetTickCount();
		}
		break;
		*/
	}
	//	case 0:
	//		break;
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}

CServerModule::CServerModule()
{
}


CServerModule::~CServerModule()
{
}

CServerModule & CServerModule::GetInstance()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	static CServerModule instance;
	return instance;
}

void CServerModule::ProcessSocketMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	//cout << "socketMEssage" << endl;
	if (WSAGETSELECTERROR(lParam)) {
		closesocket((SOCKET)wParam);
		return;
	}
	switch (WSAGETSELECTEVENT(lParam)) {
	case FD_READ:
		ReadPacket((SOCKET)wParam);
		return;
	case FD_CLOSE:
		closesocket((SOCKET)wParam);
		exit(-1);
		break;
	}

}
