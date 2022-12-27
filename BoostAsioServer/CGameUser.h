#pragma once
#include "CSession.h"

class CGameUser
{
protected:
	std::weak_ptr<CSession> m_session;

public:
	CGameUser();
	virtual ~CGameUser() {}

	void SetSession(std::shared_ptr<CSession>& _session);
	void Unlink();
	virtual void OnAccept() {}
	virtual void OnDisconnect() {}
	void SetDisConnect(int _error = 0);
	int Send(void* _buffer, int _size);
	int Send(const std::shared_ptr<CSender>& _sender);
	void Read(int _size);

	std::string GetIPAddress();
};

