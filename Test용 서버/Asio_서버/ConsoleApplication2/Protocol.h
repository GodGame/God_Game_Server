#pragma once 
#pragma warning(disable:4996)
#include <memory.h>
//#include <DirectXMath.h>
#include <xnamath.h>

const unsigned short PORT_NUMBER = 31400;

const int MAX_RECEIVE_BUFFER_LEN = 512;

const int MAX_NAME_LEN = 17;
const int MAX_MESSAGE_LEN = 129;




struct PACKET_HEADER
{
	short nID;
	short nSize;
};
enum PLAYER_DIRECTION
{
	DIR_NONE = 0x00,
	DIR_FORWARD = 0x01,
	DIR_BACKWARD = 0x02,
	DIR_LEFT = 0x04,
	DIR_RIGHT = 0x08,
	DIR_UP = 0x10,
	DIR_DOWN = 0x20
};



//  ��Ŷ
const short REQ_IN = 1;
// PKT_REQ_IN

const short RES_IN = 2;
// PKT_RES_IN

const short REQ_CHAT = 6;
// PKT_REQ_CHAT

const short NOTICE_CHAT = 7;
// PKT_NOTICE_CHAT

const short PLAYER_MOVE = 8;


struct PKT_REQ_IN : public PACKET_HEADER
{
	void Init()
	{
		nID = REQ_IN;
		nSize = sizeof(PKT_REQ_IN);
		memset(szName, 0, MAX_NAME_LEN);
	}

	char szName[MAX_NAME_LEN];
};

struct PKT_RES_IN : public PACKET_HEADER
{
	void Init()
	{
		nID = RES_IN;
		nSize = sizeof(PKT_RES_IN);
		bIsSuccess = false;
	}

	bool bIsSuccess;
};

struct PKT_REQ_CHAT : public PACKET_HEADER
{
	void Init()
	{
		nID = REQ_CHAT;
		nSize = sizeof(PKT_REQ_CHAT);
		memset(szMessage, 0, MAX_MESSAGE_LEN);
	}

	char szMessage[MAX_MESSAGE_LEN];
};

struct PKT_NOTICE_CHAT : public PACKET_HEADER
{
	void Init()
	{
		nID = NOTICE_CHAT;
		nSize = sizeof(PKT_NOTICE_CHAT);
		memset(szName, 0, MAX_NAME_LEN);
		memset(szMessage, 0, MAX_MESSAGE_LEN);
	}

	char szName[MAX_NAME_LEN];
	char szMessage[MAX_MESSAGE_LEN];
};

struct PKT_PLAYER_MOVE : public PACKET_HEADER
{
	void Init()
	{
		nID = PLAYER_MOVE;
		nSize = sizeof(PKT_PLAYER_MOVE);
		eCharDirection = DIR_NONE;
	}
	PLAYER_DIRECTION eCharDirection;
};