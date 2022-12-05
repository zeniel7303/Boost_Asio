template<typename T>
void CUdpSocket::ReceiveAsync(void* _buffer, int _size, boost::asio::ip::udp::endpoint& _endPoint, T&& _t)
{
	assert(m_socket.is_open() && "socket error");
	assert(_buffer != nullptr && "buffer error");
	assert(_size > 0 && "size error");

	m_socket.async_receive_from(
		boost::asio::mutable_buffer(_buffer, _size), _endPoint,
		CIOContext::Instance().BindExecuter(_t));
}

template<typename T>
void CUdpSocket::SendAsync(void* _buffer, int _size, boost::asio::ip::udp::endpoint& _endPoint, T&& _t)
{
	assert(m_socket.is_open() && "socket error");
	assert(_buffer != nullptr && "buffer error");
	assert(_size > 0 && "size error");

	m_socket.async_send_to(
		boost::asio::mutable_buffer(_buffer, _size), _endPoint, 
		CIOContext::Instance().BindExecuter(_t));
}