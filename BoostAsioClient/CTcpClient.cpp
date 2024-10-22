#include "CTcpClient.h"

#include "PacketList.h"

CTcpClient::CTcpClient() : m_socket(), m_buffer(8192),
m_state(EState::eDiconnected), m_errorCode(0)
{
	m_buffer.Clear();

	CSender::Init(30000);

	CClientPacketHandler::Instance().Register(1, &Echo);
	CClientPacketHandler::Instance().Register(2, &BigPacketTest);
}

CTcpClient::CTcpClient(unsigned short _bufSize) : m_socket(), m_buffer(_bufSize),
m_state(EState::eDiconnected), m_errorCode(0)
{
	m_buffer.Clear();
}

CTcpClient::~CTcpClient()
{

}

int CTcpClient::Connect(const char* _ip, unsigned short _port)
{
	int aResult = m_socket.Connect(_ip, _port);

	if (aResult == 0)
		m_state = EState::eConnected;

	return aResult;
}

int CTcpClient::Receive(bool _isRecvFull)
{
	unsigned short size = m_buffer.GetUsableSize();

	// ���۰� ����á��. ���� �������.
	if (size < 0 || m_buffer.GetBufferSize() < size)
	{
		SetDisconnect(E_BUFFER_FULL);
		return E_BUFFER_FULL;
	}

	if (_isRecvFull) size = 0;

	int receivedDataSize = 0;
	char* writePtr = m_buffer.GetWritePoint();
	int errorCode = m_socket.Receive(writePtr, size, receivedDataSize);
	if (errorCode != 0)
	{
		SetDisconnect(errorCode);
		return errorCode;
	}

	if (size == 0 && !_isRecvFull)
	{
		SetDisconnect(E_INVALID_PACKET);
		return E_INVALID_PACKET;
	}

	if (!m_buffer.OnPush(static_cast<unsigned short>(receivedDataSize)))
	{
		SetDisconnect(E_BUFFER_FULL);
		return E_BUFFER_FULL;
	}

	return 0;
}

int CTcpClient::ProcessPacket()
{
	if (m_state == EState::eDiconnected)
	{
		return E_DISCONNECTED;
	}

	unsigned short size = 0;
	int result = 0;
	char* data = m_buffer.Front(size, result);

	while (data != nullptr && result == 0 && size > 0)
	{
		CPacketHeader* header = reinterpret_cast<CPacketHeader*>(data);
		result = CClientPacketHandler::Instance().Process(header->m_packetNum, header, size);
		if (result != 0)
			break;

		data = m_buffer.Front(size, result);
	}

	if (result != 0)
		return result;

	m_buffer.Pop();
	return 0;
}

int CTcpClient::Send(void* _buffer, int _size)
{
	int sendSize = 0;
	int result = m_socket.Send(_buffer, _size, sendSize);

	if (result != 0 || sendSize == 0)
		SetDisconnect(result);

	return result;
}

int CTcpClient::Send(std::shared_ptr<CSender>& _sender)
{
	if (_sender == nullptr) return E_SENDER_NULL;

	auto sendPtr = _sender->GetSendPointer();
	auto sendSize = _sender->GetSendSize();
	int sendedSize = 0;

	if (sendPtr == nullptr) return E_SENDER_NULL;

	int result = m_socket.Send(sendPtr, sendSize, sendedSize);
	if (result != 0 || sendedSize == 0)
		SetDisconnect(result);

	return result;
}

int CTcpClient::Close()
{
	if (m_state == EState::eDiconnected) return 0;

	return m_socket.Close();
}

int CTcpClient::SetDisconnect(int _error)
{
	m_errorCode = _error;

	return Close();
}

int CTcpClient::TestFunc(char* _pChar)
{
	/*auto sender = CSender::Alloc<CTestPacket>();
	auto packet = sender->GetWritePointer<CTestPacket>();
	packet->m_packetNum = 0;
	strcpy(packet->m_test, _pChar);*/

	auto sender = CSender::Alloc<CTestBigPacket>();
	auto packet = sender->GetWritePointer<CTestBigPacket>();
	packet->m_packetNum = 1;
	strcpy(packet->m_bigData, _pChar);

	return Send(sender);
}