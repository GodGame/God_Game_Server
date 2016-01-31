#pragma once
#include "stdafx.h"
template <class T>
class CCircularQueue
{
	T		m_Queue[MAX_QUEUE_LENGTH];
	DWORD	m_QueueHead;
	DWORD	m_QueueTail;

public:
	BOOL	Begin(VOID)
	{
		ZeroMemory(m_Queue, sizeof(m_Queue));
		m_QueueHead = m_QueueTail = 0;

		return TRUE;
	}	
	BOOL	End(VOID){ return TRUE; }

	BOOL	Push(T data)
	{
		DWORD	TempTail = (m_QueueTail + 1) % MAX_QUEUE_LENGTH;
		if (TempTail == m_QueueHead)
			return FALSE;

		CopyMemory(&m_Queue[TempTail], &data, sizeof(T));
		m_QueueTail = TempTail;
		return TRUE;
	}

	BOOL	Pop(T& data)
	{
		if (m_QueueHead == m_QueueTail)
			return FALSE;

		DWORD	TempHead = (m_QueueHead + 1) % MAX_QUEUE_LENGTH;

		CopyMemory(&data, &m_Queue[TempHead], sizeof(T));

		m_QueueHead = TempHead;

		return TRUE;
	}

	BOOL	IsEmpty(VOID)
	{
		if (m_QueueHead == m_QueueTail) return TRUE;
		return FALSE;
	}

public:
	CCircularQueue();
	~CCircularQueue();
};

