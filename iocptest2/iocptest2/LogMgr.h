#pragma once
#include "Common.h"
#include "Struct.h"
#include "enum.h"

#define LOG_TYPE_DISCONNECTION_PATH				"Log\\Disconnection\\"
#define LOG_TYPE_CONNECTION_PATH				"Log\\Connection\\"
#define LOG_TYPE_GAME_SYSTEM_PATH				"Log\\GameSystem\\"
#define LOG_TYPE_SERVER_SYSTEM_PATH				"Log\\ServerSystem\\"
#define LOG_TYPE_OPERATION_SYSTEM_PATH			"Log\\OperationSystem\\"
#define LOG_TYPE_PACKET_PATH					"Log\\Packet\\"



class CLogMgr
{

public:
	void InsertLog(LogType eType, char* FileName, string Function, int Line, string Message);
	char* GetFileName(char file_path[]);
	CLogMgr();
	~CLogMgr();

public:
	static CLogMgr & GetInstance();
};

