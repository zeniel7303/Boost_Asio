#include "CTcpHandler.h"

CTcpHandler CTcpHandler::m_singleton;

CTcpHandler& CTcpHandler::Instance()
{
	return m_singleton;
}