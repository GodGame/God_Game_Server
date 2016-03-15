#pragma once
#include <memory.h>
#include "Define.h"
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


// ÆÐÅ¶

const short REQ_IN = 1;
const short RES_IN = 2;

const short REQ_CHAT = 6;

const short NOTICE_CHAT = 7;
const short PLAYER_MOVE = 8;
const short PLAYER_POSITION = 9;

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
		memset(szName, 0, MAX_NAME_LEN);
		eCharDirection = DIR_NONE;
	}
	char szName[MAX_NAME_LEN];
	PLAYER_DIRECTION eCharDirection;
};

struct PKT_PLAYER_POSITION : public PACKET_HEADER
{
	void Init()
	{
		nID = PLAYER_POSITION;
		nSize = sizeof(PKT_PLAYER_POSITION);
		memset(tCharPos, 0, sizeof(tCharPos));
		iSessionID = 0;

	}
	Position3f tCharPos[2];
	int iSessionID;
};