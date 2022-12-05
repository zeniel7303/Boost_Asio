#pragma once
#include "CServer.h"

class CPacketHeader;

class CBuffer
{
private:
	const unsigned short	m_maxSize;
	unsigned short			m_useSize;

	unsigned short			m_readIndex;
	unsigned short			m_writeIndex;

	char*					m_buffer;

public:
	CBuffer(unsigned short _size);
	~CBuffer();

	bool Push(const char* _data, unsigned short _size);
	bool OnPush(unsigned short _size);
	char* Front(unsigned short& _size, int& _errorCode);
	void Pop();
	void Clear();

	char* GetWritePoint();
	char* GetReadPoint();

	unsigned short GetUsableSize();
	unsigned short GetBufferSize();
};