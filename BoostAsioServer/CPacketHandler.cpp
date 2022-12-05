#include "CPacketHandler.h"

CPacketHandler CPacketHandler::m_singleton;

CPacketHandler& CPacketHandler::Instance()
{
	return m_singleton;
}