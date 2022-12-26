template<typename T>
int CHandler<T>::Register(int _packetNum, T&& _type)
{
	auto result = m_packetList.emplace(_packetNum, _type);

	if (result.second == NULL) return E_ALREADY_REGISTERED;

	return 0;
}

template<typename T>
template<typename... arg>
int CHandler<T>::Process(int _packetNum, arg... _arg)
{
	auto value = m_packetList.find(_packetNum);

	if (value == m_packetList.end()) return E_INVALID_PACKET;

	auto result = value->second(_arg...);
	return result;
}

template<typename T>
template<typename... arg>
int CHandler<T>::CanParse(int _packetNum, arg... _arg)
{
	auto value = m_packetList.find(_packetNum);

	if (value == m_packetList.end()) return E_INVALID_PACKET;

	return 0;
}