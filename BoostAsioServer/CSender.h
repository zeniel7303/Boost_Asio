#pragma once
#include "CServer.h"
#include "CSendPool.h"
#include "CPacketHandler.h"

//패킷 전송시 사용
//SendPool에서 패킷 전송 할당받아 사용 후 반납.

class CSender
{
private:
	CDataChunk*		m_tempChunk;
	int					m_index;
	unsigned short		m_count;

	static CSendPool	m_sendPool;

public:
	CSender();
	~CSender();

	static int Init(unsigned short _sendCount);

	template<typename T>
	static std::shared_ptr<CSender> Alloc();
	int DeAlloc();

	void SetSendDataChunk(CDataChunk* _chunk, int _chunkIndex, unsigned short _chunkCount);

	template<typename T>
	T* GetWritePointer();
	void* GetSendPointer();
	unsigned short GetSendSize();
};

#include "CSender.Inl"