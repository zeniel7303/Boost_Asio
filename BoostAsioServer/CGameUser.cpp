#include "CGameUser.h"

CGameUser::CGameUser()
{
	
}

void CGameUser::LinkSession(std::shared_ptr<CSession>& _session)
{
	m_session = _session;
}

void CGameUser::Unlink()
{
	// √ ±‚»≠
	m_session = std::move(std::weak_ptr<CSession>());
}

void CGameUser::SetDisConnect(int _error)
{
	auto shared = m_session.lock();
	if (shared != nullptr)
	{
		shared->SetDisconnect(_error);
	}
}

int CGameUser::Send(void* _buffer, int _size)
{
	auto shared = m_session.lock();
	if (shared != nullptr)
	{
		return shared->Send(_buffer, _size);
	}
	
	return 0;
}

int CGameUser::Send(const std::shared_ptr<CSender>& _sender)
{
	auto shared = m_session.lock();
	if (shared != nullptr)
	{
		return shared->Send(_sender);
	}

	return 0;
}

std::string CGameUser::GetIPAddress()
{
	auto shared = m_session.lock();
	if (shared != nullptr)
	{
		return shared->GetEndPoint().address().to_string();
	}

	return std::string("");
}