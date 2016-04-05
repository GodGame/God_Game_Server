#include "GameObject.h"



void CGameObject::InitPlayer(int id)
{
	m_Player[id].x = 1180;
	m_Player[id].y = 0;
	m_Player[id].z = 255;
	m_Player[id].overlapped_ex.curr_packet_size = 0;
	m_Player[id].overlapped_ex.is_recv = true;
	ZeroMemory(&m_Player[id].overlapped_ex.overlapped, sizeof(WSAOVERLAPPED));
	m_Player[id].overlapped_ex.prev_data_size = 0;
	m_Player[id].overlapped_ex.wsabuf.buf = m_Player[id].overlapped_ex.IOCPbuf;
	m_Player[id].overlapped_ex.wsabuf.len = MAX_BUFF_SIZE;

}

int CGameObject::GetClientID()
{
	for (int i = 0; i < 10; i++)
	{
		if (m_Player[i].in_use)
			continue;
		return i;
	}
	cout << "MAXUSERFULL" << endl;
	exit(-1);
}

player& CGameObject::GetPlayer(int id)
{
	return m_Player[id];
}

CGameObject::CGameObject()
{
	for (int i = 0; i < 10;i++)
		m_Player[i].in_use = false;
}


CGameObject::~CGameObject()
{
}
