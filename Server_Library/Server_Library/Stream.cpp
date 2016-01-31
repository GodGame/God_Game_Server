#include "stdafx.h"
#include "Stream.h"


CStream::CStream()
{
	m_BufferPointer = NULL;
	m_Length = 0;
}


CStream::~CStream()
{
}

BOOL CStream::SetBuffer(BYTE* buffer)
{
	if (!buffer)
		return FALSE;

	m_BufferPointer = buffer;
	m_Length = 0;

	return TRUE;
}

BOOL CStream::ReadInt32(INT* data)
{
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(INT));

	m_Length += sizeof(INT);

	return TRUE;
}

BOOL CStream::ReadDWORD(DWORD* data)
{
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(DWORD));

	m_Length += sizeof(DWORD);

	return TRUE;
}

BOOL CStream::ReadDWORD_PTR(DWORD_PTR* data)
{
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(DWORD_PTR));

	m_Length += sizeof(DWORD_PTR);

	return TRUE;
}

BOOL CStream::ReadByte(BYTE* data)
{
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(BYTE));

	m_Length += sizeof(BYTE);

	return TRUE;
}

BOOL CStream::ReadBytes(BYTE* data, DWORD length)
{
	CopyMemory(data, m_BufferPointer + m_Length, length);

	m_Length += length;

	return TRUE;
}

BOOL CStream::ReadFloat(FLOAT* data)
{
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(FLOAT));

	m_Length += sizeof(FLOAT);

	return TRUE;
}

BOOL CStream::ReadInt64(INT64* data)
{
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(INT64));

	m_Length += sizeof(INT64);

	return TRUE;
}

BOOL CStream::ReadUSHORT(USHORT* data)
{
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(USHORT));

	m_Length += sizeof(USHORT);

	return TRUE;
}

BOOL CStream::ReadSHORT(SHORT* data)
{
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(SHORT));

	m_Length += sizeof(SHORT);

	return TRUE;
}

BOOL CStream::ReadBOOL(BOOL* data)
{
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(BOOL));

	m_Length += sizeof(BOOL);

	return TRUE;
}

BOOL CStream::WriteInt32(INT data)
{
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(INT));

	m_Length += sizeof(INT);

	return TRUE;
}

BOOL CStream::WriteDWORD(DWORD data)
{
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(DWORD));

	m_Length += sizeof(DWORD);

	return TRUE;
}

BOOL CStream::WriteDWORD_PTR(DWORD_PTR data)
{
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(DWORD_PTR));

	m_Length += sizeof(DWORD_PTR);

	return TRUE;
}

BOOL CStream::WriteByte(BYTE data)
{
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(BYTE));

	m_Length += sizeof(BYTE);

	return TRUE;
}

BOOL CStream::WriteBytes(BYTE* data, DWORD length)
{
	CopyMemory(m_BufferPointer + m_Length, data, length);

	m_Length += length;

	return TRUE;
}

BOOL CStream::WriteFloat(FLOAT data)
{
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(FLOAT));

	m_Length += sizeof(FLOAT);

	return TRUE;
}

BOOL CStream::WriteInt64(INT64 data)
{
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(INT64));

	m_Length += sizeof(INT64);

	return TRUE;
}

BOOL CStream::WriteUSHORT(USHORT data)
{
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(USHORT));

	m_Length += sizeof(USHORT);

	return TRUE;
}

BOOL CStream::WriteSHORT(SHORT data)
{
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(SHORT));

	m_Length += sizeof(SHORT);

	return TRUE;
}

BOOL CStream::WriteBOOL(BOOL data)
{
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(BOOL));

	m_Length += sizeof(BOOL);

	return TRUE;
}

DWORD CStream::GetLength(VOID)
{
	return m_Length;
}
