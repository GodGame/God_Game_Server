#include "Singleton.h"
CSingleton::SINGLETON_LIST	CSingleton::m_listSingleton;


CSingleton::CSingleton()
{
	m_listSingleton.push_back(this);
}


CSingleton::~CSingleton()
{
	SINGLETON_LIST::iterator singleton_it = m_listSingleton.begin();
	while (singleton_it != m_listSingleton.end())
	{
		if ((*singleton_it) == this)
			break;
		++singleton_it;
	}
	m_listSingleton.erase(singleton_it);
}

void CSingleton::releaseAll()
{
	SINGLETON_LIST::reverse_iterator singleton_rit = m_listSingleton.rbegin();
	while (singleton_rit != m_listSingleton.rend())
	{
		(*singleton_rit)->releaseInstance();
		++singleton_rit;
	}
	m_listSingleton.clear();
	
}
