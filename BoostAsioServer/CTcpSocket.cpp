#include "CTcpSocket.h"
#include "CIOContext.h"

CTcpSocket::CTcpSocket() : m_socket(CIOContext::Instance().GetIOContext())
{

}

CTcpSocket::~CTcpSocket()
{
	Close();
}

int CTcpSocket::Connect(const char* _ip, unsigned short _port)
{
	assert(_ip != nullptr && "ip Error");

	boost::system::error_code error;
	auto address = boost::asio::ip::make_address(_ip, error);
	if (error.value() != 0) return error.value();

	boost::asio::ip::tcp::endpoint endpoint(address, _port);
	m_socket.open(endpoint.protocol(), error);
	if (error.value() != 0) return error.value();

	m_socket.connect(endpoint, error);
	return error.value();
}

int CTcpSocket::Receive(void* _buffer, int _size, int& _receivedSize)
{
	assert(m_socket.is_open() && "socket Error");
	assert(_buffer != nullptr && "buffer Error");
	assert(_size > 0 && "size Error");

	boost::system::error_code error;
	std::size_t size = m_socket.read_some(boost::asio::mutable_buffer(_buffer, _size), error);
	_receivedSize = static_cast<int>(size);
	return error.value();
}

int CTcpSocket::Send(void* _buffer, int _size, int& _sendSize)
{
	assert(m_socket.is_open() && "socket Error");
	assert(_buffer != nullptr && "buffer Error");
	assert(_size > 0 && "size Error");

	boost::system::error_code error;
	std::size_t size = m_socket.write_some(boost::asio::mutable_buffer(_buffer, _size), error);
	_sendSize = static_cast<int>(size);
	return error.value();
}

void CTcpSocket::SetReuse(bool _isReuse)
{
	boost::asio::socket_base::reuse_address reuse(_isReuse);
	m_socket.set_option(reuse);
}

int CTcpSocket::Close()
{
	boost::system::error_code error;

	if (m_socket.is_open())
		m_socket.close(error);

	return error.value();
}