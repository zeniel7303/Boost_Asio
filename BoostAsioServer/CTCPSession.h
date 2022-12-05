#pragma once
#include "CServer.h"
#include "CTcpSocket.h"
#include "CSender.h"
#include "CTcpHandler.h"

class CTCPObject;

class CTCPSession
{
private:
	static CTCPSession m_singleton;

	boost::asio::ip::tcp::endpoint m_endPoint;
	CTcpSocket					m_socket;
	CBuffer						m_buffer;

private:
	CTCPSession();

public:
	~CTCPSession();

	int Receive();
	int OnReceive(unsigned short _size);
	int Send(void* _buffer, int _size);
	int Send(std::shared_ptr<CSender> _sender);

	static CTCPSession& Instance();
};