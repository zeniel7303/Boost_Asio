#pragma once
#include "CServer.h"

class CIOContext
{
private:
	static CIOContext				m_singleton;

	boost::asio::io_context			m_IOContext;
	boost::asio::io_context::strand	m_strand;
	boost::asio::io_context::work	m_work;

	CIOContext();
public:	
	~CIOContext();

	void Run();
	void Stop();
	void Restart();

	boost::asio::io_context& GetIOContext();
	static CIOContext& Instance();

	template<typename T>
	auto BindExecuter(T&& _t);
	template<typename T>
	void Post(T&& _t);
};

#include "CIOContext.Inl"