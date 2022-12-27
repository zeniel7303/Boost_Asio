#include "CRingBuffer.h"

CRingBuffer::CRingBuffer(int _size, int _tempSize)
{
	m_buffer = nullptr;
	m_readPoint = nullptr;
	m_parsingPoint = nullptr;
	m_writePoint = nullptr;
	m_bufferStartPoint = nullptr;
	m_bufferMaxSize = 0;
	m_tempBufferSize = 0;
	m_dataInBuffer = 0;

	m_bufferMaxSize = _size;
	m_tempBufferSize = _tempSize;

	m_buffer = new char[_size];
	if (m_buffer == nullptr) return;

	m_bufferStartPoint = m_buffer + m_tempBufferSize;
	m_bufferEndPoint = m_buffer + m_bufferMaxSize;
	m_writePoint = m_readPoint = m_bufferStartPoint;
	m_parsingPoint = m_readPoint = m_bufferStartPoint;
}

CRingBuffer::~CRingBuffer()
{
	SAFE_DELETE_ARRAY(m_buffer);
}

void CRingBuffer::Reset()
{
	m_bufferStartPoint = m_readPoint = m_parsingPoint = m_writePoint;
	m_dataInBuffer = 0;
}

void CRingBuffer::Write(int _size)
{
	m_writePoint += _size;

	if (m_writePoint >= (m_bufferEndPoint))
	{
		m_writePoint = m_bufferStartPoint;
	}
}

void CRingBuffer::Read(int _size)
{
	//버퍼의 끝지점부터 읽는 지점까지의 사이즈
	int remainedSize = (int)(m_bufferEndPoint - m_readPoint);

	//버퍼의 끝지점부터 읽는 지점까지 사이즈가 1밖에 없는 경우 -> TempBuffer쪽으로 복사함
	if (remainedSize == 1)
	{
		char* tempBuffer = m_bufferStartPoint - 1;
		*tempBuffer = *m_readPoint;
		m_readPoint = tempBuffer;
		//이후 readPoint는 패킷 사이즈만큼 더해준다.
		m_readPoint += _size;

		return;
	}

	//버퍼의 끝지점부터 읽는 지점이 같다 -> 버퍼의 첫지점으로 읽는 지점을 옮겨야한다.
	if (remainedSize == 0)
	{
		m_readPoint = m_bufferStartPoint;
	}

	//패킷 사이즈가 남아있는 크기보다 크다. -> 끝이 짤렸다.
	if (_size > remainedSize)
	{
		//맨 앞으로 복사한다.
		m_readPoint = (m_bufferStartPoint - remainedSize);
	}

	m_readPoint += _size;

	if (m_readPoint == m_bufferEndPoint)
	{
		m_readPoint = m_bufferStartPoint;
	}

	return;
}

char* CRingBuffer::CanParsing()
{
	m_dataInBuffer = GetDataInBuffer();

	//링버퍼 안에 데이터가 2이상 있는가? (패킷 사이즈 읽을 수 있는 최소한의 크기 = 2)
	if (m_dataInBuffer >= 2)
	{
		//링버퍼 안에 PacketSize를 읽어올 수 z있으므로 그 이상만큼 들어와있는 데이터가 존재한다면 Parsing();
		if (m_dataInBuffer >= GetPacketSize())
		{
			return Parsing();
		}

		//데이터가 2이상있지만 패킷 사이즈만큼 충분히 받지 못할 경우
		//return nullptr;
	}

	//데이터가 없거나 1들어와서 크기조차 알 수 없을 경우
	return nullptr;
}

char* CRingBuffer::Parsing()
{
	//버퍼의 끝지점부터 읽는 지점까지의 사이즈
	int remainedSize = (int)(m_bufferEndPoint - m_parsingPoint);
	//파싱할 데이터는 읽는 지점부터 읽어온다.
	char* parsingData = m_parsingPoint;

	//버퍼의 끝지점부터 읽는 지점까지 사이즈가 1밖에 없는 경우 -> TempBuffer쪽으로 복사함
	if (remainedSize == 1)
	{
		char* tempBuffer = m_bufferStartPoint - 1;
		*tempBuffer = *m_parsingPoint;
		parsingData = m_parsingPoint = tempBuffer;
		//이후 parsingPoint는 패킷 사이즈만큼 더해준다.
		m_parsingPoint += GetPacketSize();

		return parsingData;
	}

	//버퍼의 끝지점부터 읽는 지점이 같다 -> 버퍼의 첫지점으로 읽는 지점을 옮겨야한다.
	if (remainedSize == 0)
	{
		m_parsingPoint = m_bufferStartPoint;
	}

	unsigned short packetSize = GetPacketSize();

	//패킷 사이즈가 남아있는 크기보다 크다. -> 끝이 짤렸다.
	if (packetSize > remainedSize)
	{
		//맨 앞으로 복사한다.
		memcpy(m_bufferStartPoint - remainedSize, m_parsingPoint, remainedSize);
		parsingData = m_parsingPoint = (m_bufferStartPoint - remainedSize);
	}

	m_parsingPoint += packetSize;

	if (m_parsingPoint == m_bufferEndPoint)
	{
		m_parsingPoint = m_bufferStartPoint;
	}

	return parsingData;
}

unsigned short CRingBuffer::GetWriteableSize()
{
	unsigned short size;

	//평범한 경우
	if (m_readPoint < m_writePoint)
	{
		size = (unsigned short)(m_bufferEndPoint - m_writePoint);
	}
	else if (m_readPoint == m_writePoint)
	{
		//readPoint와 writePoint가 같은데
		//데이터가 있는 경우 == 한바퀴 돌아서 같아짐
		if (this->GetDataInBuffer() > 0)
		{
			size = 0;
		}
		//readPoint와 writePoint가 같은데
		//데이터가 없는 경우 == ringBuffer가 데이터를 받는 시점이거나 처리할게 없음(문제X)
		else
		{
			size = (unsigned short)(m_bufferEndPoint - m_writePoint);
		}
	}
	else
	{
		size = (unsigned short)(m_readPoint - m_writePoint);
	}

	return size;
}

unsigned short CRingBuffer::GetDataInBuffer()
{
	unsigned short size;

	if (m_parsingPoint > m_writePoint)
	{
		size = (unsigned short)(m_bufferEndPoint - m_parsingPoint + (m_writePoint - m_bufferStartPoint));
	}
	else
	{
		size = (unsigned short)(m_writePoint - m_parsingPoint);
	}

	return size;
}

unsigned short CRingBuffer::GetPacketSize()
{
	return *(unsigned short*)(m_parsingPoint + 2);
}
