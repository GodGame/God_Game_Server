#include "Common.h"
#include "Timer.h"
#include "IOCPTCPServer.h"

CGameTimer::CGameTimer()
{
	if (QueryPerformanceFrequency((LARGE_INTEGER *)&m_PerformanceFrequency))
	{
		m_bHardwareHasPerformanceCounter = TRUE;
		QueryPerformanceCounter((LARGE_INTEGER *)&m_nLastTime);
		m_fTimeScale = 1.0f / m_PerformanceFrequency;
	}
	else
	{
		m_bHardwareHasPerformanceCounter = FALSE;
		m_nLastTime = ::timeGetTime();
		m_fTimeScale = 0.001f;
	}

	m_nSampleCount = 0;
	m_nCurrentFrameRate = 0;
	m_FramePerSecond = 0;
	m_fFPSTimeElapsed = 0.0f;
	m_pServer = &CIOCPTCPServer::GetInstance();
}

CGameTimer::~CGameTimer()
{
}

void CGameTimer::TimerRun()
{
	while (true)
	{
		Sleep(1);
		Tick();
		m_test = m_fTimeElapsed;
		if(m_fTimeElapsed == m_test)
			continue;
		else
		{
			OVERLAPPED_EX event_over;
			event_over.operation = OP_TIME;
			PostQueuedCompletionStatus(CIOCPTCPServer::GetInstance()._hIOCP, 1, 9999, reinterpret_cast<LPOVERLAPPED>(&event_over));
		}
	}
}

void CGameTimer::Tick()
{
	if (m_bHardwareHasPerformanceCounter)
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&m_nCurrentTime);
	}
	else
	{
		m_nCurrentTime = ::timeGetTime();
	}

	//마지막으로 이 함수를 호출한 이후 경과한 시간을 계산한다.
	float fTimeElapsed = (m_nCurrentTime - m_nLastTime) * m_fTimeScale;

	//현재 시간을 m_nLastTime에 저장한다.
	m_nLastTime = m_nCurrentTime;

	/* 마지막 프레임 처리 시간과 현재 프레임 처리 시간의 차이가 1초보다 작으면 현재 프레임 처리 시간을 m_fFrameTime[0]에 저장한다. */
	if (fabsf(fTimeElapsed - m_fTimeElapsed) < 1.0f)
	{
		memmove(&m_fFrameTime[1], m_fFrameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));
		m_fFrameTime[0] = fTimeElapsed;
		if (m_nSampleCount < MAX_SAMPLE_COUNT) m_nSampleCount++;
	}

	//초당 프레임 수를 1 증가시키고 현재 프레임 처리 시간을 누적하여 저장한다.
	m_FramePerSecond++;
	m_fFPSTimeElapsed += fTimeElapsed;
	if (m_fFPSTimeElapsed > 1.0f)
	{
		m_nCurrentFrameRate = m_FramePerSecond;
		m_FramePerSecond = 0;
		m_fFPSTimeElapsed = 0.0f;
	}

	//누적된 프레임 처리 시간의 평균을 구하여 프레임 처리 시간을 구한다.
	m_fTimeElapsed = 0.0f;
	for (ULONG i = 0; i < m_nSampleCount; i++) m_fTimeElapsed += m_fFrameTime[i];
	if (m_nSampleCount > 0) m_fTimeElapsed /= m_nSampleCount;
}

unsigned long CGameTimer::GetFrameRate()
{

	return(m_nCurrentFrameRate);
}

float CGameTimer::GetTimeElapsed()
{
	return(m_fTimeElapsed);
}

CGameTimer & CGameTimer::GetInstance()
{
	static CGameTimer instance;
	return instance;
}


