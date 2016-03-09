#pragma warning(disable:4996)
#include "ChatClient.h"
#include "Protocol.h"


#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) 

#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1) 


int main()
{
	boost::asio::io_service io_service;
	int iKey = 0;
	PLAYER_DIRECTION eDirection=DIR_NONE;

	auto endpoint = boost::asio::ip::tcp::endpoint(
		boost::asio::ip::address::from_string("127.0.0.1"),
		PORT_NUMBER);

	ChatClient Cliet(io_service);
	Cliet.Connect(endpoint);

	boost::thread thread(boost::bind(&boost::asio::io_service::run, &io_service));


	char szInputMessage[MAX_MESSAGE_LEN * 2] = { 0, };

//	while (std::cin.getline(szInputMessage, MAX_MESSAGE_LEN))
///	{
	/*	if (strnlen_s(szInputMessage, MAX_MESSAGE_LEN) == 0)
		{
			break;
		}
*/
		if (Cliet.IsConnecting() == false)
		{
			std::cout << "������ ������� �ʾҽ��ϴ�" << std::endl;
		//	continue;
		}

		if (Cliet.IsLogin() == false)
		{
			PKT_REQ_IN SendPkt;
			SendPkt.Init();
			strncpy_s(SendPkt.szName, MAX_NAME_LEN, szInputMessage, MAX_NAME_LEN - 1);

			Cliet.PostSend(false, SendPkt.nSize, (char*)&SendPkt);
		}
		else
		{
			PKT_REQ_CHAT SendPkt;
			SendPkt.Init();
			strncpy_s(SendPkt.szMessage, MAX_MESSAGE_LEN, szInputMessage, MAX_MESSAGE_LEN - 1);

			Cliet.PostSend(false, SendPkt.nSize, (char*)&SendPkt);
		}
//	}
		//while (true)
		//{
		//	if (KEYDOWN(VK_LEFT))
		//	{
		//		PKT_PLAYER_MOVE SendPkt;
		//		PLAYER_DIRECTION eDirection;
		//		SendPkt.Init();
		//		SendPkt.eCharDirection = DIR_UP;
		//		//memcpy_s(&SendPkt.eCharDirection, sizeof(PLAYER_DIRECTION), &eDirection, MAX_MESSAGE_LEN - 1);
		//		Cliet.PostSend(false, SendPkt.nSize, (char*)&SendPkt);
		//		std::cout << "Test(KEYDOWN)" << std::endl;
		//		if (std::cin.peek() != EOF)
		//		{
		//			std::cin.ignore(std::numeric_limits<std::streamsize>::max());
		//		}
		//	}
		//	else if (KEYUP(VK_LEFT))
		//	{
		//		std::cout << "Test(KEYUP)" << std::endl;
		//		//break;
		//		/*if (std::cin.peek() != EOF)
		//		{
		//			std::cin.ignore(std::numeric_limits<std::streamsize>::max());
		//		}*/
		//	}
		//
		////	rewind(stdin);
		//	//else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		//	//{
		//	//	PKT_PLAYER_MOVE SendPkt;
		//	//	PLAYER_DIRECTION eDirection;
		//	//	SendPkt.Init();
		//	//	SendPkt.eCharDirection = DIR_LEFT;
		//	//	//memcpy_s(&SendPkt.eCharDirection, sizeof(PLAYER_DIRECTION), &eDirection, MAX_MESSAGE_LEN - 1);
		//	//	Cliet.PostSend(false, SendPkt.nSize, (char*)&SendPkt);
		//	//}
		//	//else
		//	//{
		//	//	continue;
		//	//}

		//}

	io_service.stop();

	Cliet.Close();

	thread.join();

	std::cout << "Ŭ���̾�Ʈ�� ������ �ּ���" << std::endl;

	return 0;
}