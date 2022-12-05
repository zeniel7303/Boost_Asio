template<typename T>
int CTcpSocket::ConnectAsync(const char* _ip, unsigned short _port, T&& _t)
{
	assert(_ip != nullptr && "ip error");

	boost::system::error_code error;
	auto address = boost::asio::ip::make_address(_ip, error);
	if (error.value() != 0)
		return error.value();

	boost::asio::ip::tcp::endpoint endPoint(address, _port);
	m_socket.open(endPoint.protocol(), error);
	if (error.value() != 0)
		return error.value();

	m_socket.async_connect(endPoint, CIOContext::Instance().BindExecuter(_t));
	return 0;
}

template<typename T>
void CTcpSocket::ReceiveAsync(void* _buffer, int _size, T&& _t)
{
	assert(m_socket.is_open() && "socket error");
	assert(_buffer != nullptr && "buffer error");
	assert(_size > 0 && "size error");

	m_socket.async_read_some(
		boost::asio::mutable_buffer(_buffer, _size),
		CIOContext::Instance().BindExecuter(_t));
}

template<typename T>
void CTcpSocket::SendAsync(void* _buffer, int _size, T&& _t)
{
	assert(m_socket.is_open() && "socket error");
	assert(_buffer != nullptr && "buffer error");
	assert(_size > 0 && "size error");

	m_socket.async_write_some(
		boost::asio::mutable_buffer(_buffer, _size),
		CIOContext::Instance().BindExecuter(_t));
}