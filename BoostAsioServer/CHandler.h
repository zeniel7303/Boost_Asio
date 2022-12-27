#pragma once
#include "CServer.h"
#include "CDoubleQueue.h"

#include "CEventHandler.h"

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

template<typename T1, typename T2>
class CHandler
{
public:
	std::map<int, T1> m_packetList;

	CDoubleQueue<std::tuple<CPacketHeader*, T2>>	m_packetQueue;

public:
	CHandler() {}
	~CHandler() {}
	int Register(int _packetNum, T1&& _type);
	template<typename... arg>
	int Process(int _packetNum, arg... _arg);

	int CanParse(int _packetNum);
	void Parsing(std::tuple<CPacketHeader*, T2> _tuple);
};

#include "CHandler.Inl"