#include "CSender.h"

CSendPool CSender::m_sendPool;

CSender::CSender() : m_tempChunk(nullptr), m_index(-1), m_count(0)
{

}

CSender::~CSender()
{

}

int CSender::Init(unsigned short _sendCount)
{
	return m_sendPool.Init(_sendCount);
}

int CSender::DeAlloc()
{
	if (m_tempChunk == nullptr) return 0;

	assert(m_index >= 0 && "chunkIndex error");
	assert(m_count >= 0 && "chunkCount error");

	bool result = m_sendPool.DeAlloc(m_index, m_count);

	assert(result && "SendPool DeAlloc fail");

	m_tempChunk = nullptr;
	m_index = -1;
	m_count = 0;

	return 0;
}

void CSender::SetSendDataChunk(CDataChunk* _chunk, int _chunkIndex, unsigned short _chunkCount)
{
	assert(_chunk != nullptr && "chunk error");
	assert(_chunkIndex >= 0 && "chunkindex error");
	assert(_chunkCount > 0 && "chunkcount error");

	m_tempChunk = _chunk;
	m_index = _chunkIndex;
	m_count = _chunkCount;
}

void* CSender::GetSendPointer()
{
	return (reinterpret_cast<void*>(m_tempChunk));
}

unsigned short CSender::GetSendSize()
{
	if (m_tempChunk == nullptr)
		return 0;

	unsigned short dataSize = 
		(reinterpret_cast<CPacketHeader*>(m_tempChunk))->m_dataSize;

	if (dataSize == 0)
		return 0;

	return dataSize;
}