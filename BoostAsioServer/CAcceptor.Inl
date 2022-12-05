template<typename THandler>
void CAcceptor::AcceptAsync(CSession& _session, THandler&& _handler)
{
	assert(m_acceptor.is_open() && "Acceptor isn't open");

	m_acceptor.async_accept(_session.m_socket.GetSocket(), (_handler));
}