#pragma once
#include "Stdafx.h"

#include "CGameServer.h"
#include "CTestUser.h"

#include "CServerPacketManager.h"

class CTestServer : public CGameServer<CTestUser>
{
public:
	CTestServer(CServerSetting* _setting);
	~CTestServer() override;
};

