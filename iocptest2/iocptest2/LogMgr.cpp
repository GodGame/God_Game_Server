#include "LogMgr.h"


char* CLogMgr::GetFileName(char file_path[])
{

	char *file_name = "";

	while (*file_path)
	{
		if (*file_path == '\\' && (file_path + 1) != NULL)
		{
			file_name = file_path + 1;
		}
		else
		{
		}

		file_path++;
	}
	return file_name;
}

void CLogMgr::InsertLog(LogType eType, char* FileName,string Function,int Line,string Message)
{
	time_t testTime;
	char timeBuffer[256];
	char DateBuffer[256];
	char szFileName[256] = { 0, };
	char Path[256] = { 0, };

	time(&testTime);
	strftime(timeBuffer, strlen(timeBuffer), "[%Y년%m월%d일 %H시%M분%S초]", localtime(&testTime));

	DWORD dwWrittenBytes = 0;
	string string_File;
	string string_Function = Function;
	string string_Line = to_string(Line);
	string temp_time = timeBuffer;
	string temp_stringBuf;
	strftime(DateBuffer, strlen(DateBuffer), "%Y년%m월%d일", localtime(&testTime));
	string_File = GetFileName(FileName);
	switch (eType)
	{
	case LOGTYPE_CONNECTION:
		strcpy(Path, LOG_TYPE_CONNECTION_PATH);
		//Path = LOG_TYPE_CONNECTION_PATH;
		break;
	case LOGTYPE_DISCONNECTION:
		strcpy(Path, LOG_TYPE_DISCONNECTION_PATH);
		break;
	case LOGTYPE_GAME_SYSTEM:
		strcpy(Path, LOG_TYPE_GAME_SYSTEM_PATH);
		break;
	case LOGTYPE_SERVER_SYSTEM:
		strcpy(Path, LOG_TYPE_SERVER_SYSTEM_PATH);
		break;
	case LOGTYPE_OPERATION_SYSTEM:
		strcpy(Path, LOG_TYPE_OPERATION_SYSTEM_PATH);
		break;
	case LOGTYPE_PACKET:
		strcpy(Path, LOG_TYPE_PACKET_PATH);
		break;
	case LOGTYPE_TOTAL_NUM:
		break;
	default:
		break;
	}
	temp_stringBuf = temp_time+": File: "+string_File+" Function: "+string_Function+" Line: "+string_Line+" Message:"+Message+"\r\n";
	sprintf(szFileName, "%s%s.txt", Path, DateBuffer);
	ofstream outputFile(szFileName,ios::app|ios::out);
	outputFile << temp_stringBuf;
	outputFile.close();

}

CLogMgr::CLogMgr()
{
}


CLogMgr::~CLogMgr()
{
}
CLogMgr & CLogMgr::GetInstance()
{
	static CLogMgr instance;
	return instance;
}
