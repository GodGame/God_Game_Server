#include "IOCPTCPServer.h"
#include "GameObject.h"
#include "MapManager.h"
#include "Timer.h"
#include "TimeEvent.h"
#include "GameManager.h"

CIOCPTCPServer::CIOCPTCPServer()
{
	_hLogicProcessCp = INVALID_HANDLE_VALUE;
	_hWorkCp = INVALID_HANDLE_VALUE;
	randomEngine = default_random_engine(random_device{}());
	randomX = uniform_int_distribution<int>(1100, 1300);
	randomZ = uniform_int_distribution<int>(200, 300);


	m_fFrameTime = 0.0f;

}


CIOCPTCPServer::~CIOCPTCPServer()
{
	delete _pGameObject;
}
bool CIOCPTCPServer::StartServer()
{

	return false;

}

void CIOCPTCPServer::InitServer()
{
	XMFLOAT3 xv3Scale(8.0f, 1.5f, 8.0f);
	m_nRoundTime = 0;
	const int ImageWidth = 256;
	const int ImageLength = 256;
	MAPMgr.Build((L"../Assets/Image/Terrain/HeightMap.raw"), ImageWidth + 1, ImageLength + 1, ImageWidth + 1, ImageLength + 1, xv3Scale);
	cout << "HeightMap Load Complete" << endl;
	WSADATA wsadata;
	//	_wsetlocale(LC_ALL, L"korean");
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	cout << "1" << endl;
	_pGameObject = new CGameObject();
	_pGameManager = &CGameManager::GetInstance();
	//	_pTimer = &CGameTimer::GetInstance();
	for (int i = 0; i < 1000; i++)
	{
		int z = randomZ(randomEngine);
		_pGameObject->SetObjectHeight(i, MAPMgr.GetHeight(randomX(randomEngine), z, (!(z % 2))));

	}
	cout << "Height Create Success" << endl;
	_pGameObject->InitPlayer();
	cout << "2" << endl;
	_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (_hIOCP == NULL)
	{
		cout << "IOCP InitError" << endl;
	}
}

