#pragma once
#include "CServer.h"
#include <functional>
#include <map>

#pragma pack(push, 1)
class CPacketHeader
{
public:
	unsigned short m_packetNum;
	unsigned short m_dataSize;
};
#pragma pack(pop)

template<typename T>
class CHandler
{
public:
	std::map<int, T> m_packetList;

public:
	CHandler() {}
	~CHandler() {}
	int Register(int _packetNum, T&& _type);
	template<typename... arg>
	int Process(int _packetNum, arg... _arg);
};

#include "CHandler.Inl"