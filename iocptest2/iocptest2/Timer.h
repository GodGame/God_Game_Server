#pragma once
const ULONG MAX_SAMPLE_COUNT = 50; // 50ȸ�� ������ ó���ð��� �����Ͽ� ����Ѵ�.
class CIOCPTCPServer;
class CGameTimer
{
private:
	bool m_bHardwareHasPerformanceCounter;	// ��ǻ�Ͱ� Performance Counter�� ������ �ִ� ��
	float m_fTimeScale;			// Scale Counter�� ��
	float m_fTimeElapsed;		// ������ ������ ���� ������ �ð�
	float m_test;
	__int64 m_nCurrentTime;	// ������ �ð�
	__int64 m_nLastTime;		// ������ �������� �ð�
	__int64 m_PerformanceFrequency;	// ��ǻ���� Performance Frequency

	float m_fFrameTime[MAX_SAMPLE_COUNT];	// ������ �ð��� �����ϱ� ���� �迭
	ULONG m_nSampleCount;							// ������ ������ Ƚ��

	unsigned long m_nCurrentFrameRate;			// ������ ������ ����Ʈ
	unsigned long m_FramePerSecond;				// �ʴ� ������ ��
	float m_fFPSTimeElapsed;							// ������ ����Ʈ ��� �ҿ� �ð�
	CIOCPTCPServer* m_pServer;

public:
	CGameTimer();
	virtual	~CGameTimer();
	void TimerRun();
	void Tick();	// �ð� ����
	unsigned long GetFrameRate();	// ������ ����Ʈ ��ȯ
	float GetTimeElapsed();			// ������ ��� �ð� ��ȯ

	static CGameTimer & GetInstance();
};

