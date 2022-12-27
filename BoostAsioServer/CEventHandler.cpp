#include "CEventHandler.h"

CEventHandler CEventHandler::m_singleton;

CEventHandler::CEventHandler()
{
	for (int i = 0; i < MAX_EVENT; ++i)
	{
		m_hEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

		if (m_hEvent[i] == NULL)
		{
			int num = WSAGetLastError();
			assert("event handle creating fail");
		}
	}
}

CEventHandler::~CEventHandler()
{
	for (int i = MAX_EVENT; i > 0; --i)
	{
		if (m_hEvent[i - 1]) { CloseHandle(m_hEvent[i - 1]); m_hEvent[i - 1] = 0; }
	}
}

CEventHandler& CEventHandler::Instance()
{
	return m_singleton;
}

const HANDLE* CEventHandler::GetEventHandle()
{
	return m_hEvent;
}

void CEventHandler::SetEventHandle(eHandle e)
{
	SetEvent(m_hEvent[e]);
}