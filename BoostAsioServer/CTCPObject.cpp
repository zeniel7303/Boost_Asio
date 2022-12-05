#include "CTCPObject.h"

CTCPObject::CTCPObject() 
	: m_endPoint()
{

}

CTCPObject::CTCPObject(std::string&& _ip, unsigned short _port)
	: m_endPoint(boost::asio::ip::make_address(_ip), _port)
{

}

CTCPObject::CTCPObject(boost::asio::ip::tcp::endpoint& _endPoint)
	: m_endPoint(_endPoint)
{

}

CTCPObject::~CTCPObject()
{

}

int CTCPObject::Send(void* _buffer, int _size)
{
	return CTCPSession::Instance().Send(_buffer, _size);
}

int CTCPObject::Send(std::shared_ptr<CSender>& _sender)
{
	return CTCPSession::Instance().Send(_sender);
}

std::string	CTCPObject::GetIPAddress()
{
	return m_endPoint.address().to_string();
}

unsigned short CTCPObject::GetPort()
{
	return m_endPoint.port();
}

boost::asio::ip::tcp::endpoint& CTCPObject::GetEndPoint()
{
	return m_endPoint;
}