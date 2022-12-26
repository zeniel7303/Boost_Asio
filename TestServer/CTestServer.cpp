#include "CTestServer.h"
#include "CPacketHandler.h"

#include "PacketList.h"

CTestServer::CTestServer(CServerSetting* _setting) : CGameServer(_setting)
{
	CSender::Init(30000);

	CPacketHandler::Instance().Register(1, &Echo);
	CPacketHandler::Instance().Register(2, &BigPacketTest);

	printf("TestServer Start \n");
}

CTestServer::~CTestServer()
{
	printf("TestServer End \n");
}