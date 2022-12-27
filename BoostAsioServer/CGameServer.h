#pragma once
#include "CServer.h"
#include "CAcceptor.h"
#include "CFactory.h"
#include "CServerSetting.h"
#include "CSender.h"

#include "CEventHandler.h"

#include <thread>

template<typename T>
class CGameServer
{
protected:
	CServerSetting* m_setting;

private:
	//acceptor는 유니크해도 괜찮을 것 같다.
	std::vector<std::unique_ptr<CAcceptor>>				m_acceptorList;
	std::vector<std::thread>							m_threadList;

	int													m_threadCount;

	std::thread											m_mainThread;

	CEventHandler										m_eventHandler;

public:
	CGameServer(CServerSetting* _setting);
	virtual ~CGameServer();

	int Start();
	int GetAcceptorCount();

private:
	void Main();
	void Loop();
	void AddAcceptor(const char* _ip, unsigned short _port);
	int SetAccept(CAcceptor* _acceptor);

	using Process = void (CGameServer::*)();
	Process processFunc[MAX_EVENT];
	void ProcessPacket();
};

#include "CGameServer.Inl"