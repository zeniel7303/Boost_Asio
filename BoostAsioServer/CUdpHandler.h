#pragma once
#include "CHandler.h"

class CUDPObject;

using udpHandleType = std::function<int(CPacketHeader*, unsigned short, CUDPObject&)>;

class CUdpHandler : public CHandler<udpHandleType>
{
private:
	static CUdpHandler m_singleton;

	CUdpHandler() {}
public:
	~CUdpHandler() {}

	static CUdpHandler& Instance();
};

