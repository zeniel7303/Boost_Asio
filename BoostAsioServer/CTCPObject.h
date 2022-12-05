#pragma once
#include "CServer.h"
#include "CTCPSession.h"

class CTCPObject
{
private:
	boost::asio::ip::tcp::endpoint m_endPoint;

public:
	CTCPObject();
	CTCPObject(std::string&& _ip, unsigned short _port);
	CTCPObject(boost::asio::ip::tcp::endpoint& _endPoint);
	~CTCPObject();

	int Send(void* _buffer, int _size);
	int Send(std::shared_ptr<CSender>& _sender);

	std::string GetIPAddress();
	unsigned short GetPort();
	boost::asio::ip::tcp::endpoint& GetEndPoint();
};