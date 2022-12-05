#pragma once
#include "CServer.h"
#include "CGameUser.h"

class CAcceptor
{
private:
	tcp::acceptor m_acceptor;
	boost::asio::ip::address m_address;

public:
	CAcceptor();
	CAcceptor(CAcceptor&& _acceptor);
	~CAcceptor();

	//address 포함
	int Bind(const char* _pChar, unsigned short _port);
	//adress 미포함
	int Bind(unsigned short _port);
	int Listen();

	int Accept(CSession& _session);
	template<typename THandler>
	void AcceptAsync(CSession& _session, THandler&& _handler);
		
	int Close();
};

#include "CAcceptor.Inl"