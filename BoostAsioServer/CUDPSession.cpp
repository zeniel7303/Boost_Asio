#include "CUDPSession.h"
#include "CUDPObject.h"

CUDPSession CUDPSession::m_singleton;

CUDPSession::CUDPSession() : m_socket(), m_buffer(0x3fff)
{
	m_buffer.Clear();
}

CUDPSession::~CUDPSession()
{

}

int CUDPSession::Receive()
{
	unsigned short size = m_buffer.GetUsableSize();
	char* aWritePtr = m_buffer.GetWritePoint();

	m_socket.ReceiveAsync(aWritePtr, size, m_endPoint,
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

int CUDPSession::OnReceive(unsigned short _size)
{
	if (_size == 0)
		return E_INVALID_SIZE;

	if (!m_buffer.OnPush(_size))
	{
		m_buffer.Clear();
		return E_BUFFER_FULL;
	}

	unsigned short	size = 0;
	int result = 0;
	char* data = m_buffer.Front(size, result);
	CUDPObject udpObject(m_endPoint);

	assert(size >= sizeof(CPacketHeader) && "Packet Error!");

	while (data != nullptr && result == 0)
	{
		CPacketHeader* header = reinterpret_cast<CPacketHeader*>(data);
		result = CUdpHandler::Instance().Process(header->m_packetNum, header, size, udpObject);
		if (result != 0)
			break;

		data = m_buffer.Front(size, result);
	}

	if (result != 0)
		return result;

	m_buffer.Pop();
	return 0;
}

int CUDPSession::Send(void* _buffer, int _size, CUDPObject& _obejct)
{
	m_socket.SendAsync(_buffer, _size, _obejct.GetEndPoint(),
		[this](const boost::system::error_code& /*pError*/, std::size_t /*pSendSize*/)
		{
		});

	return 0;
}

int CUDPSession::Send(std::shared_ptr<CSender> _sender, CUDPObject& _object)
{
	auto sendPtr = _sender->GetSendPointer();
	auto sendSize = _sender->GetSendSize();

	if (sendPtr == nullptr) return E_SENDER_NULL;

	m_socket.SendAsync(sendPtr, sendSize, _object.GetEndPoint(),
		[this, _sender](const boost::system::error_code& /*pError*/, std::size_t /*pSendSize*/)
		{
		});

	return 0;
}

CUDPSession& CUDPSession::Instance()
{
	return m_singleton;
}