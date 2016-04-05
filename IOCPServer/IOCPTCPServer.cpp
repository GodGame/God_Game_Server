#include "IOCPTCPServer.h"
#include "GameObject.h"



CIOCPTCPServer::CIOCPTCPServer()
{
	_hLogicProcessCp = INVALID_HANDLE_VALUE;
	_hWorkCp = INVALID_HANDLE_VALUE;
	//_bIsShutdown = true;
}


CIOCPTCPServer::~CIOCPTCPServer()
{
	delete _pPlayer;
}
bool CIOCPTCPServer::StartServer()
{
	/*cout << "Server Start" << endl;

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9000);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET listenSocket;
	listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET)
		return false;
	if (::bind(listenSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
		return false;
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		return false;
	_hLogicProcessCp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
	_hWorkCp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);

	HANDLE hObj = CreateIoCompletionPort((HANDLE)listenSocket, _hWorkCp, reinterpret_cast<DWORD>(this), 0);
*/
	return false;

}

void CIOCPTCPServer::InitServer()
{
	WSADATA wsadata;

	//_wsetlocale(LC_ALL, L"korean");
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	cout << "1" << endl;
	_pPlayer = new CGameObject;
	for (int i = 0; i < 10; ++i)
	{
		_pPlayer->InitPlayer(i);
	}
	cout << "2" << endl;
	_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (_hIOCP == NULL)
	{
		cout << "IOCP InitError" << endl;
	}
}

