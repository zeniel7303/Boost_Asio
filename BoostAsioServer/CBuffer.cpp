#include "CBuffer.h"

#include "CHandler.h"

CBuffer::CBuffer(unsigned short _size) : m_maxSize(_size), m_useSize(0),
m_readIndex(0), m_writeIndex(0), m_buffer(nullptr)
{
	m_buffer = new char[m_maxSize];
	memset(m_buffer, 0, m_maxSize);
}

CBuffer::~CBuffer()
{
	delete[] m_buffer;
}

bool CBuffer::Push(const char* _data, unsigned short _size)
{
	assert(_data != nullptr && "data error");
	assert(_size > 0 && "size error");

	if (_size > GetUsableSize()) return false;

	memcpy_s(m_buffer + m_writeIndex, GetUsableSize(), _data, _size);
	m_useSize += _size;
	m_writeIndex += _size;

	return true;
}

bool CBuffer::OnPush(unsigned short _size)
{
	assert(GetUsableSize() >= _size && "size error");
	assert(m_readIndex == 0 && "readIndex error");

	if (m_writeIndex + _size > m_maxSize) return false;

	m_writeIndex += _size;
	m_useSize += _size;

	return true;
}

char* CBuffer::Front(unsigned short& _size, int& _errorCode)
{
	if (m_useSize - m_readIndex < 0) return nullptr;

	_errorCode = 0;
	char* data = m_buffer + m_readIndex;
	_size = reinterpret_cast<CPacketHeader*>(data)->m_dataSize;

	if (_size > m_maxSize)
	{
		return nullptr;
	}

	if (_size > m_useSize - m_readIndex) return nullptr;

	m_readIndex += _size;

	return data;
}

void CBuffer::Pop()
{
	assert(m_useSize >= m_readIndex && "size error");

	memmove_s(m_buffer, m_maxSize, m_buffer + m_readIndex, m_maxSize - m_readIndex);
	m_writeIndex -= m_readIndex;
	m_useSize -= m_readIndex;
	m_readIndex = 0;
}

void CBuffer::Clear()
{
	m_readIndex = 0;
	m_writeIndex = 0;
	m_useSize = 0;
}

char* CBuffer::GetWritePoint()
{
	assert(m_writeIndex < m_maxSize && "writePoint error");

	return &(m_buffer[m_writeIndex]);
}

char* CBuffer::GetReadPoint()
{
	assert(m_readIndex < m_maxSize && "readPoint error");

	return &(m_buffer[m_readIndex]);
}

unsigned short CBuffer::GetUsableSize()
{
	return m_maxSize - m_useSize;
}

unsigned short CBuffer::GetBufferSize()
{
	return m_maxSize;
}