void CIOCPTCPServer::AcceptThread()
{
	//	sockaddr_in accept_addr;
	struct sockaddr_in listen_addr;

	SOCKET accept_socket = WSASocket(AF_INET, SOCK_STREAM,
		IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	ZeroMemory(&listen_addr, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	listen_addr.sin_port = htons(9000);
	ZeroMemory(&listen_addr.sin_zero, 8);

	int errorCode = ::bind(accept_socket,
		reinterpret_cast<sockaddr*>(&listen_addr),
		sizeof(listen_addr));
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
		struct sockaddr_in client_addr;
		int addr_len = sizeof(client_addr);
		SOCKET new_socket = WSAAccept(accept_socket,
			reinterpret_cast<sockaddr*>(&client_addr),
			&addr_len, NULL, NULL);
		if (INVALID_SOCKET == new_socket)
		{
			cout << "Accept Fail" << endl;
		}
		int new_id = -1;
		for (auto i = 0; i < MAX_USER; ++i)
		{
			if (new_id == 2) i = 0;
			if (_pGameObject->m_Player[i].is_connected == false)
			{
				new_id = i;
				break;
			}
		}
		if (-1 == new_id)
		{
			cout << "Maximum User Number Fail" << endl;
			closesocket(new_socket);
			continue;
		}


		cout << "Client Accepted" << endl;

		CreateIoCompletionPort(
			reinterpret_cast<HANDLE>(new_socket),
			_hIOCP, new_id, 0);

		_pGameObject->m_Player[new_id].m_sock = new_socket;
		_pGameObject->m_Player[new_id].is_connected = true;
		_pGameObject->m_Player[new_id].m_ID = new_id;
		//_pGameObject->m_Player[new_id].SetPosition(XMFLOAT3(1300, MAPMgr.GetHeight(1300, 255, true), 255));
		//_pGameObject->m_Player[new_id].SetPosition(XMFLOAT3(MAPMgr.GetWidth()*0.5f, 100, 500+(rand()%40 -20)));
		if(new_id == 0)
			_pGameObject->m_Player[new_id].SetPosition(XMFLOAT3(1024, 300, 320));
		else
		{
			_pGameObject->m_Player[new_id].SetPosition(XMFLOAT3(1000, 300, 320));
		}
		_pGameObject->m_Player[new_id].m_overlapped_ex.operation = OP_RECV;
		_pGameObject->m_Player[new_id].m_overlapped_ex.packet_size = 0;
		_pGameObject->m_Player[new_id].previous_data_size = 0;


		//sc_packet_Init_player init_packet;

		//init_packet.id = new_id;
		//init_packet.size = sizeof(init_packet);
		//init_packet.type = SC_OBJECT_INIT;
		//init_packet.x = _pGameObject->m_Player[new_id].GetPosition().x;
		//init_packet.y = _pGameObject->m_Player[new_id].GetPosition().y;
		//init_packet.z = _pGameObject->m_Player[new_id].GetPosition().z;

		//for (auto i = 0; i < 10; i++)
		//{
		//	if (true == _pGameObject->m_Player[i].is_connected)
		//		SendPacket(reinterpret_cast<unsigned char*>(&init_packet), i);
		//}


		sc_packet_put_player put_player_packet;
		put_player_packet.id = new_id;
		put_player_packet.size = sizeof(put_player_packet);
		put_player_packet.type = SC_PUT_PLAYER;
		put_player_packet.x = _pGameObject->m_Player[new_id].GetPosition().x;
		put_player_packet.y = _pGameObject->m_Player[new_id].GetPosition().y;
		put_player_packet.z = _pGameObject->m_Player[new_id].GetPosition().z;
		put_player_packet.HP = _pGameObject->m_Player[new_id].m_HP;
		memcpy(&(put_player_packet.numberOfElement), &(_pGameObject->m_Player[new_id].m_Elements), sizeof(_pGameObject->m_Player[new_id].m_Elements));
	
		for (auto i = 0; i < 10; i++)
		{
			if (true == _pGameObject->m_Player[i].is_connected)
				SendPacket(reinterpret_cast<unsigned char*>(&put_player_packet), i);
		}
		for (auto i = 0; i < 10; ++i)
		{
			if (false == _pGameObject->m_Player[i].is_connected) continue;
			if (i == new_id) continue;

			put_player_packet.id = i;
			put_player_packet.x = _pGameObject->m_Player[i].GetPosition().x;
			put_player_packet.y = _pGameObject->m_Player[i].GetPosition().y;
			put_player_packet.z = _pGameObject->m_Player[i].GetPosition().z;
			put_player_packet.HP = _pGameObject->m_Player[i].m_HP;
			SendPacket(reinterpret_cast<unsigned char*>(&put_player_packet), new_id);
		}
		//////////////////////////////////////////////////////////////////////////
		sc_packet_GameState gamestate_packet;
		gamestate_packet.size = sizeof(sc_packet_GameState);
		gamestate_packet.type = SC_GAME_STATE;
		gamestate_packet.id = new_id;
		gamestate_packet.gamestate = STATE_TOTAL_NUM;

		for (auto i = 0; i < 10; ++i)
		{
			if (false == _pGameObject->m_Player[i].is_connected) continue;
			SendPacket(reinterpret_cast<unsigned char*>(&gamestate_packet), i);
		}
		//////////////////////////////////////////////////////////////////////////



		_pGameManager->m_nConnectCount += 1;
		//if(new_id == 0)
		if (_pGameManager->GetConnectPlayerNum() == 2)
		{
			_pGameManager->m_eGameState = STATE_READY;//STATE_RO ENTER;
			m_TimeEvent.AddTimer(new_id, EVENT_ROUND_TIMER, 1000);
		}

		DWORD flags = 0;
		int result = WSARecv(new_socket,
			&_pGameObject->m_Player[new_id].m_overlapped_ex.wsabuf,
			1, NULL, &flags,
			&_pGameObject->m_Player[new_id].m_overlapped_ex.original_overlapped,
			NULL);

		if (0 != result)
		{
			int error_no = WSAGetLastError();
			if (WSA_IO_PENDING != error_no)
			{
				error_display(__FUNCTION__ "Accept Thread: WSARecv", error_no);
				//	cout << __FUNCTION__ "Accept Thread: WSARecv" << endl;;

			}
		}

	}

}

void CIOCPTCPServer::WorkerThread()
{
	DWORD io_size, key;
	OVERLAPPED_EX* overlap;
	bool result;


	/*unsigned long IOSize;
	unsigned long long key;
	network_info* over_ex;
	PULONG_PTR;*/
	while (true)
	{
		result = GetQueuedCompletionStatus(_hIOCP, &io_size, reinterpret_cast<PULONG_PTR>(&key),
			reinterpret_cast<LPOVERLAPPED*>(&overlap), INFINITE);

		if (false == result)
		{
			// 에러 처리
		}
		if (0 == io_size)
		{

			sc_packet_remove_player rem_player;
			rem_player.id = key;
			rem_player.size = sizeof(sc_packet_remove_player);
			rem_player.type = SC_REMOVE_PLAYER;
			_pGameManager->m_nConnectCount -= 1;
			for (int i = 0; i < 10; i++)
			{
				if (false == _pGameObject->m_Player[i].is_connected) continue;
				if (key == i) continue;

				SendPacket(reinterpret_cast<unsigned char*>(&rem_player), i);

			}
			_pGameObject->m_Player[key].is_connected = false;
			closesocket(_pGameObject->m_Player[key].m_sock);
			cout << "Client [" << key << "] Disconnected" << endl;
			continue;
		}

		if (OP_RECV == overlap->operation)
		{
			unsigned char* buf_ptr = overlap->socket_buf;
			int remain = io_size;

			while (0 < remain)
			{
				if (0 == _pGameObject->m_Player[key].m_overlapped_ex.packet_size)
				{
					_pGameObject->m_Player[key].m_overlapped_ex.packet_size = buf_ptr[0];
				}
				int required = _pGameObject->m_Player[key].m_overlapped_ex.packet_size
					- _pGameObject->m_Player[key].previous_data_size;

				if (required <= remain)
				{
					memcpy(_pGameObject->m_Player[key].packetBuf + _pGameObject->m_Player[key].previous_data_size,
						buf_ptr,
						required);

					ProcessPacket(_pGameObject->m_Player[key].packetBuf, key);
					//	ProcessPacket(key, clients[key].packet);
					remain -= required;
					buf_ptr += required;
					_pGameObject->m_Player[key].m_overlapped_ex.packet_size = 0;
					_pGameObject->m_Player[key].previous_data_size = 0;

				}
				else
				{
					memcpy(_pGameObject->m_Player[key].packetBuf + _pGameObject->m_Player[key].previous_data_size,
						buf_ptr, remain);
					_pGameObject->m_Player[key].previous_data_size += remain; // 미완성 데이터 증가.
					remain = 0;
					buf_ptr += remain;
				}

			}
			DWORD flags = 0;
			WSARecv(_pGameObject->m_Player[key].m_sock, &_pGameObject->m_Player[key].m_overlapped_ex.wsabuf,
				1, NULL, &flags,
				reinterpret_cast<LPWSAOVERLAPPED>(&_pGameObject->m_Player[key].m_overlapped_ex),
				NULL);
		}
		else if (OP_SEND == overlap->operation)
		{
			delete overlap;
		}
		else if (overlap->operation == OP_ROUND_TIME) {
			//global_lock.lock();
			RoundTimer(key);
			m_TimeEvent.AddTimer(key, EVENT_ROUND_TIMER, 1000);
			//global_lock.unlock();
		}
		else if (overlap->operation == OP_CHANGE_GAMESTATE)
		{

		}
		else if (overlap->operation == OP_TIME)
		{
			
		}
		else
		{
			cout << "Unknown Event on worker_thread" << endl;
			while (true);
		}

	}
}

void CIOCPTCPServer::TimeThread()
{

}

bool CIOCPTCPServer::ServerRunCheck()
{
	return _bIsShutdown;
}

void CIOCPTCPServer::ShutdownServer()
{
	WSACleanup();
}

void CIOCPTCPServer::SendPacket(unsigned char*  packet, int id)
{
	OVERLAPPED_EX *send_over = new OVERLAPPED_EX;
	memset(send_over, 0, sizeof(OVERLAPPED_EX));
	send_over->operation = OP_SEND;
	send_over->wsabuf.buf = reinterpret_cast<CHAR*>(send_over->socket_buf);
	send_over->wsabuf.len = packet[0];
	memcpy(send_over->socket_buf, packet, packet[0]);

	int res = WSASend(_pGameObject->m_Player[id].m_sock, &send_over->wsabuf, 1,
		NULL, 0,
		&send_over->original_overlapped, NULL);
	if (0 != res) // WSA_IO_PENDING은 IOCP에서 미처 다 보내지 못했을때 나오는 에러
	{
		int error_no = WSAGetLastError();
		if (WSA_IO_PENDING != error_no)
		{
			error_display(__FUNCTION__ "SendPacket:WSASend", error_no);
			//	cout << __FUNCTION__ "SendPacket:WSASend" << endl;;
		}
		//	error_display("SendPacket:WSASend", error_no);
		//while (true);
	}
}

void CIOCPTCPServer::ProcessPacket(unsigned char* packet, int id)
{
	XMFLOAT3 tempPosition = XMFLOAT3(_pGameObject->m_Player[id].GetPosition());
	unsigned char* packet_type = packet;
	//	cout << packet_type[1] << endl;
	switch (packet_type[1])
	{
	//case CS_UP:
	//case CS_DOWN:
	//case CS_LEFT:
	//case CS_RIGHT:
	case SERVER_CHEAT:
	{
		cs_packet_serverCheat * cheat_packet = reinterpret_cast<cs_packet_serverCheat*>(packet);
		if (cheat_packet->mode == 0)
		{
			_pGameManager->m_nRoundTime += 5;
		}
		else if (cheat_packet->mode == 1)
		{
			_pGameManager->m_nRoundTime -= 5;
		}
		break;
	}
	case CS_MOVE:
//	case CS_ANI_IDLE:
		Process_Move_Packet(packet, id);
		break;
	case CS_ANI_IDLE:
	{
		sc_packet_Behavior behavior_packet;
		behavior_packet.id = id;
		behavior_packet.size = sizeof(sc_packet_Behavior);
		behavior_packet.type = SC_ANI_IDLE;

		for (int i = 0; i < 10; ++i)
		{
			if (false == _pGameObject->m_Player[i].is_connected) continue;
			SendPacket(reinterpret_cast<unsigned char*>(&behavior_packet), i);
		}
		//SC_ANI_IDLE
		break;
	}
	case CS_READY:

		break;
	case CS_INPUT:
		cout << "INPUT" << endl;
		Process_StateSet_Packet(packet, id);
		break;
	case CS_CHAR_DAMAGE:
		Process_Damage_Packet(packet, id);
		break;
	case CS_DOMINATE:
	{
		//cs_packet_dominate* dominate_packet = reinterpret_cast<cs_packet_dominate*>(packet);
		sc_packet_dominate dominate_packet;
		dominate_packet.id = id;
		dominate_packet.size = sizeof(sc_packet_dominate);
		dominate_packet.type = SC_DOMINATE;

		for (int i = 0; i < 10; ++i)
		{
			if (false == _pGameObject->m_Player[i].is_connected) continue;
			SendPacket(reinterpret_cast<unsigned char*>(&dominate_packet), i);
		}
		break;
	}
	case CS_MAGIC_CASTING:
	{
		sc_packet_Behavior behavior_packet;
		behavior_packet.id = id;
		behavior_packet.size = sizeof(sc_packet_Behavior);
		behavior_packet.type = SC_MAGIC_CASTING;

		for (int i = 0; i < 10; ++i)
		{
			if (false == _pGameObject->m_Player[i].is_connected) continue;
			SendPacket(reinterpret_cast<unsigned char*>(&behavior_packet), i);
		}
		break;
	}
	case CS_ROTATION:
	{
		cs_packet_rotate * test_packet = reinterpret_cast<cs_packet_rotate *>(packet);
		
		sc_packet_rotate rotate_packet;
		rotate_packet.id = id;
		rotate_packet.size = sizeof(sc_packet_rotate);
		rotate_packet.type = SC_ROTATION;
		rotate_packet.cxDelta = test_packet->cxDelta;
		rotate_packet.cyDelta = test_packet->cyDelta;
		rotate_packet.LookVector = test_packet->LookVector;
		//cout << "Player [" << id << "] LookVector x:" << test_packet->LookVector.x << " y:" << test_packet->LookVector.y << " z:" << test_packet->LookVector.z << endl;

		for (int i = 0; i < 10; ++i)
		{
				if (i == id) continue;
			if (false == _pGameObject->m_Player[i].is_connected) continue;
			SendPacket(reinterpret_cast<unsigned char*>(&rotate_packet), i);
		}
		break;
	}
	default:
		printf(__FUNCTION__"Unknown PACKET type [%d]\n", packet_type[1]);
		exit(-1);
		break;
	}

	//for (int i = 0; i < 10; ++i)
	//{
	//	if (_pGameObject->GetPlayer_ref(i).in_use == false) continue;;
	//	SendPacket(&pos_packet, i);
	//}


}


void CIOCPTCPServer::RoundTimer(int id)
{
	//if (m_nRoundTime < ROUND_END_TIME)
//	{
	//if(_pGameManager->GetGameState() == STATE_READY)

	switch (_pGameManager->GetGameState())
	{
	case STATE_READY:
	{
		_pGameManager->m_nReadyTime = 5;
		_pGameManager->m_eGameState = STATE_ROUND_ENTER;
		SendGameState(_pGameManager->GetGameState(), id);

		break;
	}
	case STATE_ROUND_ENTER:
	{
		if (_pGameManager->GetReadyTime() < 0)
		{
			_pGameManager->Initialize();
			//_pGameManager->m_nReadyTime = 4;
			_pGameManager->m_eGameState = STATE_ROUND_START;
			SendGameState(_pGameManager->GetGameState(), id);
			break;
		}
		//_pGameManager->m_nGameRound = 1;
		SendTime(_pGameManager->GetReadyTime(), _pGameManager->GetGameRound(), id);
		_pGameManager->m_nReadyTime -= 1;
		break;
	}
	case STATE_ROUND_START:
	{
		if (_pGameManager->GetRoundTime() < 0)
		{
			_pGameManager->m_eGameState = STATE_ROUND_END;
			SendGameState(_pGameManager->GetGameState(), id);
			break;
		}
		SendTime(_pGameManager->GetRoundTime(), _pGameManager->GetGameRound(), id);
		_pGameManager->m_nRoundTime -= 1;
		break;
	}
	case STATE_ROUND_END:
	{
		if (_pGameManager->GetEndTime() < 0)
		{
			_pGameManager->Initialize();
			_pGameManager->m_eGameState = STATE_ROUND_CLEAR;
			SendGameState(_pGameManager->GetGameState(), id);
			break;
		}
		SendTime(_pGameManager->GetEndTime(), _pGameManager->GetGameRound(), id);
		_pGameManager->m_nEndTime -= 1;
		break;
	}
	case STATE_ROUND_CLEAR:
	{
		
		if(_pGameManager->m_nGameRound == 2)
		{
			_pGameManager->m_eGameState = STATE_GAME_END;
			SendGameState(_pGameManager->GetGameState(), id);
		}
		else
		{
			_pGameManager->m_nGameRound = 2;
			_pGameManager->m_nReadyTime = 5;
			_pGameManager->m_eGameState = STATE_ROUND_ENTER;//STATE_READY;
			SendGameState(_pGameManager->GetGameState(), id);
		}
		//_pGameManager->m_eGameState = STATE_ROUND_CLEAR;
		break;
	}
	case STATE_GAME_END:
	{
	//	_pGameManager->m_nGameRound = 1;
		//_pGameManager->m_eGameState = STATE_GAME_END;
	//	SendGameState(_pGameManager->GetGameState(), id);
		break;
	}
	case STATE_ROUND_CHANGE:
	{
		_pGameManager->m_nGameRound = 2;
		_pGameManager->m_eGameState = STATE_READY;

		SendGameState(_pGameManager->GetGameState(), id);
		break;
	}
	case STATE_TOTAL_NUM:
		_pGameManager->m_eGameState = STATE_READY;
		SendGameState(_pGameManager->GetGameState(), id);
		break;
	default:
		break;
	}




	//	}

}
void CIOCPTCPServer::SendTime(int time, int round, int id)
{
	sc_packet_RoundTime roundtime_packet;
	roundtime_packet.size = sizeof(sc_packet_RoundTime);
	roundtime_packet.type = SC_ROUND_TIME;
	roundtime_packet.id = id;
	roundtime_packet.round = round;
	roundtime_packet.time = time;

	for (auto i = 0; i < 10; ++i)
	{
		if (false == _pGameObject->m_Player[i].is_connected) continue;
		SendPacket(reinterpret_cast<unsigned char*>(&roundtime_packet), i);
	}
}
void CIOCPTCPServer::SendGameState(SERVER_GAME_STATE eGamestate, int id)
{

	sc_packet_GameState gamestate_packet;
	gamestate_packet.size = sizeof(sc_packet_GameState);
	gamestate_packet.type = SC_GAME_STATE;
	gamestate_packet.id = id;
	gamestate_packet.round = _pGameManager->m_nGameRound;
	gamestate_packet.gamestate = eGamestate;

	for (auto i = 0; i < 10; ++i)
	{
		if (false == _pGameObject->m_Player[i].is_connected) continue;
		SendPacket(reinterpret_cast<unsigned char*>(&gamestate_packet), i);
	}
}



void CIOCPTCPServer::Process_Move_Packet(unsigned char* packet, int id)
{
	/*float x = _pGameObject->GetPlayer_ref(id).x;
	float y = _pGameObject->GetPlayer_ref(id).y;
	float z = _pGameObject->GetPlayer_ref(id).z;*/
	cs_packet_move_test * test_packet = reinterpret_cast<cs_packet_move_test *>(packet);
	XMFLOAT3 position = _pGameObject->m_Player[id].GetPosition();
	XMFLOAT3 direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 lookVector = _pGameObject->m_Player[id].GetLookVector();
	XMVECTOR xv3Shift = XMVectorReplicate(0);
	XMFLOAT3 Velocity;// = 15.0f;

	XMFLOAT3 tempPosition = _pGameObject->m_Player[id].GetPosition();
	unsigned char packet_type = packet[1];
	DWORD dwDirection;
	
	//test_packet->animation;
	sc_packet_pos pos_packet;
	dwDirection = test_packet->direction;
	cout << dwDirection << endl;
	position = test_packet->Position;
	_pGameObject->m_Player[id].SetPosition(position);
	_pGameObject->m_Player[id].SetLookVector(test_packet->LookVector);
	//_pGameObject->m_Player[id].Move(dwDirection, 50.0f*0.002, false);
	//switch (packet_type)
	//{
	//case CS_MOVE:
	//{
	// dwDirection = test_packet->direction;
	//	cout << dwDirection << endl;
	//	position = test_packet->Position;
	//	_pGameObject->m_Player[id].Move(dwDirection, 50.0f*0.002, false);
	//	break;

	//}
	///*case CS_UP:
	//	_pGameObject->m_Player[id].Move(DIR_FORWARD, 50.0f*0.002, false);
	//	pos_packet.animationType = eANI_RUN_FORWARD;
	//	break;
	//case CS_DOWN:
	//	_pGameObject->m_Player[id].Move(DIR_BACKWARD, 50.0f*0.002, false);
	//	pos_packet.animationType = eANI_WALK_BACK;
	//	break;
	//case CS_LEFT:
	//	_pGameObject->m_Player[id].Move(DIR_LEFT, 50.0f*0.002, false);
	//	pos_packet.animationType = eANI_WALK_LEFT;
	//	break;
	//case CS_RIGHT:
	//	_pGameObject->m_Player[id].Move(DIR_RIGHT, 50.0f*0.002, false);
	//	pos_packet.animationType = eANI_WALK_RIGHT;
	//	break;*/
	////case DIR_UP:
	////	xv3Shift += XMLoadFloat3(&_pGameObject->m_Player[id].GetLookVector()) * (50.0f * 0.002) * 1.2f;
	////	//	if (tempPosition.z <= 0)
	////	//		break;
	////	//	tempPosition.z -=0.5f;
	////	pos_packet.animationType = eANI_RUN_FORWARD;
	////	break;
	////case DIR_DOWN:
	////	//cout << m_fFrameTime << endl;
	////	xv3Shift -= XMLoadFloat3(&_pGameObject->m_Player[id].GetLookVector()) * (50.0f *0.002) * 0.8f;
	////	//	tempPosition.z +=0.5f;
	////	pos_packet.animationType = eANI_WALK_BACK;
	////	break;
	////case DIR_RIGHT:
	////	xv3Shift += XMLoadFloat3(&_pGameObject->m_Player[id].GetRightVector()) * (50.0f * 0.002) * 0.8f;
	////	/*	if (tempPosition.x <= 0)
	////			break;
	////		tempPosition.x -= 0.5f;*/
	////	pos_packet.animationType = eANI_WALK_RIGHT;
	////	break;
	////case DIR_LEFT:
	////	xv3Shift -= XMLoadFloat3(&_pGameObject->m_Player[id].GetRightVector()) * (50.0f * 0.002) * 0.8f;
	////	/*	tempPosition.x += 0.5f;*/
	////	pos_packet.animationType = eANI_WALK_LEFT;
	////	break;
	//case CS_ANI_IDLE:
	//	pos_packet.animationType = eANI_IDLE;
	//	break;
	//default:
	//	cout << "Unkwon Packet Type" << endl;
	//	exit(-1);
	//	break;
	//}

	///tempPosition = XMFLOAT3(tempPosition.x, MAPMgr.GetHeight(tempPosition.x, tempPosition.z, !(((int)tempPosition.z) % 2)), tempPosition.z);
	//_pGameObject->m_Player[id].SetPosition(tempPosition);
	//cout << "id: " << id << endl;
	/*_pGameObject->GetPlayer_ref(id).x = x;
	_pGameObject->GetPlayer_ref(id).y = y;
	_pGameObject->GetPlayer_ref(id).z = z;*/

	pos_packet.id = id;
	pos_packet.size = sizeof(sc_packet_pos);
	pos_packet.type = SC_POS;
	pos_packet.Position = _pGameObject->m_Player[id].GetPosition();
	pos_packet.LookVector = _pGameObject->m_Player[id].GetLookVector();
	pos_packet.dwDirection = dwDirection;
	if (dwDirection == 0x00)
		pos_packet.animationType = eANI_IDLE;
	//pos_packet.x = _pGameObject->m_Player[id].GetPosition().x;
	//pos_packet.y = _pGameObject->m_Player[id].GetPosition().y;
	//pos_packet.z = _pGameObject->m_Player[id].GetPosition().z;
	//pos_packet.LookVector = _pGameObject->m_Player[id].GetLookVector();
	//pos_packet.RightVetor = _pGameObject->m_Player[id].GetRightVector();
	//pos_packet.Shift = xmf3Shift;
	//XMStoreFloat3(&pos_packet.Shift, xv3Shift);
	//pos_packet.Shift = xmf3Shift;
	//pos_packet.LookVector
	//cout << id << endl;
	for (int i = 0; i < 10; ++i)
	{
		if (i == id) continue;
		if (false == _pGameObject->m_Player[i].is_connected) continue;
		SendPacket(reinterpret_cast<unsigned char*>(&pos_packet), i);
	}
}

void CIOCPTCPServer::Process_StateSet_Packet(unsigned char* packet, int id)
{
	//XMVECTOR xv3Shift = XMVectorReplicate(0);
	//char* tempbuf = reinterpret_cast<CHAR*>(packet);
	cs_packet_state * my_packet = reinterpret_cast<cs_packet_state *>(packet);

	_pGameObject->m_Player[id].m_LookVector = my_packet->LookVector;
	_pGameObject->m_Player[id].m_RightVetor = my_packet->RightVector;

	cout << "Player [" << id << "] LookVector x:" << my_packet->LookVector.x << " y:" << my_packet->LookVector.y << " z:" << my_packet->LookVector.z << endl;
	cout << "Player [" << id << "] RightVector x:" << my_packet->RightVector.x << " y:" << my_packet->RightVector.y << " z:" << my_packet->RightVector.z << endl;


}

void CIOCPTCPServer::Process_Damage_Packet(unsigned char* packet, int id)
{
	cs_packet_damage * my_packet = reinterpret_cast<cs_packet_damage *>(packet);
	_pGameObject->m_Player[id].m_HP = my_packet->damage;
	cout << "Player " << id <<" Damage!!" <<" hp: "<<_pGameObject->m_Player[id].m_HP << endl;

	sc_packet_playerInfo info_packet;
	info_packet.HP = _pGameObject->m_Player[id].m_HP;
	info_packet.id = id;
	info_packet.size = sizeof(sc_packet_playerInfo);
	info_packet.type = SC_PLAYER_INFO;

	for (int i = 0; i < 10; ++i)
	{
		if(id == i) continue;
		if (false == _pGameObject->m_Player[i].is_connected) continue;
		SendPacket(reinterpret_cast<unsigned char*>(&info_packet), i);
	}
	//my_packet->damage
}

void CIOCPTCPServer::error_display(char * msg, int err_no)
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

CIOCPTCPServer & CIOCPTCPServer::GetInstance()
{
	static CIOCPTCPServer instance;
	return instance;
}

void CIOCPTCPServer::Timer_Thread()
{
	while (true) {
		Sleep(1);
		m_TimeEvent.tq_lock.lock();
		if (m_TimeEvent.Timer_Queue.size() == 0) {
			m_TimeEvent.tq_lock.unlock();
			continue;
		}
		event_type top_event = m_TimeEvent.Timer_Queue.top();
		while (top_event.wakeup_time <= m_TimeEvent.curr_time()) {
			m_TimeEvent.Timer_Queue.pop();
			m_TimeEvent.tq_lock.unlock();
			Process_Event(top_event);
			m_TimeEvent.tq_lock.lock();
			if (m_TimeEvent.Timer_Queue.size() == 0) {
				goto loop_end;
			}
			top_event = m_TimeEvent.Timer_Queue.top();
		}
	loop_end:
		m_TimeEvent.tq_lock.unlock();
	}
}
void CIOCPTCPServer::Process_Event(event_type curr_event)
{
	OVERLAPPED_EX event_over;
	switch (curr_event.do_event) {
	case EVENT_ROUND_TIMER:
	{
		event_over.operation = OP_ROUND_TIME;
		PostQueuedCompletionStatus(_hIOCP, 1, curr_event.obj_id,
			reinterpret_cast<LPOVERLAPPED>(&event_over));
		//heart_beat(curr_event.obj_id);
		//AddTimer(curr_event.obj_id, EVENT_MOVE, 1000);
		break;
	}
	case EVENT_CHANGE_GAMESTATE:
		event_over.operation = OP_CHANGE_GAMESTATE;
		PostQueuedCompletionStatus(_hIOCP, 1, curr_event.obj_id,
			reinterpret_cast<LPOVERLAPPED>(&event_over));

		break;
	default:
		cout << "Unknown Event Type Detected!\n";
		exit(-1);
	}
}