void CIOCPTCPServer::AcceptThread()
{
	sockaddr_in accept_addr;
	sockaddr_in client_addr;

	SOCKET accept_socket = WSASocket(AF_INET, SOCK_STREAM,
		IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	ZeroMemory(&accept_addr, sizeof(accept_addr));
	accept_addr.sin_family = AF_INET;
	accept_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	accept_addr.sin_port = htons(9000);

	int errorCode = ::bind(accept_socket,
		reinterpret_cast<sockaddr*>(&accept_addr),
		sizeof(accept_addr));
	if (SOCKET_ERROR == errorCode)
	{
		cout << "bind Fail" << endl;
	}

	errorCode = listen(accept_socket, 10);
	if (SOCKET_ERROR == errorCode)
	{
		cout << "listen Fail" << endl;
	}

	while (true)
	{
		int addr_len = sizeof(accept_addr);
		SOCKET new_socket = WSAAccept(accept_socket,
			reinterpret_cast<sockaddr*>(&client_addr),
			&addr_len, NULL, NULL);
		if (INVALID_SOCKET == new_socket)
		{
			cout << "Accept Fail" << endl;
		}
		cout << "클라 접속" << endl;
		int id = _pPlayer->GetClientID();
		_pPlayer->InitPlayer(id);
		_pPlayer->GetPlayer(id).overlapped_ex.sock = new_socket;
		CreateIoCompletionPort(
			reinterpret_cast<HANDLE>(new_socket),
			_hIOCP, id, 0);

		unsigned long recv_flag = 0;
		errorCode = WSARecv(new_socket,
			&_pPlayer->GetPlayer(id).overlapped_ex.wsabuf, 1,
			NULL, &recv_flag,
			reinterpret_cast<LPOVERLAPPED>(&_pPlayer->GetPlayer(id).overlapped_ex), NULL);
		if (SOCKET_ERROR == errorCode)
		{
			int err_code = WSAGetLastError();
			if (WSA_IO_PENDING != err_code)
			{
				cout << "Accept(WSARecv)" << endl;
				exit(-1);
			}
		}
		sc_packet_put_player pp_packet;
		pp_packet.id = id;
		pp_packet.size = sizeof(pp_packet);
		pp_packet.type = SC_PUT_PLAYER;
		pp_packet.x = _pPlayer->GetPlayer(id).x;
		pp_packet.y = _pPlayer->GetPlayer(id).y;
		pp_packet.z = _pPlayer->GetPlayer(id).z;
		SendPacket(&pp_packet, id);
		_pPlayer->GetPlayer(id).in_use = true;
		for (int i = 0; i < 10; ++i) {
			if (false == _pPlayer->GetPlayer(i).in_use) continue;
			if (i == id) continue;
			SendPacket(&pp_packet, i);
		}
		for (int i = 0; i < 10; ++i) {
			if (false == _pPlayer->GetPlayer(i).in_use) continue;
			if (i == id) continue;
			pp_packet.id = i;
			pp_packet.x = _pPlayer->GetPlayer(i).x;
			pp_packet.y = _pPlayer->GetPlayer(i).y;
			pp_packet.z = _pPlayer->GetPlayer(i).z;
			SendPacket(&pp_packet, id);
		}


	}

}

void CIOCPTCPServer::WorkerThread()
{
	unsigned long IOSize;
	unsigned long long key;
	network_info* over_ex;
	PULONG_PTR;
	while (true)
	{
		GetQueuedCompletionStatus(_hIOCP, &IOSize, &key,
			reinterpret_cast<LPOVERLAPPED*>(&over_ex), INFINITE);

		if (0 == IOSize)
		{
			sc_packet_remove_player rem_player;
			rem_player.id = key;
			rem_player.size = sizeof(rem_player);
			rem_player.type = SC_REMOVE_PLAYER;

			for (int i = 0; i < 10; i++)
			{
				if (i == key) continue;;
				if(_pPlayer->GetPlayer(i).in_use == false)
					continue;
				SendPacket(&rem_player, i);
			}
			_pPlayer->GetPlayer(key).in_use = false;
			closesocket(over_ex->sock);
			continue;
		}
		if (over_ex->is_recv == true) {
			// 페킷조립 및 실행
			unsigned data_to_process = IOSize;
			char * buf = over_ex->IOCPbuf;
			while (0 < data_to_process) {
				if (0 == over_ex->curr_packet_size)
					over_ex->curr_packet_size = buf[0];
				unsigned need_to_build =
					over_ex->curr_packet_size
					- over_ex->prev_data_size;
				if (need_to_build <= data_to_process) {
					// 패킷 조립
					memcpy(over_ex->PacketBuf + over_ex->prev_data_size,
						buf, need_to_build);
					ProcessPacket(over_ex->PacketBuf, key);
					over_ex->curr_packet_size = 0;
					over_ex->prev_data_size = 0;
					data_to_process -= need_to_build;
					buf += need_to_build;
				}
				else {
					// 훗날을 기약
					memcpy(over_ex->PacketBuf + over_ex->prev_data_size,
						buf, data_to_process);
					over_ex->prev_data_size += data_to_process;
					data_to_process = 0;
					buf += data_to_process;
				}
			}
			// 다시 RECV
			unsigned long recv_flag = 0;
			WSARecv(over_ex->sock, &over_ex->wsabuf, 1, NULL,
				&recv_flag, &over_ex->overlapped, NULL);
		}
		else {
			delete over_ex;
		}

	}
}

bool CIOCPTCPServer::ServerRunCheck()
{
	return _bIsShutdown;
}

void CIOCPTCPServer::ShutdownServer()
{
	WSACleanup();
}

void CIOCPTCPServer::SendPacket(void* packet, unsigned id)
{
	network_info *over_ex = new network_info;
	int packet_size = reinterpret_cast<char *>(packet)[0];
	memcpy(over_ex->IOCPbuf, packet, packet_size);
	over_ex->is_recv = false;
	ZeroMemory(&over_ex->overlapped, sizeof(WSAOVERLAPPED));
	over_ex->wsabuf.buf = over_ex->IOCPbuf;
	over_ex->wsabuf.len = packet_size;
	unsigned long IOsize;
	WSASend(_pPlayer->GetPlayer(id).overlapped_ex.sock, &over_ex->wsabuf, 1,
		&IOsize, NULL, &over_ex->overlapped, NULL);
}

void CIOCPTCPServer::ProcessPacket(char* packet, unsigned id)
{

	switch (packet[1])
	{
	case CS_UP:
	case CS_DOWN:
	case CS_LEFT:
	case CS_RIGHT:
		Process_Move_Packet(packet, id);
		break;
	default:
		cout << "Unkwon Packet Type" << endl;
		exit(-1);
		break;
	}

	//for (int i = 0; i < 10; ++i)
	//{
	//	if (_pPlayer->GetPlayer(i).in_use == false) continue;;
	//	SendPacket(&pos_packet, i);
	//}


}

void CIOCPTCPServer::Process_Move_Packet(char* packet, unsigned id)
{
	float x = _pPlayer->GetPlayer(id).x;
	float y = _pPlayer->GetPlayer(id).y;
	float z = _pPlayer->GetPlayer(id).z;
	
	switch (packet[1])
	{
	case CS_UP:
		if (z <= 0)
			break;
		z -=0.5f;
		break;
	case CS_DOWN:
		z+=0.5f;
		break;
	case CS_LEFT:
		x+= 0.5f;
		break;
	case CS_RIGHT:
		if (x <= 0)
			break;
		x-=0.5f;
		break;
	default:
		cout << "Unkwon Packet Type" << endl;
		exit(-1);
		break;
	}

	_pPlayer->GetPlayer(id).x = x;
	_pPlayer->GetPlayer(id).y = y;
	_pPlayer->GetPlayer(id).z = z;

	sc_packet_pos pos_packet;
	pos_packet.id = id;
	pos_packet.size = sizeof(pos_packet);
	pos_packet.type = SC_POS;
	pos_packet.x = x;
	pos_packet.y = y;
	pos_packet.z = z;

	for (int i = 0; i < 10; ++i)
	{
		if (_pPlayer->GetPlayer(i).in_use == false) continue;;
		SendPacket(&pos_packet, i);
	}
}

