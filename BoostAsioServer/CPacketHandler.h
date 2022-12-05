#pragma once
#include "CHandler.h"

class CGameUser;

using handleType = std::function<int(CPacketHeader*, unsigned short, std::shared_ptr<CGameUser>&)>;

class CPacketHandler : public CHandler<handleType>
{
private:
	static CPacketHandler m_singleton;

	CPacketHandler() {}
public:
	~CPacketHandler() {}

	static CPacketHandler& Instance();
};