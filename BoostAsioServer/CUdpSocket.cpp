#include "CUdpSocket.h"
#include "CIOContext.h"

CUdpSocket::CUdpSocket() : m_socket(CIOContext::Instance().GetIOContext())
{

}

CUdpSocket::~CUdpSocket()
{
	Close();
}

int CUdpSocket::Receive(void* _buffer, int _size, int& _receivedSize, boost::asio::ip::udp::endpoint& _endPoint)
{
	assert(m_socket.is_open() && "socket Error");
	assert(_buffer != nullptr && "buffer Error");
	assert(_size > 0 && "size Error");

	boost::system::error_code error;
	std::size_t size = m_socket.receive_from(boost::asio::mutable_buffer(_buffer, _size), _endPoint, 0, error);
	_receivedSize = static_cast<int>(size);
	return error.value();
}

int CUdpSocket::Send(void* _buffer, int _size, int& _sendSize, boost::asio::ip::udp::endpoint& _endPoint)
{
	assert(m_socket.is_open() && "socket Error");
	assert(_buffer != nullptr && "buffer Error");
	assert(_size > 0 && "size Error");

	boost::system::error_code error;
	std::size_t size = m_socket.send_to(boost::asio::mutable_buffer(_buffer, _size), _endPoint, 0, error);
	_sendSize = static_cast<int>(size);
	return error.value();
}

void CUdpSocket::SetReuse(bool _isReuse)
{
	boost::asio::socket_base::reuse_address reuse(_isReuse);
	m_socket.set_option(reuse);
}

int CUdpSocket::Close()
{
	boost::system::error_code error;

	if (m_socket.is_open())
		m_socket.close(error);

	return error.value();
}