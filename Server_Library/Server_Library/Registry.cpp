#include "stdafx.h"
#include "Registry.h"



CRegistry::CRegistry()
{
	m_RootKey = NULL;
	m_IsOpen = FALSE;
}


CRegistry::~CRegistry()
{
}

BOOL CRegistry::Open(HKEY rootKey, LPCTSTR subKey)
{
	if (!subKey)
		return FALSE;

	if (m_IsOpen)
		return FALSE;

	if (RegOpenKey(rootKey, subKey, &m_RootKey) != ERROR_SUCCESS)
		return FALSE;

	m_IsOpen = TRUE;

	return TRUE;
}

BOOL CRegistry::Close(VOID)
{
	if (RegCloseKey(m_RootKey) != ERROR_SUCCESS)
		return FALSE;

	m_IsOpen = FALSE;

	return TRUE;
}

BOOL CRegistry::CreateKey(HKEY rootKey, LPCTSTR subKey)
{
	if (!subKey)
		return FALSE;

	if (m_IsOpen)
		return FALSE;

	if (RegCreateKey(m_RootKey, subKey, &m_RootKey) != ERROR_SUCCESS)
		return FALSE;

	m_IsOpen = TRUE;

	return TRUE;
}

BOOL CRegistry::DeleteKey(HKEY rootKey, LPCTSTR subKey)
{
	if (!subKey)
		return FALSE;

	if (RegDeleteKey(rootKey, subKey) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;

}

BOOL CRegistry::SetValue(LPCTSTR valueName, LPCTSTR value)
{
	if (!valueName || !value)
		return FALSE;

	if (!m_IsOpen)
		return FALSE;

	if (RegSetValueEx(m_RootKey, valueName, 0, REG_SZ, (BYTE*)value, (DWORD)_tcslen(value)*sizeof(TCHAR)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL CRegistry::SetValue(LPCTSTR valueName, DWORD value)
{
	if (!valueName || !value)
		return FALSE;

	if (!m_IsOpen)
		return FALSE;

	if (RegSetValueEx(m_RootKey, valueName, 0, REG_DWORD, (BYTE*)value, sizeof(DWORD) != ERROR_SUCCESS))
		return FALSE;

	return TRUE;
}

BOOL CRegistry::SetValueForMultiSz(LPCTSTR valueName, LPCTSTR value, DWORD byteLength)
{
	if (!valueName || !value)
		return FALSE;

	if (!m_IsOpen)
		return FALSE;

	if (RegSetValueEx(m_RootKey, valueName, 0, REG_MULTI_SZ, (BYTE*)value, byteLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL CRegistry::GetValue(LPCTSTR valueName, LPCTSTR value, LPDWORD bufferLength)
{
	DWORD valueType = 0;

	if (!valueName || !value || !bufferLength)
		return FALSE;

	if (!m_IsOpen)
		return FALSE;

	if (RegQueryValueEx(m_RootKey, valueName, 0, &valueType, (BYTE*)value, bufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;


}

BOOL CRegistry::GetValue(LPCTSTR valueName, LPCTSTR value)
{
	DWORD valueType = 0;
	DWORD bufferLength = sizeof(DWORD); 

	if (!valueName || !value)
		return FALSE;

	if (!m_IsOpen)
		return FALSE;

	if (RegQueryValueEx(m_RootKey, valueName, 0, &valueType, (BYTE*)value, &bufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;

}

BOOL CRegistry::DeleteValue(LPCTSTR valueName)
{
	if (!valueName)
		return FALSE;

	if (!m_IsOpen)
		return FALSE;

	if (RegDeleteValue(m_RootKey, valueName) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}


