#include "CUdpHandler.h"

CUdpHandler CUdpHandler::m_singleton;

CUdpHandler& CUdpHandler::Instance()
{
	return m_singleton;
}