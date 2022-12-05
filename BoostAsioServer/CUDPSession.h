#pragma once
#include "CServer.h"
#include "CUdpSocket.h"
#include "CSender.h"
#include "CUdpHandler.h"

class CUDPObject;

class CUDPSession
{
private:
	static CUDPSession m_singleton;

	boost::asio::ip::udp::endpoint m_endPoint;
	CUdpSocket					m_socket;
	CBuffer						m_buffer;

private:
	CUDPSession();

public:
	~CUDPSession();

	int Receive();
	int OnReceive(unsigned short _size);
	int Send(void* _buffer, int _size, CUDPObject& _obejct);
	int Send(std::shared_ptr<CSender> _sender, CUDPObject& _object);

	static CUDPSession& Instance();
};