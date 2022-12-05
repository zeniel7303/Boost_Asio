#include "CUDPObject.h"

CUDPObject::CUDPObject()
	: m_endPoint()
{

}

CUDPObject::CUDPObject(std::string&& _ip, unsigned short _port)
	: m_endPoint(boost::asio::ip::make_address(_ip), _port)
{

}

CUDPObject::CUDPObject(boost::asio::ip::udp::endpoint& _endPoint)
	: m_endPoint(_endPoint)
{

}

CUDPObject::~CUDPObject()
{

}

int CUDPObject::Send(void* _buffer, int _size)
{
	return CUDPSession::Instance().Send(_buffer, _size, *this);
}

int CUDPObject::Send(std::shared_ptr<CSender>& _sender)
{
	return CUDPSession::Instance().Send(_sender, *this);
}

std::string	CUDPObject::GetIPAddress()
{
	return m_endPoint.address().to_string();
}

unsigned short CUDPObject::GetPort()
{
	return m_endPoint.port();
}

boost::asio::ip::udp::endpoint& CUDPObject::GetEndPoint()
{
	return m_endPoint;
}