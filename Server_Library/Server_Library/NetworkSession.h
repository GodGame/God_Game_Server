#pragma once
class CNetworkSession : public CMultiThreadSync<CNetworkSession>
{
	OVERLAPPED_EX	

public:
	CNetworkSession();
	virtual ~CNetworkSession();
};

