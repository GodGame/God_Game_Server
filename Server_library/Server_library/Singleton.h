#pragma once
#define NETLIB_API_EX __declspec(dllexport)
#define NETLIB_API_IM __declspec(dllimport)
#include <iostream>
#include <WinSock2.h>
#include <list>
using namespace std;
#define DECLEAR_SINGLETON(className)\
	public:\
	static className*	Instance();\
	virtual void		releaseInstance();\
	private:\
	static className*	m_pInstance;

#define CREATE_FUNCTION(className, funcName)\
	static className*	##funcName()\
	{\
		return classNAme::Instance();\
	}

#define IMPLEMENT_SINGLETON(className)\
	className*	className::m_pInstance = NULL;\
	className*	className::Instance()\
		{\
			if(NULL == m_pInstance)\
				{\
					m_pInstance = new className;\
				}\
		return m_pInstance;\
		}\
		void className::releaseInstance()\
		{\
			if(NULL != m_pInstance)\
			{\
				delete m_pInstance;\
				m_pInstance = NULL;\
			}\
}

class NETLIB_API_EX CSingleton
{
public:
	typedef list<CSingleton*> SINGLETON_LIST;

	CSingleton();
	virtual ~CSingleton();

public:
	virtual void releaseInstance() = 0;
	static void releaseAll();

private:
	static SINGLETON_LIST m_listSingleton;

};

