#pragma once
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <vector>
#include <thread>
#include <mutex>
#pragma comment(lib,"ws2_32.lib")

using namespace std;
#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  255
#define CS_UP    1
#define CS_DOWN  2
#define CS_LEFT  3
#define CS_RIGHT    4
#define CS_CHAT		5
#define CS_ANI_IDLE 6
#define CS_ANI_MAGIC_CAST1 7
#define CS_ANI_MAGIC_CAST2 8

#define SC_POS           1
#define SC_PUT_PLAYER    2
#define SC_REMOVE_PLAYER 3
#define SC_CHAT		4
#define SC_ANI_IDLE 5
#define SC_ANI_MAGIC_CAST1 6
#define SC_ANI_MAGIC_CAST2 7



struct network_info {
	WSAOVERLAPPED overlapped;
	SOCKET sock;
	bool is_recv;
	WSABUF wsabuf;
	char IOCPbuf[MAX_BUFF_SIZE];
	char PacketBuf[MAX_PACKET_SIZE];
	int prev_data_size;
	int curr_packet_size;
};

struct player {
	bool in_use;
	float x;
	float y;
	float z;
	int animation_type;
	network_info overlapped_ex;
};

#pragma pack (push, 1)

struct cs_packet_up {
	BYTE size;
	BYTE type;
};

struct cs_packet_down {
	BYTE size;
	BYTE type;
};

struct cs_packet_left {
	BYTE size;
	BYTE type;
};

struct cs_packet_right {
	BYTE size;
	BYTE type;
};

struct cs_packet_chat {
	BYTE size;
	BYTE type;
	WCHAR message[100];
};

struct cs_packet_animation {
	BYTE size;
	BYTE type;
	BYTE animation_Type;
};

//struct sc_packet_pos {
//	BYTE size;
//	BYTE type;
//	WORD id;
//	BYTE x;
//	BYTE y;
//	BYTE z;
//};
struct sc_packet_pos {
	BYTE size;
	BYTE type;
	WORD id;
	float x;
	float y;
	float z;
};
struct sc_packet_put_player {
	BYTE size;
	BYTE type;
	WORD id;
	BYTE x;
	BYTE y;
	BYTE z;
};
struct sc_packet_remove_player {
	BYTE size;
	BYTE type;
	WORD id;
};

struct sc_packet_chat {
	BYTE size;
	BYTE type;
	WORD id;
	WCHAR message[100];
};
struct sc_packet_animation_player {
	BYTE size;
	BYTE type;
	WORD id;
	BYTE animation_Type;
};
#pragma pack (pop)