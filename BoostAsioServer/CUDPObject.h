#pragma once
#include "CServer.h"
#include "CUDPSession.h"

class CUDPObject
{
private:
	boost::asio::ip::udp::endpoint m_endPoint;

public:
	CUDPObject();
	CUDPObject(std::string&& _ip, unsigned short _port);
	CUDPObject(boost::asio::ip::udp::endpoint& _endPoint);
	~CUDPObject();

	int Send(void* _buffer, int _size);
	int Send(std::shared_ptr<CSender>& _sender);

	std::string GetIPAddress();
	unsigned short GetPort();
	boost::asio::ip::udp::endpoint& GetEndPoint();
};