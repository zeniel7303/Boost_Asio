#pragma once
#include "CServer.h"

enum eHandle : int
{
	EVENT_RECV,
	MAX_EVENT
};

class CEventHandler
{
private:
	static CEventHandler m_singleton;

public:
	HANDLE m_hEvent[MAX_EVENT];

	CEventHandler();
	~CEventHandler();

	static CEventHandler& Instance();
	const HANDLE* GetEventHandle();
	void SetEventHandle(eHandle e);
};