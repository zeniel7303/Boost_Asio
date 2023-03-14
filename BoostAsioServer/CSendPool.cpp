#include "CSendPool.h"
#include "CSender.h"

CSendPool::CSendPool() : m_chunk(nullptr), m_chunkCount(0),
m_flag(), m_senderList(), m_useSize(0)
{

}

CSendPool::~CSendPool()
{
	delete[] m_chunk;
}

int CSendPool::Init(unsigned short _count)
{
	assert(m_chunk == nullptr && "chunk error");
	assert(m_useSize == 0 && "useSIze error");

	try
	{
		m_chunk = new CDataChunk[_count];

		m_chunkCount = _count;

		m_flag.reserve(_count);
		m_senderList.reserve(_count);
	}
	catch (...)
	{
		assert(0 && "init failed");
		return E_INITIALIZE_FAILED;
	}

	for (int i = 0; i < _count; ++i)
	{
		m_flag.emplace_back(false);
		m_senderList.emplace_back(CSender());
	}

	return 0;
}

std::shared_ptr<CSender> CSendPool::Alloc(unsigned short _sendSize)
{
	int index = 0;
	unsigned short count = (_sendSize / sizeof(CDataChunk)) + 1;

	CSender* sender = nullptr;
	CDataChunk* sendChunk = nullptr;

	{
		WriteLock writeLock(*this);

		index = GetIndex(count);

		if (index == -1) return nullptr;

		sender = &(m_senderList[index]);
		sendChunk = &(m_chunk[index]);

		for (int i = index; i < (index + count); ++i)
		{
			m_flag[i] = true;
		}

		m_useSize += (count * sizeof(CDataChunk));
	}

	sender->SetSendDataChunk(sendChunk, index, count);

	//c++ shared_ptr delete lambda
	//https://stackoverflow.com/questions/13633737/using-a-custom-deleter-for-stdshared-ptr-on-a-direct3d11-object
	return std::shared_ptr<CSender>(sender, [this](CSender* _sender)
	{
		// 실패시 파괴
		_sender->DeAlloc();
	});
}

bool CSendPool::DeAlloc(int _index, unsigned short _count)
{
	assert(_index <= m_chunkCount && "index error");
	assert(_count < m_chunkCount && "count error");

	{
		WriteLock writeLock(*this);

		for (int i = _index; i < (_index + _count); ++i)
		{
			if (m_flag[i] == false)
				return false;

			m_flag[i] = false;
			m_useSize -= sizeof(CDataChunk);
		}
	}

	return true;
}

int CSendPool::GetIndex(unsigned short _count)
{
	int filledCount = 0;
	int allocIndex = -1;
	int flagIndex = 0;

	for (const auto& flag : m_flag)
	{
		//false다 = 미사용 중
		if (!flag)
		{
			++filledCount;

			if (allocIndex == -1)
				allocIndex = flagIndex;

			if (filledCount == _count) break;
		}
		else
		{
			filledCount = 0;
			allocIndex = -1;
		}

		++flagIndex;
	}

	if (allocIndex != -1 && filledCount != _count) 
		allocIndex = -1;

	return allocIndex;
}

unsigned short CSendPool::GetUsableSize()
{
	ReadLock readLock(*this);

	return m_useSize;
}