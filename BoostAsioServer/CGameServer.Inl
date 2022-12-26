template<typename T>
CGameServer<T>::CGameServer(CServerSetting* _setting)
{
	m_setting = _setting;
	assert(m_setting != nullptr && "setting error");

	m_threadCount = (std::thread::hardware_concurrency() * 2) + 1;

	int addressCount = m_setting->GetAddressCount();
	for (int i = 0; i < addressCount; ++i)
	{
		auto addressInfo = m_setting->GetAddress(i);

		AddAcceptor(addressInfo.m_ip, addressInfo.m_port);
	}

	CFactory::Instance().AddObejctPool<CSession>(m_setting->GetSessionCount());
	CFactory::Instance().AddObejctPool<T>(m_setting->GetSessionCount());
	CFactory::Instance().Init();
}

template<typename T>
CGameServer<T>::~CGameServer()
{
	for (auto& acceptor : m_acceptorList)
	{
		acceptor->Close();
	}

	CFactory::Instance().Close();

	CIOContext::Instance().Stop();
}

template<typename T>
int CGameServer<T>::Start()
{
#ifdef _DEBUG
	assert(m_acceptorList.size() > 0 && "_acceptorList size error");
#else
	if (m_acceptorList.size() <= 0) return 0;
#endif

	for (auto& aAcceptor : m_acceptorList)
	{
		aAcceptor->Listen();
		SetAccept(aAcceptor.get());
	}

	for (int i = 0; i < m_threadCount; ++i)
	{
		m_threadList.emplace_back(
			std::thread{ std::bind(&CGameServer::Main, this) });
	}

	for (auto& aThread : m_threadList)
	{
		aThread.join();
	}

	return 0;
}

template<typename T>
int CGameServer<T>::GetAcceptorCount()
{
	return m_acceptorList.size();
}

template<typename T>
void CGameServer<T>::Main()
{
	try
	{
		CIOContext::Instance().Run();
	}
	catch (std::exception& _exception)
	{
		assert(0 && "IOContext Run Failed");
		UNREFERENCED_PARAMETER(_exception);
	}
}

template<typename T>
void CGameServer<T>::AddAcceptor(const char* _ip, unsigned short _port)
{
	std::unique_ptr<CAcceptor> acceptor = std::make_unique<CAcceptor>();

	if (_ip == nullptr)
	{
		acceptor->Bind(_port);
	}
	else
	{
		acceptor->Bind(_ip, _port);
	}

	m_acceptorList.emplace_back(std::move(acceptor));
}

template<typename T>
int CGameServer<T>::SetAccept(CAcceptor* _acceptor)
{
	auto session = CFactory::Instance().New<CSession>(&CSession::Delete);
	if (session == nullptr)
	{
		//세션을 다 쓴 경우
		SetAccept(_acceptor);

		return false;
	}

	auto gameUser = CFactory::Instance().New<T>();
	if (gameUser == nullptr)
	{
		//남은 세션은 있는데 게임유저는 부족한 경우
		session->SetDisconnect(E_NOT_ENOUGH_RESOURCE);

		SetAccept(_acceptor);

		return false;
	}

	session->SetGameObject(gameUser);
	gameUser->SetSession(session);
	auto sessionPtr = session.get();

	//https://github.com/jacking75/boost_asio_sample/blob/master/Document/asio_cpp11.md
	_acceptor->AcceptAsync(*sessionPtr,
		[this, session, _acceptor](const boost::system::error_code& _error) mutable
	{
		if (_error.value() != 0)
		{
			session->SetDisconnect(_error.value());
			SetAccept(_acceptor);

			return;
		}

		auto sharedSession = std::move(session);
		sharedSession->SetInstance(sharedSession);
		int error = sharedSession->OnAccept();
		if (error != 0)
		{
			sharedSession->SetDisconnect(error);
			SetAccept(_acceptor);

			return;
		}

		SetAccept(_acceptor);
	});

	return 0;
}