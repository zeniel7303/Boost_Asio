#pragma once
#include "Stdafx.h"

#include "CGameServer.h"
#include "CTestUser.h"

#include "PacketList.h"

#include "CDoubleQueue.h"

enum eHandle : int
{
	EVENT_RECV,
	MAX_EVENT
};

class CTestServer : public CGameServer<CTestUser>
{
private:
	HANDLE	m_hEvent[MAX_EVENT];

	CDoubleQueue<CPacketHeader> m_packetQueue;

public:
	CTestServer(CServerSetting* _setting);
	~CTestServer() override;

	int Start() override;
};

