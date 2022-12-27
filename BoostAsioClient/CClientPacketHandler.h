#pragma once
#include "Stdafx.h"
#include "CHandler.h"

using clientHandleType = std::function<int(CPacketHeader*, unsigned short)>;

class CClientPacketHandler : public CHandler<clientHandleType, unsigned short>
{
private:
	static CClientPacketHandler m_singleton;

	CClientPacketHandler() {}

public:
	~CClientPacketHandler() {}

	static CClientPacketHandler& Instance();
};

