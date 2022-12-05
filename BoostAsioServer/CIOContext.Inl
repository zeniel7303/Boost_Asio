template<typename T>
auto CIOContext::BindExecuter(T&& _t)
{
	return boost::asio::bind_executor(m_strand, _t);
}

template<typename T>
void CIOContext::Post(T&& _t)
{
	return m_IOContext.post(m_strand, _t);
}