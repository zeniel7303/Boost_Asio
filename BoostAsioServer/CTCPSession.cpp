#include "CTCPSession.h"
#include "CTCPObject.h"

CTCPSession CTCPSession::m_singleton;

CTCPSession::CTCPSession() : m_socket(), m_buffer(0x3fff)
{
	m_buffer.Clear();
}

CTCPSession::~CTCPSession()
{

}

int CTCPSession::Receive()
{
	unsigned short size = m_buffer.GetUsableSize();
	char* aWritePtr = m_buffer.GetWritePoint();

	m_socket.ReceiveAsync(aWritePtr, size,
		[this](const boost::system::error_code& pError, std::size_t pRecvSize)
		{
			if (pError.value() != 0 || pRecvSize == 0)
			{
				return;
			}

			int error = OnReceive(static_cast<unsigned short>(pRecvSize));

			error = Receive();
		});

	return 0;
}

int CTCPSession::OnReceive(unsigned short _size)
{
	if (_size == 0)
		return 3;

	if (!m_buffer.OnPush(_size))
	{
		m_buffer.Clear();
		return 2;
	}

	unsigned short	size = 0;
	int result = 0;
	char* data = m_buffer.Front(size, result);
	CTCPObject tcpObject(m_endPoint);

	assert(size >= sizeof(CPacketHeader) && "Packet Error!");

	while (data != nullptr && result == 0)
	{
		CPacketHeader* header = reinterpret_cast<CPacketHeader*>(data);
		result = CTcpHandler::Instance().Process(header->m_packetNum, header, size, tcpObject);
		if (result != 0)
			break;

		data = m_buffer.Front(size, result);
	}

	if (result != 0)
		return result;

	m_buffer.Pop();
	return 0;
}

int CTCPSession::Send(void* _buffer, int _size)
{
	m_socket.SendAsync(_buffer, _size,
		[this](const boost::system::error_code& /*pError*/, std::size_t /*pSendSize*/)
		{
		});

	return 0;
}

int CTCPSession::Send(std::shared_ptr<CSender> _sender)
{
	auto sendPtr = _sender->GetSendPointer();
	auto sendSize = _sender->GetSendSize();

	if (sendPtr == nullptr) return 5;

	m_socket.SendAsync(sendPtr, sendSize,
		[this, _sender](const boost::system::error_code& /*pError*/, std::size_t /*pSendSize*/)
		{
		});

	return 0;
}

CTCPSession& CTCPSession::Instance()
{
	return m_singleton;
}