#pragma once
#include "Protocol.h"

class CTestClient
{
	io_service& m_IOService;
	ip::tcp::socket m_Socket;

	std::array<char, 512> m_ReceiveBuffer;
	int m_nPacketBufferMark;
	char m_PacketBuffer[MAX_RECEIVE_BUFFER_LEN * 2];

	CRITICAL_SECTION m_lock;
	std::deque< char* > m_SendDataQueue;

	Position3f m_tPlayerPos[2];
	int m_iSessionID;

	bool m_bIsLogin;
public:
	CTestClient(boost::asio::io_service& io_service);
	~CTestClient();

public:

	bool IsConnecting();
	void LoginOK();
	bool IsLogin();
	void Connect(boost::asio::ip::tcp::endpoint endpoint);
	void Close();
	void PacketUpdate();
	void PostSend(const bool bImmediately, const int nSize, char* pData);

public:
	Position3f GetPosition(int _SessionID)
	{
		return m_tPlayerPos[_SessionID];
	}

	int GetSession()
	{
		return m_iSessionID;
	}


private:

	void PostReceive();
	void handle_connect(const boost::system::error_code& error);
	void handle_write(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);
	void ProcessPacket(const char*pData);
public:

};

