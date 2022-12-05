#include "CAcceptor.h"
#include "CIOContext.h"

CAcceptor::CAcceptor() : m_acceptor(CIOContext::Instance().GetIOContext())
{

}

CAcceptor::CAcceptor(CAcceptor&& _acceptor) : m_acceptor(std::move(_acceptor.m_acceptor))
{

}

CAcceptor::~CAcceptor()
{

}

int CAcceptor::Bind(const char* _pChar, unsigned short _port)
{
	boost::system::error_code errorCode;

	m_address = boost::asio::ip::make_address(_pChar);
	tcp::endpoint endPoint(m_address, _port);

	m_acceptor.open(endPoint.protocol(), errorCode);
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor.bind(endPoint, errorCode);

	return errorCode.value();
}

int CAcceptor::Bind(unsigned short _port)
{
	boost::system::error_code errorCode;

	tcp::endpoint endPoint(boost::asio::ip::tcp::v4(), _port);

	m_acceptor.open(endPoint.protocol(), errorCode);
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor.bind(endPoint, errorCode);

	return errorCode.value();
}

int CAcceptor::Listen()
{
	assert(m_acceptor.is_open() && "Acceptor isn't open");

	boost::system::error_code errorCode;

	m_acceptor.listen(tcp::acceptor::max_listen_connections, errorCode);

	return errorCode.value();
}

int CAcceptor::Accept(CSession& _session)
{
	assert(m_acceptor.is_open() && "Acceptor isn't open");

	boost::system::error_code errorCode;

	m_acceptor.accept(_session.m_socket.GetSocket(), errorCode);

	return errorCode.value();
}

int CAcceptor::Close()
{
	boost::system::error_code errorCode;

	m_acceptor.close(errorCode);

	return errorCode.value();
}