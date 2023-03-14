#include "CTestServer.h"

#include "Protocol.h"
#include "TestPacketList.h"

CTestServer::CTestServer(CServerSetting* _setting) : CGameServer(_setting)
{
	CSender::Init(30000);

	CPacketHandler::Instance().Register(TEST_PROTOCOL::TEST_ECHO, &Echo);
	CPacketHandler::Instance().Register(TEST_PROTOCOL::TEST_BIGPACKET, &BigPacketTest);

	printf("TestServer Start \n");

	Start();
}

CTestServer::~CTestServer()
{
	printf("TestServer End \n");
}