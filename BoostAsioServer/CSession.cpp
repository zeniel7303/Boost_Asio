#include "CSession.h"
#include "CGameUser.h"
#include "CFactory.h"
#include "CPacketHandler.h"
#include "CSender.h"

CSession::CSession() : m_socket(), 
m_buffer(8192), m_state(EState::eClosed), m_errorCode(0)
{

}

CSession::~CSession()
{

}

int CSession::Init()
{
	m_buffer.Clear();

	return 0;
}

int CSession::OnAccept()
{
	assert(m_state == EState::eClosed && "state error");
	assert(m_gameUser != nullptr && "gameUser error");

	m_buffer.Clear();
	m_state = EState::eIdle;
	m_gameUser->OnAccept();
	m_socket.SetReuse(true);
	m_errorCode = 0;

	return Receive();
}

int CSession::Receive()
{
	assert(m_gameUser != nullptr && "gameUser error");

	unsigned short size = m_buffer.GetUsableSize();
	char* writePtr = m_buffer.GetWritePoint();

	if (m_state != EState::eIdle)
	{
		// 에러 코드별로 뭔가 만들어야할거같다.
		// 연결이 끊어져있음
		return E_NOT_IDLE;
	}

	// 버퍼가 가득찼음. 연결 끊어야함.
	if (size < 0 || m_buffer.GetBufferSize() < size)
	{
		SetDisconnect(E_BUFFER_FULL);
		return E_BUFFER_FULL;
	}

	auto sharedSession = GetInstance();
	m_socket.ReceiveAsync(writePtr, size,
		[this, sharedSession](const boost::system::error_code& _error, std::size_t _recvSize)
		{
			if (_error.value() != 0 || _recvSize == 0)
			{
				sharedSession->SetDisconnect();
				return;
			}

			int error = sharedSession->OnReceive(static_cast<unsigned short>(_recvSize));
			if (error != 0)
			{
				sharedSession->SetDisconnect(error);
				return;
			}

			if (m_state == EState::eIdle) sharedSession->Receive();
		});

	return 0;
}

int CSession::OnReceive(unsigned short _size)
{
	assert(m_state == EState::eIdle && "state error");

	// 사이즈 에러
	if (_size == 0) return E_INVALID_SIZE;

	if (m_state != EState::eIdle) return E_NOT_IDLE;

	if (!m_buffer.OnPush(_size)) return E_BUFFER_FULL;

	unsigned short size = 0;
	int result = 0;
	char* data = m_buffer.Front(size, result);
	auto gameUser = GetGameUser<CGameUser>();

	while (data != nullptr && gameUser != nullptr && result == 0 && size > 0)
	{
		CPacketHeader* header = reinterpret_cast<CPacketHeader*>(data);
		result = CPacketHandler::Instance().
			Process(header->m_packetNum, header, size, gameUser);

		if (result != 0) break;

		data = m_buffer.Front(size, result);
	}

	if (result != 0) return result;

	m_buffer.Pop();
	return 0;
}

int CSession::Send(void* _buffer, int _size)
{
	{
		ReadLock readLock(*this);

		if (m_state != EState::eIdle) return 0;
	}

	auto sharedSession = GetInstance();
	m_socket.SendAsync(_buffer, _size,
		[this, sharedSession](const boost::system::error_code& _error, std::size_t _sendSize)
		{
			if (_error.value() != 0 || _sendSize == 0) SetDisconnect();
		});

	return 0;
}

int CSession::Send(std::shared_ptr<CSender> _sender)
{
	if (_sender == nullptr) return E_SENDER_NULL;

	auto sendPtr = _sender->GetSendPointer();
	auto sendSize = _sender->GetSendSize();

	if (sendPtr == nullptr) return E_SENDER_NULL;

	{
		ReadLock readLock(*this);

		if (m_state == EState::eDiconnect) return 0;
	}

	auto sharedSession = GetInstance();
	m_socket.SendAsync(sendPtr, sendSize,
		[this, sharedSession](const boost::system::error_code& _error, std::size_t _sendSize)
		{
			if (_error.value() != 0 || _sendSize == 0) SetDisconnect();
		});

	return 0;
}

int CSession::Close()
{
	{
		ReadLock readLock(*this);

		if (m_state == EState::eClosed) return 0;
	}

	{
		WriteLock writeLock(*this);

		m_state = EState::eClosed;
		m_buffer.Clear();
	}

	return 0;
}

int CSession::SetDisconnect(int _error)
{
	assert(_error == 0 && "error is null");

	{
		WriteLock writeLock(*this);

		if (m_state == EState::eIdle)
		{
			m_socket.Close();
			m_state = EState::eDiconnect;

			m_instance = nullptr;
			m_errorCode = _error;

			UnlinkSession();
		}
	}

	return 0;
}

const boost::asio::ip::tcp::socket& CSession::GetSocket() const
{
	return m_socket.GetSocket();
}


boost::asio::ip::tcp::socket& CSession::GetSocket()
{
	return m_socket.GetSocket();
}


const boost::asio::ip::tcp::endpoint CSession::GetEndPoint() const
{
	return m_socket.GetEndPoint();
}
std::shared_ptr<CSession> CSession::GetInstance()
{
	return m_instance;
}

void CSession::SetInstance(std::shared_ptr<CSession>& _ptr)
{
	m_instance = _ptr;
}

void CSession::UnlinkSession()
{
	if (m_gameUser != nullptr)
	{
		m_gameUser->OnDisconnect();
		m_gameUser->Unlink();
		m_gameUser = nullptr;
	}
}

bool CSession::Delete(CSession* _session)
{
	if (_session == nullptr) return false;

	_session->Close();

	return CFactory::Instance().Delete(_session);
}