#include "TimeEvent.h"



void CTimeEvent::AddTimer(int obj_id, int do_event, int wakeup_time)
{
	event_type new_event;
	new_event.do_event = do_event;
	new_event.obj_id = obj_id;
	new_event.wakeup_time = milliseconds((int)wakeup_time) + curr_time();
	tq_lock.lock();
	Timer_Queue.push(new_event);
	tq_lock.unlock();
}

CTimeEvent::CTimeEvent()
{
}


CTimeEvent::~CTimeEvent()
{
}

CTimeEvent & CTimeEvent::GetInstance()
{
	static CTimeEvent instance;
	return instance;
}


