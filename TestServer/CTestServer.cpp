#include "CTestServer.h"

CTestServer::CTestServer(CServerSetting* _setting) : CGameServer(_setting)
{
	CSender::Init(30000);

	CServerPacketManager::Instance().Init();

	printf("TestServer Start \n");
}

CTestServer::~CTestServer()
{
	printf("TestServer End \n");
}