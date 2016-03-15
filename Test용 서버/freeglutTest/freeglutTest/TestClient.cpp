#include "stdafx.h"
#include "TestClient.h"

CTestClient::CTestClient(io_service& io_service) : m_IOService(io_service),
m_Socket(io_service)
{
	m_bIsLogin = false;
	memset(m_tPlayerPos, 0, sizeof(Position3f));
	InitializeCriticalSectionAndSpinCount(&m_lock, 4000);
}


CTestClient::~CTestClient()
{
	EnterCriticalSection(&m_lock);

	while (m_SendDataQueue.empty() == false)
	{
		delete[] m_SendDataQueue.front();
		m_SendDataQueue.pop_front();
	}

	LeaveCriticalSection(&m_lock);

	DeleteCriticalSection(&m_lock);
}

bool CTestClient::IsConnecting()
{
	return m_Socket.is_open();
}

void CTestClient::LoginOK()
{
	m_bIsLogin = true;
}

bool CTestClient::IsLogin()
{
	return m_bIsLogin;
}

void CTestClient::Connect(boost::asio::ip::tcp::endpoint endpoint)
{
	m_nPacketBufferMark = 0;

	m_Socket.async_connect(endpoint,
		boost::bind(&CTestClient::handle_connect, this,
			boost::asio::placeholders::error)
		);

}

void CTestClient::Close()
{
	if (m_Socket.is_open())
	{
		m_Socket.close();
	}
}

void CTestClient::PacketUpdate()
{
	PostReceive();
}

void CTestClient::PostSend(const bool bImmediately, const int nSize, char* pData)
{
	char* pSendData = nullptr;

	EnterCriticalSection(&m_lock);		// �� ����

	if (bImmediately == false)
	{
		pSendData = new char[nSize];
		memcpy(pSendData, pData, nSize);

		m_SendDataQueue.push_back(pSendData);
	}
	else
	{
		pSendData = pData;
	}

	if (bImmediately || m_SendDataQueue.size() < 2)
	{
		boost::asio::async_write(m_Socket, boost::asio::buffer(pSendData, nSize),
			boost::bind(&CTestClient::handle_write, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
			);
	}

	LeaveCriticalSection(&m_lock);		// �� �Ϸ�
}

void CTestClient::PostReceive()
{
	memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer));

	m_Socket.async_read_some
		(
			boost::asio::buffer(m_ReceiveBuffer),
			boost::bind(&CTestClient::handle_receive, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)

			);
}

void CTestClient::handle_connect(const boost::system::error_code& error)
{
	if (!error)
	{
		/*std::cout << "���� ���� ����" << std::endl;
		std::cout << "�̸��� �Է��ϼ���!!" << std::endl;

		PostReceive();*/
		//m_iSessionID = 
		PostReceive();
	}
	else
	{
		std::cout << "���� ���� ����. error No: " << error.value() << " error Message: " << error.message() << std::endl;
	}
}

void CTestClient::handle_write(const boost::system::error_code& error, size_t bytes_transferred)
{
	EnterCriticalSection(&m_lock);			// �� ����

	delete[] m_SendDataQueue.front();
	m_SendDataQueue.pop_front();

	char* pData = nullptr;

	if (m_SendDataQueue.empty() == false)
	{
		pData = m_SendDataQueue.front();
	}

	LeaveCriticalSection(&m_lock);			// �� �Ϸ�


	if (pData != nullptr)
	{
		PACKET_HEADER* pHeader = (PACKET_HEADER*)pData;
		PostSend(true, pHeader->nSize, pData);
	}
}

void CTestClient::handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		if (error == boost::asio::error::eof)
		{
			std::cout << "Ŭ���̾�Ʈ�� ������ ���������ϴ�" << std::endl;
		}
		else
		{
			std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
		}

		Close();
	}
	else
	{
		memcpy(&m_PacketBuffer[m_nPacketBufferMark], m_ReceiveBuffer.data(), bytes_transferred);

		int nPacketData = m_nPacketBufferMark + bytes_transferred;
		int nReadData = 0;

		while (nPacketData > 0)
		{
			if (nPacketData < sizeof(PACKET_HEADER))
			{
				break;
			}

			PACKET_HEADER* pHeader = (PACKET_HEADER*)&m_PacketBuffer[nReadData];

			if (pHeader->nSize <= nPacketData)
			{
				ProcessPacket(&m_PacketBuffer[nReadData]);

				nPacketData -= pHeader->nSize;
				nReadData += pHeader->nSize;
			}
			else
			{
				break;
			}
		}

		if (nPacketData > 0)
		{
			char TempBuffer[MAX_RECEIVE_BUFFER_LEN] = { 0, };
			memcpy(&TempBuffer[0], &m_PacketBuffer[nReadData], nPacketData);
			memcpy(&m_PacketBuffer[0], &TempBuffer[0], nPacketData);
		}

		m_nPacketBufferMark = nPacketData;


		PostReceive();
	}
}

void CTestClient::ProcessPacket(const char*pData)
{
	PACKET_HEADER* pheader = (PACKET_HEADER*)pData;

	switch (pheader->nID)
	{
	case RES_IN:
	{
		PKT_RES_IN* pPacket = (PKT_RES_IN*)pData;

		LoginOK();

		std::cout << std::boolalpha << "Ŭ���̾�Ʈ �α��� ���� ?: " << pPacket->bIsSuccess << std::endl;
	}
	break;
	case NOTICE_CHAT:
	{
		PKT_NOTICE_CHAT* pPacket = (PKT_NOTICE_CHAT*)pData;

		std::cout << pPacket->szName << ": " << pPacket->szMessage << std::endl;
	}
	break;
	case PLAYER_MOVE:
	{
		PKT_PLAYER_MOVE* pPacket = (PKT_PLAYER_MOVE*)pData;
		//std::cout << pPacket->szName << ": " << pPacket->eCharDirection << std::endl;
		break;
	}
	case PLAYER_POSITION:
	{
		PKT_PLAYER_POSITION* pPacket = (PKT_PLAYER_POSITION*)(pData);
		m_iSessionID = pPacket->iSessionID;
		switch (m_iSessionID)
		{
		case 0:
			m_tPlayerPos[0] = pPacket->tCharPos[0];
			break;
		case 1:
			m_tPlayerPos[1] = pPacket->tCharPos[1];
			break;
		}
		
		break;
	}


	}
}
