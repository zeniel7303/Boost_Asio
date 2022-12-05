#pragma once
#include "Stdafx.h"

#include "CTcpSocket.h"
#include "CBuffer.h"
#include "CSender.h"
#include "CClientPacketHandler.h"


class CTcpClient
{
public:
	enum class EState
	{
		eConnected,
		eDiconnected
	};

	CTcpClient();
	CTcpClient(unsigned short _bufSize);
	~CTcpClient();

	int Connect(const char* _ip, unsigned short _port);
	int Receive(bool _isRecvFull = false);
	int ProcessPacket();
	int Send(void* _buffer, int _size);
	int Send(std::shared_ptr<CSender>& _sender);
	int Close();
	int SetDisconnect(int _error = 0);

	int TestFunc(char* _pChar);

protected:
	CTcpSocket	m_socket;
	EState		m_state;
	CBuffer		m_buffer;
	int			m_errorCode;
};

