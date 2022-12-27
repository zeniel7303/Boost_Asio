#pragma once
#include "CServer.h"

#define	SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] (p); (p) = NULL;} }

class CRingBuffer
{
private:
	//버퍼 사이즈
	int			m_bufferMaxSize;
	//임시 저장 버퍼 사이즈
	int			m_tempBufferSize;
	//버퍼 주소
	char*		m_buffer;
	//읽는 지점
	char*		m_readPoint;
	//파싱 지점
	char*		m_parsingPoint;
	//쓰는 지점
	char*		m_writePoint;
	//버퍼의 시작 지점
	char*		m_bufferStartPoint;
	//버퍼의 끝 지점
	char*		m_bufferEndPoint;
	//버퍼 안 데이터 크기
	int			m_dataInBuffer;

public:
	CRingBuffer(int _size, int _tempSize);
	~CRingBuffer();

	//초기화
	void Reset();
	//_size만큼 쓰기,읽기
	void Write(int _size);
	void Read(int _size);
	//파싱 가능한지 여부 체크
	char* CanParsing();
	//파싱
	char* Parsing();

	//쓸 수 있는, 읽을 수 있는 버퍼의 크기 계산
	unsigned short GetWriteableSize();
	//링버퍼 안에 있는 데이터의 총 크기
	unsigned short GetDataInBuffer();
	//패킷 사이즈 
	unsigned short GetPacketSize();

	char* GetWritePoint() { return m_writePoint; }
	char* GetReadPoint() { return m_readPoint; }
	int GetBufferSize() { return m_bufferMaxSize; }
};

