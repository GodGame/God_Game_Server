#pragma once
class CIniFile
{
	TCHAR	m_FileName[MAX_PATH];

public:

	BOOL	Open(LPCTSTR fileName);
	BOOL	Close(VOID);
	BOOL	SetValue(LPCTSTR keyName, LPCTSTR valueName, LPCTSTR value);
	BOOL	SetValue(LPCTSTR keyName, LPCTSTR valueName, DWORD value);
	BOOL	SetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT value);

	BOOL	GetValue(LPCTSTR keyName, LPCTSTR valueName, LPTSTR value, LPDWORD bufferLength);
	BOOL	GetValue(LPCTSTR keyName, LPCTSTR valueName, LPDWORD value);
	BOOL	GetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT* value);

public:
	CIniFile();
	~CIniFile();
};


