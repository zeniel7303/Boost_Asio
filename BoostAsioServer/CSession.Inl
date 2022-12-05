template<typename T>
void CSession::SetGameObject(std::shared_ptr<T>& _ptr)
{
	static_assert(std::is_base_of<CGameUser, T>::value == true);

	assert(m_gameUser.get() == nullptr && "error");
	assert(_ptr.get() != nullptr && "error");

	m_gameUser = std::static_pointer_cast<CGameUser>(_ptr);
}

template<typename T>
std::shared_ptr<T> CSession::GetGameUser()
{
	static_assert(std::is_base_of<CGameUser, T>::value == true);

	return GetGameUserImpl<T>(std::is_same<CGameUser, T>());
}

template<typename T>
std::shared_ptr<T> CSession::GetGameUserImpl(std::true_type)
{
	return m_gameUser;
}

template<typename T>
std::shared_ptr<T> CSession::GetGameUserImpl(std::false_type)
{
	// sharedPtr -> weakPtr
	return std::dynamic_pointer_cast<T>(CGameUser);
}