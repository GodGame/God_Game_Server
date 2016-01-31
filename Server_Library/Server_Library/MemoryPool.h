#pragma once
#include "stdafx.h"

template<class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool
{
	static UCHAR*	m_FreePointer;
public:
	static VOID* operator new(std::size_t allocLength)
	{
		assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR*));
		if (!m_FreePointer)
			allocBlock();
		
		UCHAR*	ReturnPointer = m_FreePointer;
		
		m_FreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);

		return ReturnPointer;
	}
		static VOID		operator delete(VOID* deletePointer)
	{
		*reinterpret_cast<UCHAR**>(deletePointer) = m_FreePointer;
		m_FreePointer = static_cast<UCHAR*>(deletePointer);
	}

private:
	static VOID		allocBlock()
	{
		m_FreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];

		UCHAR** Current = reinterpret_cast<UCHAR**>(m_FreePointer);

		UCHAR* Next = m_FreePointer;

		for (INT i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			Next += sizeof(T);
			*Current = Next;
			Current = reinterpret_cast<UCHAR**>(Next);
		}

		*Current = 0;
	}
	
protected:
	~CMemoryPool()
	{
	}
};

template<class T, int ALLOC_BLOCK_SIZE>
UCHAR*	CMemoryPool<T, ALLOC_BLOCK_SIZE>::m_FreePointer;

