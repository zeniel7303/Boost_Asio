#pragma once
#include "CPacketHandler.h"

enum TEST_PROTOCOL
{
	TEST_ECHO = 0,
	TEST_BIGPACKET,
};

//================================================================

#pragma pack(push, 1)
class CTestPacket : public CPacketHeader
{
public:
	CTestPacket() = default;
	~CTestPacket() = default;

	char m_test[256];
};

class CTestBigPacket : public CPacketHeader
{
public:
	CTestBigPacket() = default;
	~CTestBigPacket() = default;

	char m_bigData[2048];
};
#pragma pack(pop)