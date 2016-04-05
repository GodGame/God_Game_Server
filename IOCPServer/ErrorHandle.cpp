#include "ErrorHandle.h"



void CErrorHandle::error_display(char * msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("%s", msg);
	wprintf(L"¿¡·¯[%s]\n", lpMsgBuf);
	LocalFree(lpMsgBuf);
}

CErrorHandle::CErrorHandle()
{
}


CErrorHandle::~CErrorHandle()
{
}
