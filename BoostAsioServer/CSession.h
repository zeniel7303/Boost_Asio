#pragma once
#include "CServer.h"
#include "CTcpSocket.h"
#include "CBuffer.h"
#include "CSharedLock.h"

class CSender;
class CGameUser;

class CSession : public CSharedLock<CSession>
{
	friend class CAcceptor;

public:
	enum EState
	{
		eIdle,
		eDiconnect,
		eClosed,
	};

protected:
	CTcpSocket		m_socket;

	EState			m_state;

	int				m_errorCode;

private:
	std::shared_ptr<CSession> m_instance;
	std::shared_ptr<CGameUser> m_gameUser;

	CBuffer			m_buffer;

public :
	CSession();
	virtual ~CSession();

	virtual int Init();
	int OnAccept();
	int Receive();
	int OnReceive(unsigned short _size);
	int Send(void* _buffer, int _size);
	int Send(std::shared_ptr<CSender> _sender);
	virtual int Close();
	virtual int SetDisconnect(int _error = 0);

	const boost::asio::ip::tcp::socket& GetSocket() const;
	boost::asio::ip::tcp::socket& GetSocket();
	const boost::asio::ip::tcp::endpoint GetEndPoint() const;

	std::shared_ptr<CSession>	GetInstance();
	void SetInstance(std::shared_ptr<CSession>& _ptr);
	void UnlinkSession();

	static bool Delete(CSession* _session);

	template<typename T>
	void SetGameObject(std::shared_ptr<T>& _ptr);
	template<typename T = CGameUser>
	std::shared_ptr<T> GetGameUser();
	template<typename T>
	std::shared_ptr<T> GetGameUserImpl(std::true_type);
	template<typename T>
	std::shared_ptr<T> GetGameUserImpl(std::false_type);
};

#include "CSession.Inl"