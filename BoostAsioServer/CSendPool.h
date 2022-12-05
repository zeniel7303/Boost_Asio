#pragma once
#include "CServer.h"
#include "CSharedLock.h"

struct CDataChunk
{
	char mBuffer[SENDDATACHUNKSIZE];
};

class CSender;

class CSendPool : public CSharedLock<CSendPool>
{
private:
	CDataChunk*				m_chunk;
	std::vector<CSender>	m_senderList;
	std::vector<bool>		m_flag;
	unsigned short			m_chunkCount;
	unsigned short			m_useSize;

public:
	CSendPool();
	~CSendPool();

	int Init(unsigned short _count);
	std::shared_ptr<CSender> Alloc(unsigned short _sendSize);
	bool DeAlloc(int _index, unsigned short _count);

	int GetIndex(unsigned short _count);
	unsigned short GetUsableSize();
};