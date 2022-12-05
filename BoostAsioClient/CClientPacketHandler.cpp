#include "CClientPacketHandler.h"

CClientPacketHandler CClientPacketHandler::m_singleton;

CClientPacketHandler& CClientPacketHandler::Instance()
{
	return m_singleton;
}