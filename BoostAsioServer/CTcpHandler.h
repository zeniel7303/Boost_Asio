#pragma once
#include "CHandler.h"

class CTCPObject;

using tcpHandleType = std::function<int(CPacketHeader*, unsigned short, CTCPObject&)>;

class CTcpHandler : public CHandler<tcpHandleType>
{
private:
	static CTcpHandler m_singleton;

	CTcpHandler() {}
public:
	~CTcpHandler() {}

	static CTcpHandler& Instance();
};

