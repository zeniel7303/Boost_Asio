#include "CIOContext.h"

CIOContext CIOContext::m_singleton;

CIOContext::CIOContext() : m_IOContext(), m_strand(m_IOContext), m_work(m_IOContext)
{

}

CIOContext::~CIOContext()
{
	
}

void CIOContext::Run()
{
	m_IOContext.run();
}

void CIOContext::Stop()
{
	m_IOContext.stop();
}

void CIOContext::Restart()
{
	m_IOContext.restart();
}

boost::asio::io_context& CIOContext::GetIOContext()
{
	return m_IOContext;
}

CIOContext& CIOContext::Instance()
{
	return m_singleton;
}