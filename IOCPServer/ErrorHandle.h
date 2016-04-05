#pragma once
#include "define.h"
class CErrorHandle
{

public:
	void error_display(char *msg, int err_no);
public:
	CErrorHandle();
	~CErrorHandle();
};

