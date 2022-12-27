template<typename T1, typename T2>
int CHandler<T1, T2>::Register(int _packetNum, T1&& _type)
{
	auto result = m_packetList.emplace(_packetNum, _type);

	if (result.second == NULL) return E_ALREADY_REGISTERED;

	return 0;
}

template<typename T1, typename T2>
template<typename... arg>
int CHandler<T1, T2>::Process(int _packetNum, arg... _arg)
{
	auto value = m_packetList.find(_packetNum);

	if (value == m_packetList.end()) return E_INVALID_PACKET;

	auto result = value->second(_arg...);
	return result;
}

template<typename T1, typename T2>
int CHandler<T1, T2>::CanParse(int _packetNum)
{
	auto value = m_packetList.find(_packetNum);

	if (value == m_packetList.end()) return E_INVALID_PACKET;

	return 0;
}

template<typename T1, typename T2>
void CHandler<T1, T2>::Parsing(std::tuple<CPacketHeader*, T2> _tuple)
{
	m_packetQueue.AddObject(_tuple);

	CEventHandler::Instance().SetEventHandle(EVENT_RECV);
}