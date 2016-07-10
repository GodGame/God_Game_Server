#pragma once
#include "common.h"
#include "Protocol.h"

struct network_info {
	WSAOVERLAPPED overlapped;
	SOCKET sock;
	bool is_recv;
	WSABUF wsabuf;
	char IOCPbuf[MAX_BUFF_SIZE];
	char PacketBuf[MAX_BUFF_SIZE];
	int prev_data_size;
	int curr_packet_size;
};

struct OVERLAPPED_EX {
	WSAOVERLAPPED original_overlapped;
	int operation;
	WSABUF wsabuf;
	unsigned char socket_buf[MAX_BUFF_SIZE];

	int packet_size;
};

struct event_type {
	int obj_id;
	time_point<high_resolution_clock> wakeup_time;
	int do_event;
	int target_id;
};
class mycomparison
{
	bool reverse;
public:
	mycomparison() {}
	bool operator() (const event_type lhs, const event_type rhs) const
	{
		return (lhs.wakeup_time > rhs.wakeup_time);
	}
};

struct LogEventType
{
	time_point<high_resolution_clock> time;
};