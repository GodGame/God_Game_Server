#pragma once
#include "Common.h"
#include "Struct.h"

class CTimeEvent
{
public:
	priority_queue<event_type, vector<event_type>, mycomparison> Timer_Queue;
	mutex tq_lock;
	time_point<high_resolution_clock> start_time;

public:

	time_point<high_resolution_clock> curr_time() { return high_resolution_clock::now(); }
	void AddTimer(int obj_id, int do_event, int wakeup_time);
	//void AddTimer(int obj_id, int index_id, int do_event, int wakeup_time);
	void Timer_Thread();
public:
	static CTimeEvent & GetInstance();
public:
	CTimeEvent();
	~CTimeEvent();
};

